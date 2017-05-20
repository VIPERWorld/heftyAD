#include "ItemBasicForm.h"
#include "ModelItem.h"
#include "ViewItem.h"

#include "others/SignalBreaker.hpp"

#include <QSignalBlocker>

ItemBasicForm::ItemBasicForm(QWidget *parent)
    : ItemForm(parent)
{
    // add widgets

    addRow(&m_valueLabel,   &m_value);
    addRow(&m_opacityLabel, &m_opacity);
    addRow(&m_posLabel,     QWidgetList() << &m_posX << &m_posY, Qt::Horizontal);
    addRow(&m_editorLabel,  QWidgetList() << &m_editorX << &m_editorY, Qt::Horizontal);

    // customize widgets

    m_value.setMaximumHeight(50);
    m_value.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QWidgetList widgets = QWidgetList() << &m_value << &m_opacity << &m_posX << &m_posY << &m_editorX << &m_editorY;
    for(auto *w : widgets) {
        w->setContextMenuPolicy(Qt::NoContextMenu);
    }

    m_opacity.setRange(0, 1);
    m_opacity.setSingleStep(0.01);

    const int M = 1e6;
    QList<QDoubleSpinBox*> spins = QList<QDoubleSpinBox*>() << &m_posX << &m_posY << &m_editorX << &m_editorY;
    for(auto *s : spins) {
        s->setRange(-M, M);
    }

    const QString X = "X = ";
    const QString Y = "Y = ";
    m_posX.setPrefix(X);
    m_posY.setPrefix(Y);
    m_editorX.setPrefix(X);
    m_editorY.setPrefix(Y);
}

void ItemBasicForm::setValueEditorPosEnabled(bool enabled)
{
    m_editorLabel.setEnabled(enabled);
    m_editorX.setEnabled(enabled);
    m_editorY.setEnabled(enabled);
}

void ItemBasicForm::retranslate()
{
    m_valueLabel.setText(trUtf8("Valeur"));
    m_opacityLabel.setText(trUtf8("Opacité"));
    m_posLabel.setText(trUtf8("Position"));
    m_editorLabel.setText(trUtf8("Editeur"));
}

void ItemBasicForm::clearFields()
{
    ItemForm::clearFields();

    m_value.clear();
    m_opacity.clear();
    m_posX.clear(); m_posY.clear();
    m_editorX.clear(); m_editorY.clear();
}

void ItemBasicForm::registerItems()
{
    ItemForm::registerItems();

    if(m_items.size() == 1) {
        ModelItem *item = m_items.first()->modelItem();

        m_value.setPlainText(item->value());
        m_opacity.setValue(item->opacity());
        m_posX.setValue(item->x()); m_posY.setValue(item->y());
        m_editorX.setValue(item->valueEditorPos().x()); m_editorY.setValue(item->valueEditorPos().y());

        connect(item, &ModelItem::valueChanged,          this, &ItemBasicForm::onItemValueChanged);
        connect(item, &ModelItem::opacityChanged,        this, &ItemBasicForm::onItemOpacityChanged);
        connect(item, &ModelItem::posChanged,            this, &ItemBasicForm::onItemPosChanged);
        connect(item, &ModelItem::valueEditorPosChanged, this, &ItemBasicForm::onItemValueEditorPosChanged);
    }

    connect(&m_value,   &QPlainTextEdit::textChanged,                                                this, &ItemBasicForm::onThisValueChanged);
    connect(&m_opacity, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemBasicForm::onThisOpacityChanged);
    connect(&m_posX,    static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemBasicForm::onThisPosChanged);
    connect(&m_posY,    static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemBasicForm::onThisPosChanged);
    connect(&m_editorX, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemBasicForm::onThisValueEditorPosChanged);
    connect(&m_editorY, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemBasicForm::onThisValueEditorPosChanged);
}

void ItemBasicForm::unregisterItems()
{
    ItemForm::unregisterItems();

    if(m_items.size() == 1) {
        ModelItem *item = m_items.first()->modelItem();

        disconnect(item, &ModelItem::valueChanged,          this, &ItemBasicForm::onItemValueChanged);
        disconnect(item, &ModelItem::opacityChanged,        this, &ItemBasicForm::onItemOpacityChanged);
        disconnect(item, &ModelItem::posChanged,            this, &ItemBasicForm::onItemPosChanged);
        disconnect(item, &ModelItem::valueEditorPosChanged, this, &ItemBasicForm::onItemValueEditorPosChanged);
    }

    disconnect(&m_value,   &QPlainTextEdit::textChanged,                                                this, &ItemBasicForm::onThisValueChanged);
    disconnect(&m_opacity, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemBasicForm::onThisOpacityChanged);
    disconnect(&m_posX,    static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemBasicForm::onThisPosChanged);
    disconnect(&m_posY,    static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemBasicForm::onThisPosChanged);
    disconnect(&m_editorX, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemBasicForm::onThisValueEditorPosChanged);
    disconnect(&m_editorY, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemBasicForm::onThisValueEditorPosChanged);
}

void ItemBasicForm::onItemValueChanged()
{
    const QSignalBlocker blocker(m_value); Q_UNUSED(blocker)

    auto *item = qobject_cast<ModelItem*>(sender());
    m_value.setPlainText(item->value());
}

void ItemBasicForm::onItemOpacityChanged()
{
    const QSignalBlocker blocker(m_opacity); Q_UNUSED(blocker)

    auto *item = qobject_cast<ModelItem*>(sender());
    m_opacity.setValue(item->opacity());
}

void ItemBasicForm::onItemPosChanged()
{
    const QSignalBlocker blocker1(m_posX); Q_UNUSED(blocker1)
    const QSignalBlocker blocker2(m_posY); Q_UNUSED(blocker2)

    auto *item = qobject_cast<ModelItem*>(sender());
    m_posX.setValue(item->x());
    m_posY.setValue(item->y());
}

void ItemBasicForm::onItemValueEditorPosChanged()
{
    const QSignalBlocker blocker1(m_editorX); Q_UNUSED(blocker1)
    const QSignalBlocker blocker2(m_editorY); Q_UNUSED(blocker2)

    auto *item = qobject_cast<ModelItem*>(sender());
    m_editorX.setValue(item->valueEditorPos().x());
    m_editorY.setValue(item->valueEditorPos().y());
}

void ItemBasicForm::onThisValueChanged()
{
    for(ViewItem *viewItem : m_items) {
        ModelItem *item = viewItem->modelItem();
        const SignalBreaker b(item, &ModelItem::valueChanged, this, &ItemBasicForm::onItemValueChanged); Q_UNUSED(b)

        viewItem->setText(m_value.toPlainText());
    }
}

void ItemBasicForm::onThisOpacityChanged()
{
    for(ViewItem *viewItem : m_items) {
        ModelItem *item = viewItem->modelItem();
        const SignalBreaker b(item, &ModelItem::opacityChanged, this, &ItemBasicForm::onItemOpacityChanged); Q_UNUSED(b)

        viewItem->setOpacity(m_opacity.value());
    }
}

void ItemBasicForm::onThisPosChanged()
{
    for(ViewItem *viewItem : m_items) {
        ModelItem *item = viewItem->modelItem();
        const SignalBreaker b(item, &ModelItem::posChanged, this, &ItemBasicForm::onItemPosChanged); Q_UNUSED(b)

        item->setPos(m_posX.value(), m_posY.value());
    }
}

void ItemBasicForm::onThisValueEditorPosChanged()
{
    for(ViewItem *viewItem : m_items) {
        ModelItem *item = viewItem->modelItem();
        const SignalBreaker b(item, &ModelItem::valueEditorPosChanged, this, &ItemBasicForm::onItemValueEditorPosChanged); Q_UNUSED(b)

        item->setValueEditorPos(m_editorX.value(), m_editorY.value());
    }
}
