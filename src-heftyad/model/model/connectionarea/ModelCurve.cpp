#include "ModelCurve.h"
#include "ModelNode.h"

#include "graphics/GraphicsAlgo.h"

ModelCurve::ModelCurve(const ModelNode &node1, const ModelNode &node2,
                       bool bidir,
                       QObject *parent)
    : ModelItem(parent),
      m_node1(const_cast<ModelNode&>(node1)),
      m_node2(const_cast<ModelNode&>(node2)),
      m_bidir(bidir || &m_node1==&m_node2),
      m_arrowSize(10),
      m_points_role(CurvePoint)
{
    initPointsPos();

    m_value = "0";

    m_pen = QPen(Qt::black, 1., Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    m_brush = Qt::NoBrush;
}

ModelNode& ModelCurve::node1() const {return m_node1;}
ModelNode& ModelCurve::node2() const {return m_node2;}
bool ModelCurve::isBidir() const {return m_bidir;}
bool ModelCurve::isLoop() const {return &m_node1 == &m_node2;}

qreal ModelCurve::arrowSize() const {return m_arrowSize;}
void ModelCurve::setArrowSize(int size, bool notify)
{
    if(m_arrowSize!=size && size>=10) {
        m_arrowSize = size;
        if(notify) {
            emit arrowSizeChanged();
        }
    }
}

bool ModelCurve::isValidPointIndex(int index) const
{
    return index>=0 && index<=3; // since there is only 4 points
}

QPointF ModelCurve::pointPos(int index) const {return isValidPointIndex(index) ? m_points_pos[index] : QPointF();}
void ModelCurve::setPointPos(int index, const QPointF &pos, bool notify)
{
    if(!isValidPointIndex(index)) {
        return;
    }

    if(m_points_pos[index] != pos) {
        m_points_pos[index] = pos;
        if(notify) {
            emit pointPosChanged(index);
        }
    }
}

ModelCurve::PointRole ModelCurve::pointRole() const {return m_points_role;}
void ModelCurve::setPointRole(int role, bool notify)
{
    bool validPointRole = false;
    switch(role) {
    case BezierCtrlPoint: case CurvePoint:
        validPointRole = true;
        break;
    }

    if(validPointRole && m_points_role!=role) {
        m_points_role = (PointRole) role;
        if(notify) {
            emit pointRoleChanged();
        }
    }
}

void ModelCurve::alignMidPoints()
{
    const QPointF &from(m_points_pos[0]);
    const QPointF &to  (m_points_pos[3]);

    setPointPos(1, GraphicsAlgo::getPointOnLine(from, to, 1./3));
    setPointPos(2, GraphicsAlgo::getPointOnLine(from, to, 2./3));
}

void ModelCurve::alignPoints()
{
    initEndPoints();  // set end points
    alignMidPoints(); // set mid points: we may have called initMidPoints to avoid the points from disapearing
                      //                 when node1 collides with node2
}

QPair<QPointF, QPointF> ModelCurve::controlPoints() const
{
    QPair<QPointF, QPointF> ctrlPts;
    const QPointF &from = m_points_pos[0];
    const QPointF &mid1 = m_points_pos[1];
    const QPointF &mid2 = m_points_pos[2];
    const QPointF &to   = m_points_pos[3];

    switch(m_points_role) {
    case BezierCtrlPoint:
        ctrlPts.first  = mid1;
        ctrlPts.second = mid2;
        break;

    case CurvePoint:
        ctrlPts = GraphicsAlgo::getControlPoints(from, mid1, mid2, to, 0.33, 0.67);
        break;
    }

    return ctrlPts;
}

QPainterPath ModelCurve::path() const
{
    const QPair<QPointF, QPointF> &ctrlPts(controlPoints());
    const QPointF &from = m_points_pos[0];
    const QPointF &to   = m_points_pos[3];

    QPainterPath path;
    path.moveTo(from);
    path.cubicTo(ctrlPts.first, ctrlPts.second, to);

    return path;
}

QRectF ModelCurve::minimalRect() const
{
    const QPointF &from = m_points_pos[0];
    const QPointF &mid1 = m_points_pos[1];
    const QPointF &mid2 = m_points_pos[2];
    const QPointF &to   = m_points_pos[3];

    QPolygonF poly;
    poly << from << mid1 << mid2 << to;
    return poly.boundingRect()
          .normalized();
}

qreal ModelCurve::scenePenWidth() const
{
    return ModelItem::scenePenWidth() + 0.8;
}

QRectF ModelCurve::rect() const
{
    /*
     * We return the bounding rectangle of this item' shape:
     *     since the normal value to return (see below) may be too large:
     *         when pointRole() is set to CurvePoint, the control points position may be too far from this item' shape.
     *         So collision detection (currently based on sceneBoundingRect()) becomes less relevant.
     *     since it's even better than minimalRect():
     *         for some layout of the points, sub-paths of the shape() are not clickable (thus item won't be selected).
     */
    return shape().boundingRect();

    // The normal value to return by this function is computed below

    const QPointF &from = m_points_pos[0];
    const QPointF &to   = m_points_pos[3];
    const auto &ctrlPts = controlPoints();

    QPolygonF poly;
    poly << from << ctrlPts.first << ctrlPts.second << to;
    return poly.boundingRect()
          .normalized();
}

qreal ModelCurve::extra() const
{
    return (scenePenWidth() + m_arrowSize) / 2.0;
}

QPainterPath ModelCurve::shape() const
{
    return QPainterPathStroker().createStroke(path()).simplified();
}

void ModelCurve::sendAttributeChanges()
{
    ModelItem::sendAttributeChanges();

    emit arrowSizeChanged();
    for(int i=0; i<=3; i++) {
        emit pointPosChanged(i);
    }
    emit pointRoleChanged();
}

void ModelCurve::initPointsPos()
{
    initEndPoints();
    initMidPoints();
}

void ModelCurve::initEndPoints()
{
    QPointF from, to;
    if(m_node1.intersects(m_node2)) {
        from = m_node1.pos()-QPointF(25, 0); // 25 is considered source vertex radius (but whatever positive value will be ok)
        to   = m_node2.pos()+QPointF(25, 0); // 25 is considered target vertex radius (but whatever positive value will be ok)
    }
    else {
        const QVariant &sDim = m_node1.dimension();
        const QVariant &tDim = m_node2.dimension();

        switch(m_node1.shapeKind()) {
        case ModelMultiShapeItem::Circle:
            from = GraphicsAlgo::getPointOnCircle   (m_node2.pos(), m_node1.pos(), sDim.toReal()/2);
            break;
        case ModelMultiShapeItem::Rectangle:
            from = GraphicsAlgo::getPointOnRectangle(m_node2.pos(), m_node1.pos(), sDim.toSizeF().width(), sDim.toSizeF().height());
            break;
        }

        switch(m_node2.shapeKind()) {
        case ModelMultiShapeItem::Circle:
            to   = GraphicsAlgo::getPointOnCircle   (m_node1.pos(), m_node2.pos(), tDim.toReal()/2);
            break;
        case ModelMultiShapeItem::Rectangle:
            to   = GraphicsAlgo::getPointOnRectangle(m_node1.pos(), m_node2.pos(), tDim.toSizeF().width(), tDim.toSizeF().height());
            break;
        }
    }

    setPointPos(0, from);
    setPointPos(3, to);
}

void ModelCurve::initMidPoints()
{
    const QPointF &from = m_points_pos[0];
    const QPointF &to   = m_points_pos[3];

    QPointF mid1 = GraphicsAlgo::getPointOnLine(from, to, 1./3);
    QPointF mid2 = GraphicsAlgo::getPointOnLine(from, to, 2./3);
    if(m_node1.intersects(m_node2)) {
        const qreal d = qMax(m_node1.dimension().toReal(), m_node1.dimension().toReal());
        mid1 -= QPointF(0, d);
        mid2 -= QPointF(0, d);
    }

    setPointPos(1, mid1);
    setPointPos(2, mid2);
}
