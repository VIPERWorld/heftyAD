#include "View.h"
#include "ViewForm.h"

#include "widget/SectionWidget.h"

ViewForm::ViewForm(View *view, QWidget *parent)
    : TabWidget(parent)
{
    addTab(&m_modelSections, "");
    addTab(&m_itemSections,  "");

    addSection(m_modelSections, &m_bgBrushSection, &m_bgBrushWidget);

    addSection(m_itemSections, &m_basicAttrs, &m_basicForm);
    addSection(m_itemSections, &m_brushAttrs, &m_brushForm);
    addSection(m_itemSections, &m_penAttrs,   &m_penForm);
    addSection(m_itemSections, &m_fontAttrs,  &m_fontForm);
    addSection(m_itemSections, &m_shapeAttrs, &m_shapeForm);
    addSection(m_itemSections, &m_curveAttrs, &m_curveForm);

    for(auto *f : forms()) {
        f->clearFields();
    }

    if(view) {
        m_bgBrushWidget.setBrush(view->backgroundBrush());
        connect(&m_bgBrushWidget, &BrushFormWidget::brushEdited, [this, view]() {
            view->setModelBackgroundBrush(m_bgBrushWidget.brush());
        });

        connect(view, &View::sceneSelectionChanged, this, &ViewForm::onSceneSelectionChanged);
    }
}

void ViewForm::retranslate()
{
    setTabText(0, trUtf8("Modèle"));
    setTabText(1, trUtf8("Items"));

    m_bgBrushSection->setBarTitle(trUtf8("Brosse d'arrière-plan"));
    m_bgBrushWidget.setFormTitles(trUtf8("Couleur"), trUtf8("Gradient"), trUtf8("Pixmap"));

    retranslateWidgets(m_basicAttrs, trUtf8("Attributs de base"),   m_basicForm);
    retranslateWidgets(m_brushAttrs, trUtf8("Brosse"),              m_brushForm);
    retranslateWidgets(m_penAttrs,   trUtf8("Stylo"),               m_penForm);
    retranslateWidgets(m_fontAttrs,  trUtf8("Police de caractère"), m_fontForm);
    retranslateWidgets(m_shapeAttrs, trUtf8("Forme"),               m_shapeForm);
    retranslateWidgets(m_curveAttrs, trUtf8("Arc"),                 m_curveForm);
}

QList<ItemForm*> ViewForm::forms() const
{
    return QList<ItemForm*>() << const_cast<ItemBasicForm*>(&m_basicForm)
                              << const_cast<ItemBrushForm*>(&m_brushForm)
                              << const_cast<ItemPenForm*>(&m_penForm)
                              << const_cast<ItemFontForm*>(&m_fontForm)
                              << const_cast<ItemShapeForm*>(&m_shapeForm)
                              << const_cast<ItemCurveForm*>(&m_curveForm);
}

void ViewForm::onSceneSelectionChanged() {}

void ViewForm::addSection(SectionWidgetGroup &sectionWidget, SectionWidget **section, QWidget *widget)
{
    *section = sectionWidget.addSection();
    SectionWidget *s = *section;
    s->addWidget(widget, 1, 0);
}

void ViewForm::retranslateWidgets(SectionWidget *section, const QString &sectionTitle, ItemForm &form)
{
    section->setBarTitle(sectionTitle);
    form.retranslate();
}
