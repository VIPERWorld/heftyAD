#ifndef VIEWCURVEPOINT_H
#define VIEWCURVEPOINT_H

#include <QGraphicsRectItem>

class ModelCurve;
class ModelNode;

class ViewCurvePoint : public QGraphicsRectItem
{
protected:
    ModelCurve *m_curve;
    int m_curve_index;

    ModelNode *m_node;
    QPointF m_nodeLastPos;

    ViewCurvePoint *m_alignmentPoint;

public:
    ViewCurvePoint(QGraphicsItem *parent = 0);

    void setCurve(ModelCurve *curve, int index);
    void setAlignmentPoint(ViewCurvePoint *point);

    void attach(ModelNode *node);
    void tieToNode();

    QPointF getPosOnNodeBorderFrom(const QPointF &pos) const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // VIEWCURVEPOINT_H
