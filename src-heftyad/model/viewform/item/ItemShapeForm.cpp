#include "ItemShapeForm.h"
#include "ModelMultiShapeItem.h"
#include "ViewMultiShapeItem.h"

#include "others/SignalBreaker.hpp"

#include <QSignalBlocker>

ItemShapeForm::ItemShapeForm(QWidget *parent)
    : ItemForm(parent)
{
    // add widgets

    addRow(&m_shapeKindLabel, QWidgetList() << &m_shapeKind << &m_rounded, Qt::Vertical);
    addRow(&m_dim1Label, &m_dim1);
    addRow(&m_dim2Label, &m_dim2);

    // customize widgets

    const int M = 1e4;
    m_dim1.setRange(20, M); // minimum dimension is the same for shaped items
    m_dim2.setRange(20, M);

    m_shapeKind.addItem("Circle");
    m_shapeKind.addItem("Rectangle");

    // connect signals to slots

    connect(&m_shapeKind, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ItemShapeForm::updateDimensionWidgets);
}

void ItemShapeForm::retranslate()
{
    m_shapeKindLabel.setText(trUtf8("Forme"));
    m_rounded.setText(trUtf8("Arrondir la forme ?"));
    updateDimensionWidgets();
}

void ItemShapeForm::clearFields()
{
    ItemForm::clearFields();

    m_rounded.setChecked(false);
    m_dim1.clear();
    m_dim2.clear();
}

void ItemShapeForm::registerItems()
{
    // First make sure only shaped items are concerned

    for(ViewItem *viewItem : m_items) {
        if(!qgraphicsitem_cast<ViewMultiShapeItem*>(viewItem)) {
            m_items.clear();
            break;
        }
    }

    // Now register items

    ItemForm::registerItems();

    if(m_items.size() == 1) {
        auto *item = static_cast<ModelMultiShapeItem*>(m_items.first()->modelItem());

        m_shapeKind.setCurrentIndex(item->shapeKind());
        m_rounded.setChecked(item->isRounded());
        setDimensionFrom(item);

        connect(item, &ModelMultiShapeItem::shapeKindChanged, this, &ItemShapeForm::onItemShapeKindChanged);
        connect(item, &ModelMultiShapeItem::roundedChanged,   this, &ItemShapeForm::onItemRoundedChanged);
        connect(item, &ModelMultiShapeItem::dimensionChanged, this, &ItemShapeForm::onItemDimensionChanged);
    }

    connect(&m_shapeKind, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),       this, &ItemShapeForm::onThisShapeKindChanged);
    connect(&m_rounded,   &QCheckBox::released,                                                        this, &ItemShapeForm::onThisRoundedChanged);
    connect(&m_dim1,      static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemShapeForm::onThisDimensionChanged);
    connect(&m_dim2,      static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemShapeForm::onThisDimensionChanged);
}

void ItemShapeForm::unregisterItems()
{
    ItemForm::unregisterItems();

    if(m_items.size() == 1) {
        auto *item = static_cast<ModelMultiShapeItem*>(m_items.first()->modelItem());

        disconnect(item, &ModelMultiShapeItem::shapeKindChanged, this, &ItemShapeForm::onItemShapeKindChanged);
        disconnect(item, &ModelMultiShapeItem::roundedChanged,   this, &ItemShapeForm::onItemRoundedChanged);
        disconnect(item, &ModelMultiShapeItem::dimensionChanged, this, &ItemShapeForm::onItemDimensionChanged);
    }

    disconnect(&m_shapeKind, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),       this, &ItemShapeForm::onThisShapeKindChanged);
    disconnect(&m_rounded,   &QCheckBox::released,                                                        this, &ItemShapeForm::onThisRoundedChanged);
    disconnect(&m_dim1,      static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemShapeForm::onThisDimensionChanged);
    disconnect(&m_dim2,      static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &ItemShapeForm::onThisDimensionChanged);
}

void ItemShapeForm::updateDimensionWidgets()
{
    switch(m_shapeKind.currentIndex()) {
    case ModelMultiShapeItem::Circle:
        m_dim1Label.setText(trUtf8("Diamètre"));

        m_dim2Label.setVisible(false);
        m_dim2.setVisible(false);
        break;

    case ModelMultiShapeItem::Rectangle:
        m_dim1Label.setText(trUtf8("Largeur"));

        m_dim2Label.setVisible(true);
        m_dim2Label.setText(trUtf8("Hauteur"));
        m_dim2.setVisible(true);
        break;
    }
}

void ItemShapeForm::setDimensionFrom(ModelMultiShapeItem *item)
{
    const QVariant &dim = item->dimension();
    switch(item->shapeKind()) {
    case ModelMultiShapeItem::Circle:
        m_dim1.setValue(dim.toReal());
        break;

    case ModelMultiShapeItem::Rectangle:
        m_dim1.setValue(dim.toSizeF().width());
        m_dim2.setValue(dim.toSizeF().height());
        break;
    }
}

void ItemShapeForm::onItemShapeKindChanged()
{
    const QSignalBlocker blocker(m_shapeKind); Q_UNUSED(blocker)

    auto *item = qobject_cast<ModelMultiShapeItem*>(sender());
    m_shapeKind.setCurrentIndex(item->shapeKind());
}

void ItemShapeForm::onItemRoundedChanged()
{
    //const QSignalBlocker blocker(m_shapeKind); Q_UNUSED(blocker) // not necessary here

    auto *item = qobject_cast<ModelMultiShapeItem*>(sender());
    m_rounded.setChecked(item->isRounded());
}

void ItemShapeForm::onItemDimensionChanged()
{
    const QSignalBlocker blocker(m_shapeKind); Q_UNUSED(blocker)

    auto *item = qobject_cast<ModelMultiShapeItem*>(sender());
    setDimensionFrom(item);
}

void ItemShapeForm::onThisShapeKindChanged()
{
    for(ViewItem *viewItem : m_items) {
        auto *item = qobject_cast<ModelMultiShapeItem*>(viewItem->modelItem());
        const SignalBreaker b(item, &ModelMultiShapeItem::shapeKindChanged, this, &ItemShapeForm::onItemShapeKindChanged); Q_UNUSED(b)

        item->switchToShapeKind(m_shapeKind.currentIndex());
    }
}

void ItemShapeForm::onThisRoundedChanged()
{
    for(ViewItem *viewItem : m_items) {
        auto *item = qobject_cast<ModelMultiShapeItem*>(viewItem->modelItem());
        const SignalBreaker b(item, &ModelMultiShapeItem::roundedChanged, this, &ItemShapeForm::onItemRoundedChanged); Q_UNUSED(b)

        item->setRounded(m_rounded.isChecked());
    }
}

void ItemShapeForm::onThisDimensionChanged()
{
    for(ViewItem *viewItem : m_items) {
        auto *item = qobject_cast<ModelMultiShapeItem*>(viewItem->modelItem());
        const SignalBreaker b(item, &ModelMultiShapeItem::dimensionChanged, this, &ItemShapeForm::onItemDimensionChanged); Q_UNUSED(b)

        switch(item->shapeKind()) {
        case ModelMultiShapeItem::Circle:
            item->setDimension(m_dim1.value());
            break;

        case ModelMultiShapeItem::Rectangle:
                item->setDimension(QSizeF(m_dim1.value(), m_dim2.value()));
            break;
        }
    }
}
