#ifndef JSALGORITHMPARSER_H
#define JSALGORITHMPARSER_H

#include "scripting/ScriptEngine.h"

#include <QJSValue>
#include <QObject>

class JSAlgorithmParser : public QObject
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

public:
    explicit JSAlgorithmParser(QObject *parent = 0);

    bool loadFile(const QString &filePath);
    bool loadProgram(const QString &program);
    QString errorText();

    QString algorithmName() const;
    QString algorithmDescription() const;
    QList<Author> algorithmAuthors() const;

protected:
    bool load();

signals:

public slots:
};

#endif // JSALGORITHMPARSER_H
