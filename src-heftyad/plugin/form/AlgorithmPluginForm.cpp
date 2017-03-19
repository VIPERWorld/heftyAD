#include "AlgorithmPluginForm.h"
#include "AlgorithmPluginInterface.h"

AlgorithmPluginForm::AlgorithmPluginForm(QWidget *parent)
    : TabWidget(parent)
{
    addTab(&m_infos, "");
    addTab(&m_options, "");
}

void AlgorithmPluginForm::fillIn(AlgorithmPluginInterface *plugin)
{
    m_infos.fillIn(plugin);

    // ...
}

void AlgorithmPluginForm::retranslate()
{
    setTabText(0, trUtf8("Infos"));
    setTabText(1, trUtf8("Options"));

    m_infos.retranslate();
}
