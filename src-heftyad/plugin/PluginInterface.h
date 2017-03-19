#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QObject> // If not included, Q_DECLARE_INTERFACE will be unknown during compilation
#include <QString>
#include <QStringList>

/**
 * First of all
 * ============
 *
 * This interface and its subclasses are part of the heftyAD project.
 * They'll help you create plugins (algorithms, ...) for heftyAD.
 *
 * Since the application is under constant development, please before writing any plugin,
 * make sure the version of this file you've got is up to date. You can download
 * the latest version of this file from the project git repository.
 *
 * About this class
 * ================
 *
 * The PluginInterface class is the base class for all heftyAD plugins.
 */

class PluginInterface
{
public:
    typedef struct {
        QString name;
        QString surname;
        QString description; // a (preferably short) description about this author.
    } Author;

public:
    PluginInterface();
    virtual ~PluginInterface();

    /**
     * Returns the name of this plugin.
     */
    virtual QString name() const = 0;
    /**
     * Returns a string describing this plugin.
     */
    virtual QString description() const;

    /**
     * Returns this plugin authors.
     */
    virtual QList<Author> authors() const;
};

Q_DECLARE_INTERFACE(PluginInterface, "org.heftyAD.plugins.PluginInterface")

#endif // PLUGININTERFACE_H
