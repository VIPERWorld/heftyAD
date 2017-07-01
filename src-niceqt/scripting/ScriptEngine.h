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

    /*
     * Be aware that ownership defaults to JavaScriptOwnership.
     * So upon destruction QJSEngine will track and delete any exposed orphan QObject (the ones with no parent).
     * This behaviour can be overridden like this:
     *     QQmlEngine::setObjectOwnership(QObject*, QQmlEngine::CppOwnership);
     */
    void setGlobalVariable(const QString &name, QObject *object);

    static QString getStringProperty(const QString &propertyName, const QJSValue &from);

signals:

public slots:
};

#endif // SCRIPTENGINE_H
