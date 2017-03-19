#include "PluginInterface.h"

PluginInterface::PluginInterface()
{
}

PluginInterface::~PluginInterface()
{
}

QString PluginInterface::description() const {return QString();}

QList<PluginInterface::Author> PluginInterface::authors() const {return QList<Author>();}
