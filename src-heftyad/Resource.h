#ifndef RESOURCE_H
#define RESOURCE_H

#include "singleton/Singleton.hpp"

#include <QIcon>
#include <QString>

/**
 * The Resource class gives access to all heftyAD resources.
 *
 * These resources basically involve
 *      icons,
 *      several heftyAD specific directories (e.g. the translation dir),
 *      ...,
 *      or even methods such as loadStyleSheets(QString) which ...
 *
 * If heftyAD were a C program, this resource file would presumably
 * contain declarations like:
 *      #define WINDOW_ICON  "path_to_window_icon.png"
 *      #define TOOLBAR_ICON "path_to_toolbar_icon.png"
 *      #define ... ...
 *
 * [To be completed]
 */

class Resource : public ncpp::Singleton<Resource>
{
    friend class Singleton<Resource>;

private:
    Resource();
    ~Resource();

public:
    void makeUserDirs(void) const;

    QString userCommonDir(void) const;
    QString userBackupDir(void) const;
    QString userAppPluginDir(void) const;
    QString userAppPropertyDir(void) const;
    QString userAppTranslationDir(void) const;
    QString userAppWorkingModelDir(void) const;

    void loadStyleSheets(QString fileName = "") const;

    QString languageDir(void) const;
    QString languageFileName(QString fileName) const;
    QIcon languageIcon(QString fileName) const;

    QString simulationDir(void) const;
    QString simulationFileName(QString fileName) const;
    QIcon simulationIcon(QString fileName) const;

    QString toolBarDir(void) const;
    QString toolBarFileName(QString fileName) const;
    QIcon toolBarIcon(QString fileName) const;

    QIcon windowIcon(void) const;

    QString shadowDir(void) const;
    QString shadowFileName(QString fileName) const;
    QIcon shadowIcon(QString fileName) const;

    QString stylesheetDir(void) const;
    QString stylesheetFileName(QString fileName) const;
    QIcon stylesheetIcon(QString fileName) const;
};

#endif // RESOURCE_H
