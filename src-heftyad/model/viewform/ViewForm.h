#ifndef VIEWFORM_H
#define VIEWFORM_H

#include "widget/SectionedWidget.h"
#include "widget/TabWidget.h"

#include "widget/dataform/BrushFormWidget.h"

class View;

class ViewForm : public TabWidget
{
    Q_OBJECT

protected:
    SectionedWidget m_modelTab;
        BrushFormWidget m_modelBgBrush;
    SectionedWidget m_itemTab;

public:
    explicit ViewForm(View *view, QWidget *parent = 0);

    virtual void retranslate();

protected:
    virtual void onSceneSelectionChanged();

signals:
};

#endif // VIEWFORM_H
