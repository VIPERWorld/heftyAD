#include "PluginLoader.h"

PluginLoader::PluginLoader()
{
    m_loadPluginsFromSubDirs = false;
}

QString PluginLoader::pluginDirPath(void) const {return m_pluginDirPath;}
void PluginLoader::setPluginDirPath(QString path) {m_pluginDirPath = path;}

void PluginLoader::setLoadPluginsFromSubDirs(bool value) {m_loadPluginsFromSubDirs = value;}

const QStringList& PluginLoader::pluginFilePaths(void) const {return m_pluginFilePaths;}
