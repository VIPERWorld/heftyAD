#include "JSFileParser.h"

JSFileParser::JSFileParser(QObject *parent)
    : QObject(parent)
{
}

bool JSFileParser::loadFile(const QString &filePath)
{
    m_filePath = filePath;
    m_jsValue = m_jsEngine.evaluateFromFile(filePath);
    return load();
}

bool JSFileParser::loadProgram(const QString &program)
{
    m_filePath = "";
    m_jsValue = m_jsEngine.evaluate(program);
    return load();
}

QString JSFileParser::errorText()
{
    if(m_jsValue.isUndefined()) {
        if(!m_filePath.isEmpty()) {
            return trUtf8("Le fichier '%1' est inaccessible.").arg(m_filePath);
        }

        return trUtf8("Le chargement de l'algorithme est un échec.");
    }

    if(m_jsValue.isError()) {
        const QJSValue &line = m_jsValue.property("lineNumber");
        return trUtf8("Ligne %1").arg(line.toInt()) + "\n" + m_jsValue.toString();
    }

    return "";
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
