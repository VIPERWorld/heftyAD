#include "BrushedItemForm.h"

BrushedItemForm::BrushedItemForm(QWidget *parent)
    : ItemForm(parent)
{
    addRow(&m_brush);
}

void BrushedItemForm::retranslate()
{
    m_brush.setFormTitles(trUtf8("Couleur"), trUtf8("Gradient"), trUtf8("Pixmap"));
}
