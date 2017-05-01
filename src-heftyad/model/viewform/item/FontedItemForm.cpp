#include "FontedItemForm.h"

FontedItemForm::FontedItemForm(QWidget *parent)
    : ItemForm(parent)
{
    addRow(&m_font);
}

void FontedItemForm::retranslate()
{
    m_font.setFieldTexts1(trUtf8("Famille"), trUtf8("Taille"));
    m_font.setFieldTexts2(trUtf8("Gras"), trUtf8("Italique"), trUtf8("Rayé"), trUtf8("Souligné"));
}
