#include "ItemFontForm.h"
#include "ModelItem.h"
#include "ViewItem.h"

#include "others/SignalBreaker.hpp"

#include <QSignalBlocker>

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

void ItemFontForm::clearFields()
{
    ItemForm::clearFields();

    m_font.setFontz(QFont());
}

void ItemFontForm::registerItems()
{
    ItemForm::registerItems();

    if(m_items.size() == 1) {
        ModelItem *item = m_items.first()->modelItem();

        m_font.setFontz(item->font());

        connect(item, &ModelItem::fontChanged, this, &ItemFontForm::onItemFontChanged);
    }

    connect(&m_font, &FontFormWidget::fontChanged, this, &ItemFontForm::onThisFontChanged);
}

void ItemFontForm::unregisterItems()
{
    ItemForm::unregisterItems();

    if(m_items.size() == 1) {
        ModelItem *item = m_items.first()->modelItem();

        disconnect(item, &ModelItem::fontChanged, this, &ItemFontForm::onItemFontChanged);
    }

    disconnect(&m_font, &FontFormWidget::fontChanged, this, &ItemFontForm::onThisFontChanged);
}

void ItemFontForm::onItemFontChanged()
{
    const QSignalBlocker blocker(m_font); Q_UNUSED(blocker)

    auto *item = qobject_cast<ModelItem*>(sender());
    m_font.setFontz(item->font());
}

void ItemFontForm::onThisFontChanged()
{
    for(ViewItem *viewItem : m_items) {
        ModelItem *item = viewItem->modelItem();
        const SignalBreaker b(item, &ModelItem::fontChanged, this, &ItemFontForm::onItemFontChanged); Q_UNUSED(b)

        item->setFont(m_font.fontz());
    }
}
