#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <QJSEngine>

class ScriptEngine : public QJSEngine
{
    Q_OBJECT

public:
    ScriptEngine(QObject *parent = 0);
    ~ScriptEngine();

    QJSValue evaluateFromFile(const QString &filePath, int lineNumber = 1);

    static QString getStringProperty(const QJSValue &jsValue, const QString &propertyName);

signals:

public slots:
};

#endif // SCRIPTENGINE_H
