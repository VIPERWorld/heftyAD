#include "ItemForm.h"

ItemForm::ItemForm(QWidget *parent)
    : FormWidget(parent)
{
}

void ItemForm::setItems(const QList<ViewItem*> &items)
{
    unregisterItems();
    m_items = items;
    registerItems();
}

void ItemForm::retranslate() {}

void ItemForm::clearFields()
{
    setEnabled(!m_items.isEmpty());
}

void ItemForm::registerItems()
{
    clearFields();
}
void ItemForm::unregisterItems() {}
