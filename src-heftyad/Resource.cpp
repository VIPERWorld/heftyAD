#include "Resource.h"

#include <QApplication>
#include <QDir>

void Resource::makeUserDirs()
{
    QDir().mkdir(userCommonDir());
    QDir().mkdir(userBackupDir());
    QDir().mkdir(userAppPluginDir());
    QDir().mkdir(userAppPropertyDir());
    QDir().mkdir(userAppTranslationDir());
    QDir().mkdir(userAppWorkingModelDir());
}

void Resource::loadStyleSheets(const QString &fileName)
{
    const QString &filePath = Resource::stylesheetFilePath(fileName);
    qApp->setStyleSheet(readTextFile(filePath));
}

QString Resource::readTextFile(const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QFile::ReadOnly)) {
        return file.readAll();
    }

    return "";
}

QString Resource::userCommonDir() {return QApplication::applicationDirPath();}
QString Resource::userBackupDir() {return userCommonDir()+"/heftyAD-backups";}
QString Resource::userAppPluginDir() {return userCommonDir()+"/heftyAD-plugins";}
QString Resource::userAppPropertyDir() {return userCommonDir()+"/heftyAD-properties";}
QString Resource::userAppTranslationDir() {return userCommonDir()+"/heftyAD-translations";}
QString Resource::userAppWorkingModelDir() {return userCommonDir()+"/heftyAD-workingmodel";}

QString Resource::languageDir() {return ":/rsrc/icons/languages";}
QString Resource::languageFilePath(const QString &fileName) {return languageDir()+"/"+fileName;}
QIcon Resource::languageIcon(const QString &fileName) {return QIcon(languageFilePath(fileName));}

QString Resource::shadowDir() {return ":/rsrc/shadow";}
QString Resource::shadowFilePath(const QString &fileName) {return shadowDir()+"/"+fileName;}
QIcon Resource::shadowIcon(const QString &fileName) {return QIcon(shadowFilePath(fileName));}

QString Resource::simulationDir() {return ":/rsrc/icons/simulation";}
QString Resource::simulationFilePath(const QString &fileName) {return simulationDir()+"/"+fileName;}
QIcon Resource::simulationIcon(const QString &fileName) {return QIcon(simulationFilePath(fileName));}

QString Resource::stylesheetDir() {return ":/rsrc/stylesheets";}
QString Resource::stylesheetFilePath(const QString &fileName) {return stylesheetDir()+"/"+fileName;}
QIcon Resource::stylesheetIcon(const QString &fileName) {return QIcon(stylesheetFilePath(fileName));}

QString Resource::toolBarDir() {return ":/rsrc/icons/toolbar";}
QString Resource::toolBarFilePath(const QString &fileName) {return toolBarDir()+"/"+fileName;}
QIcon Resource::toolBarIcon(const QString &fileName) {return QIcon(toolBarFilePath(fileName));}

QIcon Resource::windowIcon() {return QIcon(":/rsrc/logo/heftyAD.png");}
