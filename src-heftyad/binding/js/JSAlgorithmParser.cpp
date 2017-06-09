#include "JSAlgorithmParser.h"

JSAlgorithmParser::JSAlgorithmParser(QObject *parent)
    : QObject(parent)
{
}

bool JSAlgorithmParser::loadFile(const QString &filePath)
{
    m_filePath = filePath;
    m_jsValue = m_jsEngine.evaluateFromFile(filePath);
    return load();
}

bool JSAlgorithmParser::loadProgram(const QString &program)
{
    m_filePath = "";
    m_jsValue = m_jsEngine.evaluate(program);
    return load();
}

QString JSAlgorithmParser::errorText()
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

QString JSAlgorithmParser::algorithmName() const {return ScriptEngine::getStringProperty(m_metadata, "name");}
QString JSAlgorithmParser::algorithmDescription() const {return ScriptEngine::getStringProperty(m_metadata, "description");}
QList<JSAlgorithmParser::Author> JSAlgorithmParser::algorithmAuthors() const
{
    QList<Author> retVal;

    if(!m_metadata.isUndefined()) {
        const QJSValue &jsvAuthors = m_metadata.property("authors");
        if(!jsvAuthors.isUndefined()) {
            int i = 0;
            QJSValue jsvAuth = jsvAuthors.property(i);
            while(!jsvAuth.isUndefined()) {
                Author author;
                author.name = ScriptEngine::getStringProperty(jsvAuth, "name");
                author.description = ScriptEngine::getStringProperty(jsvAuth, "description");
                retVal.append(author);

                i++;
                jsvAuth = jsvAuthors.property(i);
            }
        }
    }

    return retVal;
}

bool JSAlgorithmParser::load()
{
    bool valid = !m_jsValue.isUndefined() && !m_jsValue.isError();
    m_metadata = valid ? m_jsValue.property("metadata") : QJSValue();
    return valid;
}
