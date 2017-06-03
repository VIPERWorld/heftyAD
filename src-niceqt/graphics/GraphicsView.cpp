#include "GraphicsEditableItem.h"
#include "GraphicsView.h"

#include <QGraphicsItem>
#include <QMouseEvent>

#include <math.h>

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent),
      m_zoomInScalFactor(1.2),
      m_wheelDirection(WheelDir1)
{
}

GraphicsView::~GraphicsView()
{
}

qreal GraphicsView::zoomInScaleFactor() const {return m_zoomInScalFactor;}
void GraphicsView::setZoomInScaleFactor(qreal scaleFactor) {m_zoomInScalFactor = scaleFactor;}

GraphicsView::WheelDirection GraphicsView::wheelDirection() const {return m_wheelDirection;}
void GraphicsView::setWheelDirection(GraphicsView::WheelDirection direction) {m_wheelDirection = direction;}

QGraphicsItem* GraphicsView::itemUnderMouseEvent(QMouseEvent *event) const
{
    return itemAt(event->pos());
}

QGraphicsItem* GraphicsView::itemUnderContextMenuEvent(QContextMenuEvent *event) const
{
    return itemAt(event->pos());
}

bool GraphicsView::scaleView(qreal scaleFactor)
{
    const auto factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if(factor < 0.07 || factor > 100) {
        return false;
    }

    scale(scaleFactor, scaleFactor);
    return true;
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    //if(event->modifiers() == Qt::ControlModifier) {
        const auto scaleFactor = pow(2., m_wheelDirection*event->delta()/240.0);
        scaleView(scaleFactor);
    //}
}

bool GraphicsView::saveImageTo(const QString &filePath, const char *format)
{
    QImage pixmap(width(), height(), QImage::Format_ARGB32_Premultiplied);

    QPainter p;
    p.begin(&pixmap);
    p.setRenderHint(QPainter::Antialiasing, true);
    render(&p);
    p.end();

    return pixmap.save(filePath, format);
}

void GraphicsView::setEnabledItemEdition(bool enabled)
{
    foreach(QGraphicsItem *item, items()) {
        auto *editableItem = dynamic_cast<GraphicsEditableItem*>(item);
        if(editableItem) {
            editableItem->setEditable(enabled);
        }
    }
}

void GraphicsView::setEnabledItemMove(bool enabled)
{
    foreach(QGraphicsItem *item, items()) {
        item->setFlag(QGraphicsItem::ItemIsMovable, enabled);
    }
}

void GraphicsView::selectAll()
{
    foreach(QGraphicsItem *item, items()) {
        item->setSelected(true);
    }
}

void GraphicsView::deselectAll()
{
    foreach(QGraphicsItem *item, items()) {
        item->setSelected(false);
    }
}

void GraphicsView::removeSelection() {}

void GraphicsView::cutSelection() {}
void GraphicsView::copySelection() {}
void GraphicsView::copySelectionToClipboard() {}
void GraphicsView::pasteCopiedSelection() {}

void GraphicsView::duplicateSelection() {}

bool GraphicsView::zoomIn()  {return scaleView(m_zoomInScalFactor);}
bool GraphicsView::zoomOut() {return scaleView(1/m_zoomInScalFactor);}
