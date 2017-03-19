#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QDirIterator>
#include <QPluginLoader>

class PluginLoader
{
protected:
    QString m_pluginDirPath;
    bool m_loadPluginsFromSubDirs;

    QStringList m_pluginFilePaths;

public:
    PluginLoader();

    QString pluginDirPath(void) const;
    void setPluginDirPath(QString path);

    void setLoadPluginsFromSubDirs(bool value);

    const QStringList& pluginFilePaths(void) const;

    /**
     * Tells whether the given fileName matches with a plugin fileName.
     */
    template <typename T>
    bool isPlugin(QString filePath) const
    {
        bool isAPlugin = false;

        QPluginLoader loader(filePath);
        QObject *plugin = loader.instance();
        if(plugin) {
            T pluginCasted = qobject_cast<T>(plugin);
            if(pluginCasted) {
                isAPlugin = true;
            }
            loader.unload();
        }

        return isAPlugin;
    }

    /**
     * T should be a pointer ==> ? (to be edited).
     *
     * If the plugin dir path is empty, this method (which uses QDirIterator) seems
     * to search for plugins in a specific directory (e.g. "D:" for the Windows OS).
     *
     * Note: This method can have a very long running time (if there are several
     * directories and/or files to analyze). ==> So use Thread later.
     *
     * To be completed.
     */
    template <typename T>
    void findPlugins(void)
    {
        // First clear plugin list

        m_pluginFilePaths.clear();

        // Then gather plugins

        QDirIterator::IteratorFlags flags = QDirIterator::NoIteratorFlags;
        if(m_loadPluginsFromSubDirs) {
            flags = QDirIterator::Subdirectories;
        }

        QDirIterator it(m_pluginDirPath, QStringList() << "*.so" << "*.dll", QDir::Files, flags);
        while(it.hasNext()) {
            const auto filePath = it.next();
            if(isPlugin<T>(filePath)) {
                m_pluginFilePaths << filePath;
            }
        }
    }
};

#endif // PLUGINLOADER_H
