#ifndef VIEWFORM_H
#define VIEWFORM_H

#include "ItemBasicForm.h"
#include "ItemBrushForm.h"
#include "ItemCurveForm.h"
#include "ItemFontForm.h"
#include "ItemPenForm.h"
#include "ItemShapeForm.h"

#include "widget/SectionWidgetGroup.h"
#include "widget/TabWidget.h"

#include "widget/dataform/BrushFormWidget.h"

class View;

class ViewForm : public TabWidget
{
    Q_OBJECT

protected:
    SectionWidgetGroup m_modelSections;
        SectionWidget *m_bgBrushSection; // background brush
            BrushFormWidget m_bgBrushWidget;
    SectionWidgetGroup m_itemSections;
        SectionWidget *m_basicAttrs;
            ItemBasicForm m_basicForm;
        SectionWidget *m_brushAttrs;
            ItemBrushForm m_brushForm;
        SectionWidget *m_penAttrs;
            ItemPenForm m_penForm;
        SectionWidget *m_fontAttrs;
            ItemFontForm m_fontForm;
        SectionWidget *m_shapeAttrs;
            ItemShapeForm m_shapeForm;
        SectionWidget *m_curveAttrs;
            ItemCurveForm m_curveForm;

public:
    explicit ViewForm(View *view, QWidget *parent = 0);

    virtual void retranslate();

protected:
    QList<ItemForm*> forms() const;
    virtual void onSceneSelectionChanged();

private:
    void addSection(SectionWidgetGroup &sectionWidget, SectionWidget **section, QWidget *widget);
    void retranslateWidgets(SectionWidget *section, const QString &sectionTitle, ItemForm &form);

signals:
};

#endif // VIEWFORM_H
