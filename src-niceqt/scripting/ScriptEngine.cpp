#include "ScriptEngine.h"

#include <QFile>

ScriptEngine::ScriptEngine(QObject *parent)
    : QJSEngine(parent)
{
}

ScriptEngine::~ScriptEngine()
{
    collectGarbage(); // see QJSEngine destructor
}

QJSValue ScriptEngine::evaluateFromFile(const QString &filePath, int lineNumber)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QJSValue(); // Returns an undefined value -> use QJSValue::isUndefined() to check
    }

    return evaluate(file.readAll(), filePath, lineNumber);
}

void ScriptEngine::setGlobalVariable(const QString &name, QObject *object)
{
    QJSValue value = newQObject(object);
    globalObject().setProperty(name, value);
}

QString ScriptEngine::getStringProperty(const QString &propertyName, const QJSValue &from)
{
    QString retVal;
    if(!from.isUndefined() && !from.isError()) {
        const QJSValue &v = from.property(propertyName);
        if(v.isString()) {
            retVal = v.toString();
        }
    }

    return retVal;
}
