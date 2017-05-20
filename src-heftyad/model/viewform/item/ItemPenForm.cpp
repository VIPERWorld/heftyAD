#include "ItemPenForm.h"
#include "ModelItem.h"
#include "ViewItem.h"

#include "others/SignalBreaker.hpp"

#include <QSignalBlocker>

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

void ItemPenForm::clearFields()
{
    ItemForm::clearFields();

    m_pen.setPen(Qt::NoPen);
}

void ItemPenForm::registerItems()
{
    ItemForm::registerItems();

    if(m_items.size() == 1) {
        ModelItem *item = m_items.first()->modelItem();

        m_pen.setPen(item->pen());

        connect(item, &ModelItem::penChanged, this, &ItemPenForm::onItemPenChanged);
    }

    connect(&m_pen, &PenFormWidget::penChanged, this, &ItemPenForm::onThisPenChanged);
}

void ItemPenForm::unregisterItems()
{
    ItemForm::unregisterItems();

    if(m_items.size() == 1) {
        ModelItem *item = m_items.first()->modelItem();

        disconnect(item, &ModelItem::penChanged, this, &ItemPenForm::onItemPenChanged);
    }

    disconnect(&m_pen, &PenFormWidget::penChanged, this, &ItemPenForm::onThisPenChanged);
}

void ItemPenForm::onItemPenChanged()
{
    const QSignalBlocker blocker(m_pen); Q_UNUSED(blocker)

    auto *item = qobject_cast<ModelItem*>(sender());
    m_pen.setPen(item->pen());
}

void ItemPenForm::onThisPenChanged()
{
    for(ViewItem *viewItem : m_items) {
        ModelItem *item = viewItem->modelItem();
        const SignalBreaker b(item, &ModelItem::penChanged, this, &ItemPenForm::onItemPenChanged); Q_UNUSED(b)

        item->setPen(m_pen.pen());
    }
}
