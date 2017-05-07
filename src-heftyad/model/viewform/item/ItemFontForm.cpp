#include "ItemFontForm.h"

ItemFontForm::ItemFontForm(QWidget *parent)
    : ItemForm(parent)
{
    addRow(&m_font);
    formLayout()->setMargin(0);
}

void ItemFontForm::retranslate()
{
    m_font.setFieldTexts1(trUtf8("Famille"), trUtf8("Taille"));
    m_font.setFieldTexts2(trUtf8("Gras"), trUtf8("Italique"));
    m_font.setFieldTexts3(trUtf8("Rayé"), trUtf8("Souligné"), trUtf8("Surligné"));
}
