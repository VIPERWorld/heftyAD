#ifndef JSPLUGININTERFACE_H
#define JSPLUGININTERFACE_H

#include "AlgorithmPluginInterface.h"
#include "JSFileParser.h"

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
    JSFileParser m_parser;

public:
    explicit JSPluginInterface(QObject *parent = 0);

    QString name() const override;
    QString description() const override;
    QList<Author> authors() const override;

    Algorithm* algorithmInstance() const override;

    bool load(const QString &filePath);
    void reset();

signals:

public slots:
};

#endif // JSPLUGININTERFACE_H
