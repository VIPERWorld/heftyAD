#ifndef PLUGINFORM_H
#define PLUGINFORM_H

#include "widget/FormWidget.h"

#include <QLineEdit>
#include <QTextEdit>

class PluginInterface;

class PluginForm : public FormWidget
{
    Q_OBJECT

protected:
    QLineEdit m_name;
    QTextEdit m_description;
    QTextEdit m_authors; // will use a better structure (combobox + textEdit) later

public:
    explicit PluginForm(QWidget *parent = 0);

    /**
     * Fill in this form using the given PluginInterface data.
     */
    void fillIn(PluginInterface *plugin);
    void retranslate();

signals:

public slots:
};

#endif // PLUGINFORM_H
