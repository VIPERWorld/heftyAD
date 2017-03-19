#include "View.h"
#include "ViewForm.h"

#include "widget/SectionedWidgetElt.h"

ViewForm::ViewForm(View *view, QWidget *parent)
    : TabWidget(parent)
{
    addTab(&m_modelTab, "");
    addTab(&m_itemTab,  "");

    SectionedWidgetElt *sec = m_modelTab.addSection();
    sec->addWidget(&m_modelBgBrush, 1, 0);
    sec->setBarTitle("Brosse d'arrière-plan");

    if(view) {
        connect(view, &View::sceneSelectionChanged, this, &ViewForm::onSceneSelectionChanged);
    }
}

void ViewForm::retranslate()
{
    setTabText(0, trUtf8("Modèle"));
    setTabText(1, trUtf8("Items"));

    m_modelBgBrush.setFormTitles(trUtf8("Couleur"), trUtf8("Gradient"), trUtf8("Pixmap"));
}

void ViewForm::onSceneSelectionChanged() {}
