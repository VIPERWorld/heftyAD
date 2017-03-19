#include "ModelReader.h"

#include <QFile>

ModelReader::ModelReader(Model *model)
    : QObject(),
      m_model(model)
{
}

Model* ModelReader::model() const {return m_model;}

bool ModelReader::read(const QString &filePath)
{
    QFile in(filePath);
    if(!in.open(QIODevice::ReadOnly)) {
        return false;
    }

    return read(in);
}
