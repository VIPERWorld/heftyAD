#include "ScriptEngine.h"

#include <QFile>

ScriptEngine::ScriptEngine(QObject *parent)
    : QJSEngine(parent)
{
}

ScriptEngine::~ScriptEngine()
{
}

QJSValue ScriptEngine::evaluateFromFile(const QString &filePath, int lineNumber)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QJSValue(); // Returns an undefined value -> use QJSValue::isUndefined() to check
    }

    return evaluate(file.readAll(), filePath, lineNumber);
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
