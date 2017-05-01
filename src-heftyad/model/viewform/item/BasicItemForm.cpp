#include "BasicItemForm.h"

BasicItemForm::BasicItemForm(QWidget *parent)
    : ItemForm(parent)
{
}

void BasicItemForm::retranslate()
{
    ItemForm::retranslate();

    addRow(trUtf8("Valeur"),   &m_value);
    addRow(trUtf8("Opacité"),  &m_opacity);
    addRow(trUtf8("Position"), QWidgetList() << &m_posX << &m_posY, Qt::Horizontal);
}
