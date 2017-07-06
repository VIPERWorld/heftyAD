#include "JSFileParser.h"
#include "Model.h"

#include "graphics/highlighting/HighlightingData.h"
#include "graphics/highlighting/HighlightingTextData.h"

#include "scripting/ScriptEngine.h"
#include "scripting/ScriptUltimateObject.h"

#include <QQmlEngine>

JSFileParser::JSFileParser(QObject *parent)
    : QObject(parent),
      m_jsEngine(new ScriptEngine(this)),
      m_model(nullptr)
{
//    m_jsEngine->installExtensions(QJSEngine::TranslationExtension | QJSEngine::ConsoleExtension);

    exposeCommonElements();
}

void JSFileParser::exposeCppModel(Model *model)
{
    if(m_model == model) {
        return;
    }

    if(m_model) {
        m_model->setJSEngine(nullptr);
    }
    m_model = model;
    exposeCppModel();
}

void JSFileParser::setGlobalVariable(const QString &name, QObject *object)
{
    m_jsEngine->setGlobalVariable(name, object);
}

bool JSFileParser::loadFile(const QString &filePath)
{
    m_filePath = filePath;
    m_jsValue = m_jsEngine->evaluateFromFile(filePath);
    return load();
}

bool JSFileParser::loadProgram(const QString &program)
{
    m_filePath = "";
    m_jsValue = m_jsEngine->evaluate(program);
    return load();
}

QString JSFileParser::errorText()
{
    if(m_jsValue.isUndefined()) {
        if(!m_filePath.isEmpty()) {
            return trUtf8("Le fichier '%1' est inaccessible.").arg(m_filePath);
        }

        return trUtf8("L'algorithme est syntaxiquement incorrect.");
    }

    if(m_jsValue.isError()) {
        const QJSValue &line = m_jsValue.property("lineNumber");
        return trUtf8("Ligne %1").arg(line.toInt()) + "\n" + m_jsValue.toString();
    }

    return "";
}

void JSFileParser::reset()
{
    delete m_jsEngine;
    m_jsEngine = new ScriptEngine(this);
    exposeCommonElements();
    exposeCppModel();
    loadProgram("");

    emit reseted();
}

QString JSFileParser::algorithmName() const {return ScriptEngine::getStringProperty("name", m_metadata);}
QString JSFileParser::algorithmDescription() const {return ScriptEngine::getStringProperty("description", m_metadata);}
QList<JSFileParser::Author> JSFileParser::algorithmAuthors() const
{
    QList<Author> retVal;

    if(!m_metadata.isUndefined()) {
        const QJSValue &jsvAuthors = m_metadata.property("authors");
        if(!jsvAuthors.isUndefined()) {
            int i = 0;
            QJSValue jsvAuth = jsvAuthors.property(i);
            while(!jsvAuth.isUndefined()) {
                Author author;
                author.name = ScriptEngine::getStringProperty("name", jsvAuth);
                author.description = ScriptEngine::getStringProperty("description", jsvAuth);
                retVal.append(author);

                i++;
                jsvAuth = jsvAuthors.property(i);
            }
        }
    }

    return retVal;
}

QString JSFileParser::algorithmRequiredModel() const {return ScriptEngine::getStringProperty("requires", m_code);}
QJSValue JSFileParser::algorithmPreExecFunc() const {return m_code.property("preExecute");}
QJSValue JSFileParser::algorithmExecFunc() const {return m_code.property("execute");}
QJSValue JSFileParser::algorithmPostExecFunc() const {return m_code.property("postExecute");}

bool JSFileParser::load()
{
    const bool valid = !m_jsValue.isUndefined() && !m_jsValue.isError();
    if(!valid) {
        m_metadata = QJSValue();
        m_code = QJSValue();
    }
    else {
        m_metadata = m_jsValue.property("metadata");
        m_code = m_jsValue.property("code");
    }

    return valid;
}

void JSFileParser::exposeCommonElements()
{
    setGlobalVariable("sympa", new ScriptUltimateObject);
    exposeTypes();
}

void JSFileParser::exposeTypes()
{
    QJSValue value;

    value = m_jsEngine->newQMetaObject<HighlightingData>();
    m_jsEngine->globalObject().setProperty("HighlightingData", value);

    value = m_jsEngine->newQMetaObject<HighlightingTextData>();
    m_jsEngine->globalObject().setProperty("HighlightingTextData", value);
}

void JSFileParser::exposeCppModel()
{
    if(m_model) {
        m_model->setJSEngine(m_jsEngine);
    }
    setGlobalVariable("model", m_model);
    QQmlEngine::setObjectOwnership(m_model, QQmlEngine::CppOwnership);
}
