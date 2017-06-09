#include "AlgorithmWork.h"
#include "ModelWork.h"

#include "ArrayModel.h"
#include "GraphModel.h"

#include "WorkArrayView.h"
#include "WorkGraphView.h"
#include "WorkHelper.h"

#include <QFile>
#include <QXmlStreamReader>

QStringList WorkHelper::validAlgorithmTypes()
{
    return QStringList() << "algorithm";
}

QStringList WorkHelper::validModelTypes()
{
    return QStringList() << "array" << "graph";
}

Work* WorkHelper::workInstanceByType(const QString &type)
{
    if(validAlgorithmTypes().contains(type)) {
        return new AlgorithmWork;
    }

    if(validModelTypes().contains(type)) {
        View *view = modelViewInstanceByType(type);
        if(view) {
            return new ModelWork(view);
        }
    }

    return nullptr;
}

Work* WorkHelper::workInstanceByFilePath(const QString &filePath, const QString &family)
{
    QString type = family;
    if(family == "model") {
        type = getTypeFromXmlFilePath(filePath);
        if(!validModelTypes().contains(type)) {
            return nullptr;
        }
    }

    Work *work = workInstanceByType(type);
    if(work) {
        work->setFilePath(filePath);
        work->load();
    }
    return work;
}

View* WorkHelper::modelViewInstanceByType(const QString &type)
{
    if(type == "array") {
        return (new WorkArrayView)->withModel(new ArrayModel);
    }

    if(type == "graph") {
        return (new WorkGraphView)->withModel(new GraphModel);
    }

    return nullptr;
}

View* WorkHelper::modelViewInstanceByFilePath(const QString &filePath)
{
    const QString &type = getTypeFromXmlFilePath(filePath);
    View *view = modelViewInstanceByType(type);
    if(view) {
        view->model()->loadFrom(filePath);
    }

    return view;
}

QString WorkHelper::getTypeFromXmlFilePath(const QString &filePath)
{
    QString retVal;

    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly)) {
        QXmlStreamReader xml;
        xml.setDevice(&file);
        if(xml.readNextStartElement()) {
            retVal = xml.name().toString();
        }
    }

    return retVal;
}
