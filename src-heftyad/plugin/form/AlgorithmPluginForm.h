#ifndef ALGORITHMPLUGINFORM_H
#define ALGORITHMPLUGINFORM_H

#include "PluginForm.h"

#include "widget/GridWidget.h"
#include "widget/TabWidget.h"

class AlgorithmPluginInterface;

class AlgorithmPluginForm : public TabWidget
{
    Q_OBJECT

protected:
    PluginForm m_infos;
    GridWidget m_options;

public:
    explicit AlgorithmPluginForm(QWidget *parent = 0);

    void fillIn(AlgorithmPluginInterface *plugin);
    void retranslate();

signals:

public slots:
};

#endif // ALGORITHMPLUGINFORM_H
