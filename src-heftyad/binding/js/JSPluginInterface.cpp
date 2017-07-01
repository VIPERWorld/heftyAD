#include "JSAlgorithm.h"
#include "JSPluginInterface.h"

JSPluginInterface::JSPluginInterface(QObject *parent)
    : QObject(parent),
      AlgorithmPluginInterface()
{
}

QString JSPluginInterface::name() const {return m_parser.algorithmName();}
QString JSPluginInterface::description() const {return m_parser.algorithmDescription();}
QList<PluginInterface::Author> JSPluginInterface::authors() const
{
    QList<Author> retVal;

    for(const JSFileParser::Author &author : m_parser.algorithmAuthors()) {
        Author a;
        a.name = author.name;
        a.description = author.description;

        retVal.append(a);
    }

    return retVal;
}

Algorithm* JSPluginInterface::algorithmInstance() const
{
    return new JSAlgorithm(const_cast<JSFileParser&>(m_parser));
}

bool JSPluginInterface::load(const QString &filePath)
{
    return m_parser.loadFile(filePath);
}

void JSPluginInterface::reset()
{
    m_parser.reset();
}
