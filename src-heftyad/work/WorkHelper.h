#ifndef WORKHELPER_H
#define WORKHELPER_H

#include <QStringList>

class Model;
class View;
class Work;

class WorkHelper
{
public:
    WorkHelper() = delete;
    WorkHelper(const WorkHelper&) = delete;
    WorkHelper& operator=(const WorkHelper&) = delete;
    WorkHelper(const WorkHelper&&) = delete;

    static QStringList validAlgorithmTypes();
    static QStringList validModelTypes();

    static Work* workInstanceByType(const QString &type);
    /**
     * Returns a work based on the given filepath content.
     * If family is "model", this function returns nullptr if the filepath doesn't refer to a model file.
     * If family is "algorithm", this function returns nullptr if the filepath doesn't refer to an algorithm file.
     * Any weird value of family is merely ignored.
     */
    static Work* workInstanceByFilePath(const QString &filePath, const QString &family = QString());

    static View* modelViewInstanceByType(const QString &type);
    static View* modelViewInstanceByFilePath(const QString &filePath);

private:
    static QString getTypeFromXmlFilePath(const QString &filePath);
};

#endif // WORKHELPER_H
