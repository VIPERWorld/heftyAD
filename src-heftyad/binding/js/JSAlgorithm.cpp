#include "ArrayModel.h"
#include "GraphModel.h"
#include "JSAlgorithm.h"
#include "JSFileParser.h"

JSAlgorithm::JSAlgorithm(const JSFileParser &parser)
    : Algorithm(),
      m_parser(parser)
{
    connect(this, &JSAlgorithm::modelChanged, this, &JSAlgorithm::onModelChanged);
}

bool JSAlgorithm::requiresAModel() const
{
    const QString &requiredModel = m_parser.algorithmRequiredModel();
    return !requiredModel.isEmpty() && requiredModel != "none";
}

bool JSAlgorithm::hasAValidModel() const
{
    const QString &requiredModel = m_parser.algorithmRequiredModel();
    if(requiredModel == "array") return dynamic_cast<ArrayModel*>(m_model);
    if(requiredModel == "graph") return dynamic_cast<GraphModel*>(m_model);

    return false;
}

void JSAlgorithm::preExecute()  {throwExceptionIfAny(m_parser.algorithmPreExecFunc ().call());}
void JSAlgorithm::execute()     {throwExceptionIfAny(m_parser.algorithmExecFunc    ().call());}
void JSAlgorithm::postExecute() {throwExceptionIfAny(m_parser.algorithmPostExecFunc().call());}

void JSAlgorithm::throwExceptionIfAny(const QJSValue &jsValue) const
{
    if(jsValue.isError()) {
        const QJSValue &line = jsValue.property("lineNumber");

        QString error;
        error += QString("Line %1").arg(line.toInt()) + ", " + jsValue.toString();
        throw std::runtime_error(error.toStdString());
    }
}

#include <QDebug>
void JSAlgorithm::onModelChanged()
{
    qDebug() << "js algo model changed";
}
