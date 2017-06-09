#ifndef JSPLUGININTERFACE_H
#define JSPLUGININTERFACE_H

#include "AlgorithmPluginInterface.h"
#include "JSAlgorithmParser.h"

#include <QObject>

/**
 * The JSPluginInterface class can not be loaded by QPluginLoader.
 * It's not a Qt plugin. It just helps to load Javascript algorithms.
 */
class JSPluginInterface : public QObject, public AlgorithmPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID AlgorithmPluginInterface_ID)
    Q_INTERFACES(PluginInterface AlgorithmPluginInterface)

private:
    JSAlgorithmParser m_parser;

public:
    explicit JSPluginInterface(QObject *parent = 0);

    bool load(const QString &filePath);

    QString name() const override;
    QString description() const override;
    QList<Author> authors() const override;

    Algorithm* algorithmInstance() const override;

signals:

public slots:
};

#endif // JSPLUGININTERFACE_H
