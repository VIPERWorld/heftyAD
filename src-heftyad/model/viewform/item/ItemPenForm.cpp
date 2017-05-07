#include "ItemPenForm.h"

ItemPenForm::ItemPenForm(QWidget *parent)
    : ItemForm(parent)
{
    addRow(&m_pen);
    formLayout()->setMargin(0);
}

void ItemPenForm::retranslate()
{
    m_pen.setFieldTexts(trUtf8("Style"), trUtf8("Largeur"), trUtf8("Couleur"));
}
