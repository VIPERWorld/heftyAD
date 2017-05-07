#include "ItemBrushForm.h"

ItemBrushForm::ItemBrushForm(QWidget *parent)
    : ItemForm(parent)
{
    addRow(&m_brush);
    formLayout()->setMargin(0);
}

void ItemBrushForm::retranslate()
{
    m_brush.setFormTitles(trUtf8("Couleur"), trUtf8("Gradient"), trUtf8("Pixmap"));
}
