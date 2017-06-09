#ifndef RESOURCE_H
#define RESOURCE_H

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

class Resource
{
public:
    Resource() = delete;
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    Resource(const Resource&&) = delete;
    ~Resource() = delete;

    static void makeUserDirs();
    static void loadStyleSheets(const QString &fileName);
    static QString readTextFile(const QString &filePath);

    static QString userCommonDir();
    static QString userBackupDir();
    static QString userAppPluginDir();
    static QString userAppPropertyDir();
    static QString userAppTranslationDir();
    static QString userAppWorkingModelDir();

    static QString languageDir();
    static QString languageFilePath(const QString &fileName);
    static QIcon languageIcon(const QString &fileName);

    static QString shadowDir();
    static QString shadowFilePath(const QString &fileName);
    static QIcon shadowIcon(const QString &fileName);

    static QString simulationDir();
    static QString simulationFilePath(const QString &fileName);
    static QIcon simulationIcon(const QString &fileName);

    static QString stylesheetDir();
    static QString stylesheetFilePath(const QString &fileName);
    static QIcon stylesheetIcon(const QString &fileName);

    static QString toolBarDir();
    static QString toolBarFilePath(const QString &fileName);
    static QIcon toolBarIcon(const QString &fileName);

    static QIcon windowIcon();
};

#endif // RESOURCE_H
