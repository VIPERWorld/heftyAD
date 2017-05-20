#include "ItemBrushForm.h"
#include "ModelItem.h"
#include "ViewItem.h"

#include "others/SignalBreaker.hpp"

#include <QSignalBlocker>

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

void ItemBrushForm::clearFields()
{
    ItemForm::clearFields();

    m_brush.setBrush(Qt::NoBrush);
}

void ItemBrushForm::registerItems()
{
    ItemForm::registerItems();

    if(m_items.size() == 1) {
        ModelItem *item = m_items.first()->modelItem();

        m_brush.setBrush(item->brush());

        connect(item, &ModelItem::brushChanged, this, &ItemBrushForm::onItemBrushChanged);
    }

    connect(&m_brush, &BrushFormWidget::brushChanged, this, &ItemBrushForm::onThisBrushChanged);
}

void ItemBrushForm::unregisterItems()
{
    ItemForm::unregisterItems();

    if(m_items.size() == 1) {
        ModelItem *item = m_items.first()->modelItem();

        disconnect(item, &ModelItem::brushChanged, this, &ItemBrushForm::onItemBrushChanged);
    }

    disconnect(&m_brush, &BrushFormWidget::brushChanged, this, &ItemBrushForm::onThisBrushChanged);
}

void ItemBrushForm::onItemBrushChanged()
{
    const QSignalBlocker blocker(m_brush); Q_UNUSED(blocker)

    auto *item = qobject_cast<ModelItem*>(sender());
    m_brush.setBrush(item->brush());
}

void ItemBrushForm::onThisBrushChanged()
{
    for(ViewItem *viewItem : m_items) {
        ModelItem *item = viewItem->modelItem();
        const SignalBreaker b(item, &ModelItem::brushChanged, this, &ItemBrushForm::onItemBrushChanged); Q_UNUSED(b)

        item->setBrush(m_brush.brush());
    }
}
