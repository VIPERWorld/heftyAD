#include "PluginForm.h"
#include "PluginInterface.h"

PluginForm::PluginForm(QWidget *parent)
    : FormWidget(parent)
{
    m_name.setReadOnly(true);
    m_description.setReadOnly(true);
    m_authors.setReadOnly(true);

    m_name.setContextMenuPolicy(Qt::NoContextMenu);
    m_description.setContextMenuPolicy(Qt::NoContextMenu);
    m_authors.setContextMenuPolicy(Qt::NoContextMenu);

    // No need to add rows here -> see retranslate
}

void PluginForm::fillIn(PluginInterface *plugin)
{
    m_name.setText(plugin->name());
    //
    m_description.setText(plugin->description());
    //
    m_authors.clear();
    for(const PluginInterface::Author &author : plugin->authors()) {
        m_authors.append(author.name + " " + author.surname + "{" + author.description + "}");
        m_authors.append("");
    }
}

void PluginForm::retranslate()
{
    // First clear the layout

    clear();

    // Now add row using the right text

    addRow(trUtf8("Nom :"),         &m_name);
    addRow(trUtf8("Description :"), &m_description);
    addRow(trUtf8("Auteurs :"),     &m_authors);
}
