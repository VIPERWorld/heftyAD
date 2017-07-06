#include "ArrayModel.h"
#include "GraphModel.h"
#include "JSAlgorithm.h"
#include "JSFileParser.h"
#include "SimulationClarifier.h"
#include "SimulationHighlighter.h"
#include "SimulationLocker.h"

JSAlgorithm::JSAlgorithm(JSFileParser &parser)
    : Algorithm(),
      m_parser(parser)
{
    exposeAttributes();

    connect(&parser, &JSFileParser::reseted,     this, &JSAlgorithm::onParserReseted);
    connect(this,    &JSAlgorithm::modelChanged, this, &JSAlgorithm::onModelChanged);
}

bool JSAlgorithm::requiresAModel() const
{
    const QString &requiredModel = m_parser.algorithmRequiredModel();
    return !requiredModel.isEmpty() && requiredModel != "none";
}

bool JSAlgorithm::hasAValidModel() const
{
    const QString &requiredModel = m_parser.algorithmRequiredModel();
    if(requiredModel == "any"  ) return dynamic_cast<Model*>(m_model);
    if(requiredModel == "array") return dynamic_cast<ArrayModel*>(m_model);
    if(requiredModel == "graph") return dynamic_cast<GraphModel*>(m_model);

    return false;
}

void JSAlgorithm::preExecute()  {QJSValue func = m_parser.algorithmPreExecFunc();  callJSFunction(func);}
void JSAlgorithm::execute()     {QJSValue func = m_parser.algorithmExecFunc();     callJSFunction(func);}
void JSAlgorithm::postExecute() {QJSValue func = m_parser.algorithmPostExecFunc(); callJSFunction(func);}

void JSAlgorithm::exposeAttributes()
{
    m_parser.setGlobalVariable("locker",      m_locker);
    m_parser.setGlobalVariable("clarifier",   m_clarifier);
    m_parser.setGlobalVariable("highlighter", m_highlighter);
}

void JSAlgorithm::callJSFunction(QJSValue &jsFunction) const
{
    const QJSValue &jsValue = jsFunction.call();
    if(jsValue.isError()) {
        const QJSValue &line = jsValue.property("lineNumber");
        const QString &error = QString("Line %1").arg(line.toInt()) + ", " + jsValue.toString();
        throw std::runtime_error(error.toStdString());
    }
}

void JSAlgorithm::onModelChanged()
{
    m_parser.exposeCppModel(m_model);
}

void JSAlgorithm::onParserReseted()
{
    exposeAttributes();
    if(requiresAModel()) { // expose model only if it's required
        onModelChanged();
    }
}
