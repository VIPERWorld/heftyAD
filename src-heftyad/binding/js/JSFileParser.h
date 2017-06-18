#ifndef JSFILEPARSER_H
#define JSFILEPARSER_H

#include "scripting/ScriptEngine.h"

#include <QJSValue>
#include <QObject>

class JSFileParser : public QObject
{
    Q_OBJECT

public:
    typedef struct {
        QString name;
        QString description; // a (preferably short) description about this author.
    } Author;

private:
    QString m_filePath;

    ScriptEngine m_jsEngine;
    QJSValue m_jsValue;
        QJSValue m_metadata;
        QJSValue m_code;

public:
    explicit JSFileParser(QObject *parent = 0);

    bool loadFile(const QString &filePath);
    bool loadProgram(const QString &program);
    QString errorText();

    QString algorithmName() const;
    QString algorithmDescription() const;
    QList<Author> algorithmAuthors() const;

    QString algorithmRequiredModel() const;
    QJSValue algorithmPreExecFunc() const;
    QJSValue algorithmExecFunc() const;
    QJSValue algorithmPostExecFunc() const;

protected:
    bool load();

signals:

public slots:
};

#endif // JSFILEPARSER_H
