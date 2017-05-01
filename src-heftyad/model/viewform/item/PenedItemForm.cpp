#include "PenedItemForm.h"

PenedItemForm::PenedItemForm(QWidget *parent)
    : ItemForm(parent)
{
    addRow(&m_pen);
}

void PenedItemForm::retranslate()
{
    m_pen.setFieldTexts(trUtf8("Style"), trUtf8("Largeur"), trUtf8("Couleur"));
}
