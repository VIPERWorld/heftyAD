#include "GraphModelVertex.h"
#include "ModelCurve.h"
#include "ViewCurvePoint.h"

#include "graphics/GraphicsAlgo.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

static const double Pi = 3.14159265358979323846264338327950288419717;

ViewCurvePoint::ViewCurvePoint(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    m_curve = nullptr;
    m_curve_index = -1;
    m_node = nullptr;

    m_alignmentPoint = nullptr;

    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);

    const double length = 10;
    setRect(-length/2.0, -length/2.0, length, length);
}

void ViewCurvePoint::setCurve(ModelCurve *curve, int index)
{
    m_curve = curve;
    m_curve_index = m_curve ? index : -1;
}

void ViewCurvePoint::setAlignmentPoint(ViewCurvePoint *point) {m_alignmentPoint = point;}

void ViewCurvePoint::attach(ModelNode *node)
{
    if(m_node == node) {
        return;
    }

    if(m_node != nullptr) {
        // QObject::disconnect
    }

    m_node = node;
    if(m_node != nullptr) {
        m_nodeLastPos = m_node->pos();
        // QObject::connect
    }
}

void ViewCurvePoint::tieToNode()
{
    if(m_node == nullptr) {
        return;
    }

    if(m_nodeLastPos != m_node->pos()) { // the vertex has moved
        QPointF p = m_node->pos() - m_nodeLastPos;
        /*
         * This should be done before setting the position.
         * If not, the next time we may still have lastPos = currentPos.
         * Indeed, when this item is moved, the edge is notified to adjust itself.
         */
        m_nodeLastPos = m_node->pos();

        setPos(x()+p.x(), y()+p.y());
    }
}

QPointF ViewCurvePoint::getPosOnNodeBorderFrom(const QPointF &pos) const
{
    if(!m_node) {
        return pos;
    }

    const QPointF &A = pos;
    const QPointF &B = m_node->pos();
    QPointF I; // the intersection point to be returned

    const QVariant &dim = m_node->dimension();
    switch(m_node->shapeKind()) {
    case ModelMultiShapeItem::Circle:
        I = GraphicsAlgo::getPointOnCircle(A, B, dim.toReal()/2);
        break;

    case ModelMultiShapeItem::Rectangle:
        I = GraphicsAlgo::getPointOnRectangle(A, B, dim.toSizeF().width(), dim.toSizeF().height());
        break;
    }

    return I;
}

void ViewCurvePoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    setBrush(isSelected() ? Qt::gray : Qt::white);
    QGraphicsRectItem::paint(painter, option, widget);
}

QVariant ViewCurvePoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch(change) {
    case ItemPositionChange:
        if(m_curve) {
            QPointF newPos = getPosOnNodeBorderFrom(value.toPointF());
            m_curve->setPointPos(m_curve_index, newPos, false);
            emit m_curve->needAdjustment();
            return newPos;
        }
        break;

    case ItemSelectedChange: // we need to update the scene (not necessarily to adjust the edge)
        if(m_curve) {
            emit m_curve->needAdjustment();
//            m_curve->setSelected(value.toBool());
        }
        break;
    default:
        break;
    }

    return QGraphicsRectItem::itemChange(change, value);
}

void ViewCurvePoint::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mouseDoubleClickEvent(event);

    if(m_node) {
        const auto &edgePoint = m_alignmentPoint ? *m_alignmentPoint : *this;
        const QPointF &I = getPosOnNodeBorderFrom(edgePoint.pos());
        setPos(I);
    }
    else {
        if(m_curve) {
            if(event->modifiers() & Qt::ControlModifier) {
                m_curve->alignPoints();
            }
            else {
                m_curve->alignMidPoints();
            }
        }
    }
}
