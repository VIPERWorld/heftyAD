#include "Resource.h"

#include <QApplication>
#include <QDir>

Resource::Resource()
    : Singleton<Resource>()
{
}

Resource::~Resource()
{
}

void Resource::makeUserDirs(void) const
{
    QDir().mkdir(userCommonDir());
    QDir().mkdir(userBackupDir());
    QDir().mkdir(userAppPluginDir());
    QDir().mkdir(userAppPropertyDir());
    QDir().mkdir(userAppTranslationDir());
    QDir().mkdir(userAppWorkingModelDir());
}

QString Resource::userCommonDir(void) const {return QApplication::applicationDirPath();}
QString Resource::userBackupDir(void) const {return userCommonDir()+"/heftyAD-backups";}
QString Resource::userAppPluginDir(void) const {return userCommonDir()+"/heftyAD-plugins";}
QString Resource::userAppPropertyDir(void) const {return userCommonDir()+"/heftyAD-properties";}
QString Resource::userAppTranslationDir(void) const {return userCommonDir()+"/heftyAD-translations";}
QString Resource::userAppWorkingModelDir(void) const {return userCommonDir()+"/heftyAD-workingmodel";}

void Resource::loadStyleSheets(QString fileName) const
{
    if(fileName.isEmpty()) {
        fileName = stylesheetFileName("style.qss");
    }

    QFile file(fileName);
    file.open(QFile::ReadOnly);
    qApp->setStyleSheet(file.readAll());
}

QString Resource::languageDir(void) const {return ":/rsrc/icons/languages";}
QString Resource::languageFileName(QString fileName) const {return languageDir()+"/"+fileName;}
QIcon Resource::languageIcon(QString fileName) const {return QIcon(languageFileName(fileName));}

QString Resource::simulationDir(void) const {return ":/rsrc/icons/simulation";}
QString Resource::simulationFileName(QString fileName) const {return simulationDir()+"/"+fileName;}
QIcon Resource::simulationIcon(QString fileName) const {return QIcon(simulationFileName(fileName));}

QString Resource::toolBarDir(void) const {return ":/rsrc/icons/toolbar";}
QString Resource::toolBarFileName(QString fileName) const {return toolBarDir()+"/"+fileName;}
QIcon Resource::toolBarIcon(QString fileName) const {return QIcon(toolBarFileName(fileName));}

QIcon Resource::windowIcon(void) const {return QIcon(":/rsrc/logo/heftyAD.png");}

QString Resource::shadowDir(void) const {return ":/rsrc/shadow";}
QString Resource::shadowFileName(QString fileName) const {return shadowDir()+"/"+fileName;}
QIcon Resource::shadowIcon(QString fileName) const {return QIcon(shadowFileName(fileName));}

QString Resource::stylesheetDir(void) const {return ":/rsrc/stylesheets";}
QString Resource::stylesheetFileName(QString fileName) const {return stylesheetDir()+"/"+fileName;}
QIcon Resource::stylesheetIcon(QString fileName) const {return QIcon(stylesheetFileName(fileName));}
