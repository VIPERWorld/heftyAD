#include "ModelItem.h"
#include "ViewItem.h"

#include <QEvent>
#include <QPainter>

ViewItem::ViewItem(QGraphicsItem *parent)
    : GraphicsEditableItem(parent),
      m_modelItem(nullptr)
{
    m_textEditorFixed = true;
    m_textEditor.installEventFilter(this); // see eventFilter() in this class

    setFlag(ItemSendsGeometryChanges);

    connect(this, &ViewItem::xChanged,    this, &ViewItem::onXChanged);
    connect(this, &ViewItem::yChanged,    this, &ViewItem::onYChanged);
    connect(this, &ViewItem::textChanged, this, &ViewItem::onTextChanged);

    connect(this, &ViewItem::modelItemChanged, this, &ViewItem::onModelItemChanged);

    connect(&m_textEditor, &GraphicsTextItem::xChanged, this, &ViewItem::onTextEditorPosChanged);
    connect(&m_textEditor, &GraphicsTextItem::yChanged, this, &ViewItem::onTextEditorPosChanged);
}

ViewItem::~ViewItem()
{
}

ModelItem* ViewItem::modelItem() const {return m_modelItem;}
void ViewItem::setModelItem(ModelItem *modelItem)
{
    if(m_modelItem==modelItem || !acceptModelItem(modelItem)) { // acceptModelItem() ensures that each instance is given a valid model
        return;
    }

    if(m_modelItem != nullptr) {
        disconnect(m_modelItem, 0, this, 0);
    }

    m_modelItem = modelItem;
    if(m_modelItem != nullptr) {
        connect(m_modelItem, &ModelItem::destroyed, this, &ViewItem::onModelItemDestroyed);

        connect(m_modelItem, &ModelItem::valueChanged, this, &ViewItem::onModelItemValueChanged);

        connect(m_modelItem, &ModelItem::enabledChanged,    this, &ViewItem::onModelItemEnabledChanged);
        connect(m_modelItem, &ModelItem::selectedChanged,   this, &ViewItem::onModelItemSelectedChanged);
        connect(m_modelItem, &ModelItem::opacityChanged,    this, &ViewItem::onModelItemOpacityChanged);
        connect(m_modelItem, &ModelItem::visibilityChanged, this, &ViewItem::onModelItemVisibilityChanged);

        connect(m_modelItem, &ModelItem::brushChanged, this, &ViewItem::onModelItemBrushChanged);
        connect(m_modelItem, &ModelItem::penChanged,   this, &ViewItem::onModelItemPenChanged);
        connect(m_modelItem, &ModelItem::fontChanged,  this, &ViewItem::onModelItemFontChanged);

        connect(m_modelItem, &ModelItem::posChanged,  this, &ViewItem::onModelItemPosChanged);

        connect(m_modelItem, &ModelItem::valueEditorPosChanged, this, &ViewItem::onModelItemValueEditorPosChanged);
    }

    emit modelItemChanged(); // subclasses can respond to this signal by overriding onModelItemChanged
}

QRectF ViewItem::rect() const
{
    // We use QRectF(...) to ensure the item is well-painted even if it has no associated model.
    return m_modelItem ? m_modelItem->rect() : QRectF(0, 0, 75, 75);
}

QRectF ViewItem::boundingRect() const
{
    return m_modelItem ? m_modelItem->boundingRect() : rect(); // parent' boundingRect() function is pure virtual so we use rect()
}

QPainterPath ViewItem::shape() const
{
    return m_modelItem ? m_modelItem->shape() : GraphicsEditableItem::shape();
}

bool ViewItem::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == &m_textEditor) {
        switch(event->type()) {
        case QEvent::GraphicsSceneContextMenu: return true;

        case QEvent::GraphicsSceneHoverEnter: emit editorHovered(true);  return true;
        case QEvent::GraphicsSceneHoverLeave: emit editorHovered(false); return true;

        default: break;
        }
    }

    return GraphicsItem::eventFilter(watched, event);
}

QVariant ViewItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(m_modelItem) {
        switch(change) {
            case ItemEnabledChange:  m_modelItem->setEnabled (value.toBool(), false); break;
            case ItemSelectedChange: m_modelItem->setSelected(value.toBool(), false); break;
            case ItemOpacityChange:  m_modelItem->setOpacity (value.toReal(), false); break;
            case ItemVisibleChange:  m_modelItem->setVisible (value.toBool(), false); break;

            case ItemPositionChange: // Already handled through the xChanged() and yChanged() signals.
                break;

            default: break;
        }
    }

    return GraphicsEditableItem::itemChange(change, value);
}

void ViewItem::preparePainting(QPainter *painter)
{
    if(m_modelItem) {
        painter->setBrush(m_modelItem->brush());
        painter->setFont(m_modelItem->font()); // need this to draw the value using the right font
        painter->setPen(m_modelItem->pen());
        painter->setOpacity(m_modelItem->opacity());

        QPen p = painter->pen();
        p.setWidthF(m_modelItem->scenePenWidth());
        painter->setPen(p);
    }

    m_textEditor.setFont(painter->font());
    m_textEditor.setDefaultTextColor(painter->pen().color());
}

void ViewItem::onModelItemChanged()
{
    if(m_modelItem) {
        m_modelItem->sendAttributeChanges(); // to make sure view registers the right values for model attributes.
    }
}

void ViewItem::onModelItemDestroyed()
{
    // Note: There is no reason to delete this view since it may be used with another model.
    setModelItem(nullptr);
    setText("I'm\nalone"); // just for fun
}

void ViewItem::onModelItemValueChanged()
{
    if(m_modelItem) {
        setText(m_modelItem->value());
        // Handle the case where text edition is enabled (this item is being edited).
        if(m_textEditor.isVisible()) {
            m_textEditor.setPlainText(text());
        }
    }
}

void ViewItem::onModelItemEnabledChanged()    { if(m_modelItem) {setEnabled(m_modelItem->isEnabled());   update();} }
void ViewItem::onModelItemSelectedChanged()   { if(m_modelItem) {setSelected(m_modelItem->isSelected()); update();} }
void ViewItem::onModelItemOpacityChanged()    { if(m_modelItem) {setOpacity(m_modelItem->opacity());     update();} }
void ViewItem::onModelItemVisibilityChanged() { if(m_modelItem) {setVisible(m_modelItem->isVisible());   update();} }

void ViewItem::onModelItemBrushChanged() { update(); }
void ViewItem::onModelItemPenChanged()   { update(); }
void ViewItem::onModelItemFontChanged()  { update(); }

void ViewItem::onModelItemPosChanged()
{
    if(m_modelItem) {
        const auto &p(m_modelItem->pos());
        setPos(!parentItem() ? p : parentItem()->mapFromScene(p));
    }
}

void ViewItem::onModelItemValueEditorPosChanged()
{
    if(m_modelItem) {
        m_textEditor.setPos(m_modelItem->valueEditorPos());
    }
}

void ViewItem::onXChanged()
{
    if(m_modelItem) {
        const auto &p(!parentItem() ? pos() : parentItem()->mapToScene(pos()));
        m_modelItem->setX(p.x(), false);
    }
}

void ViewItem::onYChanged()
{
    if(m_modelItem) {
        const auto &p(!parentItem() ? pos() : parentItem()->mapToScene(pos()));
        m_modelItem->setY(p.y(), false);
    }
}

void ViewItem::onTextChanged(QString text)
{
    if(m_modelItem) {
        m_modelItem->setValue(text, false);
    }
}

void ViewItem::onTextEditorPosChanged()
{
    if(m_modelItem) {
        m_modelItem->setValueEditorPos(m_textEditor.pos(), false);
    }
}
