#ifndef JSFILEPARSER_H
#define JSFILEPARSER_H

#include <QJSValue>
#include <QObject>
#include <QPointer>

class Model;
class ScriptEngine;

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

    ScriptEngine *m_jsEngine;
    QJSValue m_jsValue;
        QJSValue m_metadata;
        QJSValue m_code;

    QPointer<Model> m_model;

public:
    explicit JSFileParser(QObject *parent = 0);

    void exposeCppModel(Model *model);
    void setGlobalVariable(const QString &name, QObject *object);

    bool loadFile(const QString &filePath);
    bool loadProgram(const QString &program);
    QString errorText();
    /**
     * Makes sure any element(function, variable, ...) that is no longer defined in JS code would be no more available.
     * When such an element is referenced from JS code, the following error would be yield:
     *     ReferenceError: <symbol_name> is not defined
     */
    void reset();

    QString algorithmName() const;
    QString algorithmDescription() const;
    QList<Author> algorithmAuthors() const;

    QString algorithmRequiredModel() const;
    QJSValue algorithmPreExecFunc() const;
    QJSValue algorithmExecFunc() const;
    QJSValue algorithmPostExecFunc() const;

protected:
    bool load();

private:
    void exposeCommonElements();
    void exposeTypes();
    void exposeCppModel();

signals:

public slots:
};

#endif // JSFILEPARSER_H
