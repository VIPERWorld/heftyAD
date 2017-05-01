#ifndef VIEWFORM_H
#define VIEWFORM_H

#include "BasicItemForm.h"
#include "BrushedItemForm.h"
#include "CurvedItemForm.h"
#include "FontedItemForm.h"
#include "PenedItemForm.h"
#include "ShapedItemForm.h"

#include "widget/SectionedWidget.h"
#include "widget/TabWidget.h"

#include "widget/dataform/BrushFormWidget.h"

class View;

class ViewForm : public TabWidget
{
    Q_OBJECT

protected:
    SectionedWidget m_modelSections;
        SectionedWidgetElt *m_bgBrushSection; // background brush
            BrushFormWidget m_bgBrushWidget;
    SectionedWidget m_itemSections;
        SectionedWidgetElt *m_basicAttrs;
            BasicItemForm m_basicForm;
        SectionedWidgetElt *m_brushAttrs;
            BrushedItemForm m_brushForm;
        SectionedWidgetElt *m_penAttrs;
            PenedItemForm m_penForm;
        SectionedWidgetElt *m_fontAttrs;
            FontedItemForm m_fontForm;
        SectionedWidgetElt *m_shapeAttrs;
            ShapedItemForm m_shapeForm;
        SectionedWidgetElt *m_curveAttrs;
            CurvedItemForm m_curveForm;

public:
    explicit ViewForm(View *view, QWidget *parent = 0);

    virtual void retranslate();

protected:
    virtual void onSceneSelectionChanged();

private:
    void addSection(SectionedWidget &sectionWidget, SectionedWidgetElt **section, QWidget *widget);
    void retranslateWidgets(SectionedWidgetElt *section, const QString &sectionTitle, ItemForm &form);

signals:
};

#endif // VIEWFORM_H
