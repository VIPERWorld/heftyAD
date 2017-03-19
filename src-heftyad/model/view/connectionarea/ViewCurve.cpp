#include "ModelNode.h"
#include "ModelCurve.h"
#include "ViewCurve.h"
#include "ViewCurvePoint.h"

#include "graphics/GraphicsAlgo.h"

#include <QGraphicsScene>
#include <QPainter>

#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

ViewCurve::ViewCurve(QGraphicsItem *parent)
    : ViewItem(parent)
{
    m_textEditorFixed = false;
    m_textEditor.setVisible(true); // to make sure editor text is changed when modelItem value changed
    m_textEditor.setFlags(ItemIsMovable|ItemIsSelectable); // To ensure rubber band selection applies to the editor too.

    createPoints();

    /*
     * Ensure this curve won't move with rubber band selection.
     * Note: ItemIsSelectable is still needed,
     *       since this curve is selected when text editor is hovered (see sgnals/slots connections below).
     */
    setFlag(ItemIsMovable, false);
    /*
     * We set acceptedMouseButtons to Qt::NoButton to avoid this edge from being selected (for instance).
     * Indeed, clicking in the area covered by this edge boundingRect() drives it to a Selected state.
     * I'v also tried overriding mousePressEvent(), checking whether shape() contains the mouse position, ...
     * But nothings works, unfortunately.
     */
    //setAcceptedMouseButtons(Qt::NoButton);

    connect(this, &ViewCurve::editorHovered, [this](bool hovered) {
        setSelected(hovered);
        update();
    });
}

void ViewCurve::adjust()
{
    // Make sure each point is tied to its associated vertex, as soon as vertex moves.
    m_from->tieToNode();
    m_to->tieToNode();

    scene()->update(); // This is normally not required, but due to QPainterPath (bezier curves) drawing,
    // we have to do it: if not, the scene remains dirty until an update is performed.
}

void ViewCurve::setPointsVisibility(bool visible)
{
    m_from->setVisible(visible);
    m_mid1->setVisible(visible);
    m_mid2->setVisible(visible);
    m_to->setVisible(visible);
}

void ViewCurve::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //painter->drawRect(boundingRect());

    // ensure the editor is visible even if text edition is disabled (via the model-workspace toolbar)
    if(!m_textEditor.isVisible()) {
        m_textEditor.setVisible(true);
    }

    preparePainting(painter);

    // First paint text editor

    ViewItem::paint(painter, option, widget);

    // Set up data do draw curve

    const bool pointsSelected = m_from->isSelected() || m_mid1->isSelected() || m_mid2->isSelected() || m_to->isSelected();

    QPair<QPointF, QPointF> ctrlPts;
    bool tieCtrlPoints = false;
    qreal arrowSize = 10.;
    bool arrowAtSource = false;

    auto *curve = static_cast<ModelCurve*>(m_modelItem);
    if(curve) {
        ctrlPts = curve->controlPoints();

        switch(curve->pointRole()) {
        case ModelCurve::BezierCtrlPoint:
            tieCtrlPoints = pointsSelected;
            m_mid1->setOpacity(pointsSelected ? 1. : 0.5);
            m_mid2->setOpacity(pointsSelected ? 1. : 0.5);
            break;

        default:
            break;
        }

        arrowSize = curve->arrowSize();
        arrowAtSource = curve->isBidir() && !curve->isLoop();
    }
    else {
        ctrlPts.first  = m_mid1->pos();
        ctrlPts.second = m_mid2->pos();
    }

    // Draw the curve

    const QPen &pen(painter->pen());
    const QBrush &brush(painter->brush());
    paintCubicBezierCurve(painter, m_from->pos(), ctrlPts.first, ctrlPts.second, m_to->pos(),
                          pen, brush,
                          tieCtrlPoints, QPen(), QBrush(),
                          arrowAtSource, pen.color(),
                          true, pen.color(),
                          arrowSize);
}

void ViewCurve::paintCubicBezierCurve(QPainter *painter,
                                      const QPointF &from, const QPointF &ctrlPt1, const QPointF &ctrlPt2, const QPointF &to,
                                      const QPen &curvePen, const QBrush &curveBrush,
                                      bool tieCtrlPoints, const QPen &tiePen, const QBrush &tieBrush,
                                      bool arrowAtFrom, const QBrush &fromBrush,
                                      bool arrowAtTo, const QBrush &toBrush,
                                      qreal arrowSize)
{
    // Draw curve

    QPainterPath path;
    path.moveTo(from);
    path.cubicTo(ctrlPt1, ctrlPt2, to);

    painter->setPen(curvePen);
    painter->setBrush(curveBrush);

    painter->drawPath(path);

    // Tie ctrl points

    if(tieCtrlPoints) {
        painter->setPen(tiePen);
        painter->setBrush(tieBrush);

        painter->drawLine(from, ctrlPt1);
        painter->drawLine(ctrlPt1, ctrlPt2);
        painter->drawLine(ctrlPt2, to);
    }

    // Draw arrows

    if(arrowAtFrom) {
        QLineF line(ctrlPt1, from);
        paintArrows(painter, line, false, QBrush(), true, fromBrush, arrowSize);
    }

    if(arrowAtTo) {
        QLineF line(ctrlPt2, to);
        paintArrows(painter, line, false, QBrush(), true, toBrush, arrowSize);
    }
}

/**
 * A slightly updated version of the Edge::paint function availabe at:
 *     https://doc.qt.io/qt-5/qtwidgets-graphicsview-elasticnodes-example.html
 */
void ViewCurve::paintArrows(QPainter *painter,
                            const QLineF &line,
                            bool atSource, const QBrush &sourceBrush,
                            bool atTarget, const QBrush &targetBrush,
                            qreal arrowSize)
{
    if(qFuzzyCompare(line.length(), qreal(0.))) { // Is the arrow length approximately 0?
        return;
    }

    // We proceed to drawing one arrow at each end of the given line.
    // The coordinates for the arrow are determined using simple trigonometry.

    qreal angle = ::acos(line.dx() / line.length());
    if(line.dy() >= 0) {
        angle = TwoPi - angle;
    }

    QPointF sourceArrowP1 = line.p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                cos(angle + Pi / 3) * arrowSize);
    QPointF sourceArrowP2 = line.p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1   = line.p2() + QPointF(sin(angle - Pi / 3) * arrowSize,
                                                cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2   = line.p2() + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                                cos(angle - Pi + Pi / 3) * arrowSize);

    QPolygonF sourceArrow = QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2;
    QPolygonF destArrow   = QPolygonF() << line.p2() << destArrowP1   << destArrowP2;

    if(atSource) {
        painter->setBrush(sourceBrush);
        painter->drawPolygon(sourceArrow); // Draw arrow at source
    }

    if(atTarget) {
        painter->setBrush(targetBrush);
        painter->drawPolygon(destArrow); // Draw arrow at dest
    }
}

QVariant ViewCurve::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionChange) {
        if(m_modelItem) {
            m_modelItem->setPos(pos(), false);
        }
        return pos(); // the position won't change
    }

    return ViewItem::itemChange(change, value);
}

void ViewCurve::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event) // ignore mouse move event (just to make sure undo/redo is not registered)
}

void ViewCurve::createPoints()
{
    m_from = new ViewCurvePoint;
    m_mid1  = new ViewCurvePoint;
    m_mid2 = new ViewCurvePoint;
    m_to   = new ViewCurvePoint;

    m_from->setAlignmentPoint(m_mid1);
    m_to->setAlignmentPoint(m_mid2);
}

void ViewCurve::setPointsCurve(ModelCurve *curve)
{
    m_from->setCurve(curve, 0);
    m_mid1->setCurve(curve, 1);
    m_mid2->setCurve(curve, 2);
    m_to->setCurve  (curve, 3);

    if(!scene()) {
        return;
    }

    if(curve) {
        const ModelNode &source(curve->node1());
        const ModelNode &dest  (curve->node2());

        scene()->addItem(m_from);
        m_from->attach(const_cast<ModelNode*>(&source));

        scene()->addItem(m_to);
        m_to->attach(const_cast<ModelNode*>(&dest));

        scene()->addItem(m_mid1);

        scene()->addItem(m_mid2);
    }
    else {
        scene()->removeItem(m_from);
        scene()->removeItem(m_mid1);
        scene()->removeItem(m_mid2);
        scene()->removeItem(m_to);
    }
}

void ViewCurve::onModelItemChanged()
{
    auto *curve = static_cast<ModelCurve*>(m_modelItem);
    if(curve) {
        connect(curve, &ModelCurve::needAdjustment,   this, &ViewCurve::onAdjustementNeeded);

        connect(curve, &ModelCurve::arrowSizeChanged, this, &ViewCurve::onArrowSizeChanged);
        connect(curve, &ModelCurve::pointPosChanged,  this, &ViewCurve::onPointPosChanged);
        connect(curve, &ModelCurve::pointRoleChanged, this, &ViewCurve::onPointRoleChanged);
    }

    setPointsCurve(curve);

    ViewItem::onModelItemChanged();
}

void ViewCurve::onAdjustementNeeded() {adjust();}

void ViewCurve::onArrowSizeChanged() {update();}

void ViewCurve::onPointPosChanged(int index)
{
    auto *curve = static_cast<ModelCurve*>(m_modelItem);
    if(!curve || !curve->isValidPointIndex(index)) {
        return;
    }

    switch(index) {
    case 0: m_from->setPos(curve->pointPos(index)); break;
    case 1: m_mid1->setPos(curve->pointPos(index)); break;
    case 2: m_mid2->setPos(curve->pointPos(index)); break;
    case 3: m_to->setPos(curve->pointPos(index));   break;
    }

    m_textEditor.setPos(GraphicsAlgo::getPointOnLine(m_mid1->pos(), m_mid2->pos(), 1./2));
    adjust();
}

void ViewCurve::onPointRoleChanged() {update();}
