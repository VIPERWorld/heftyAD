#ifndef VIEWCURVE_H
#define VIEWCURVE_H

#include "ViewItem.h"

class ModelCurve;
class ViewCurvePoint;

class ViewCurve : public ViewItem
{
    Q_OBJECT

protected:
    // points: initialized in createPoints();
    ViewCurvePoint *m_from;
    ViewCurvePoint *m_mid1;
    ViewCurvePoint *m_mid2;
    ViewCurvePoint *m_to;

public:
    ViewCurve(QGraphicsItem *parent = 0);

    /* Updates the edge start and end position. */
    void adjust();
    void setPointsVisibility(bool visible);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void paintCubicBezierCurve(QPainter *painter,
                               const QPointF &from, const QPointF &ctrlPt1, const QPointF &ctrlPt2, const QPointF &to,
                               const QPen &curvePen, const QBrush &curveBrush,
                               bool tieCtrlPoints, const QPen &tiePen, const QBrush &tieBrush,
                               bool arrowAtFrom, const QBrush &fromBrush,
                               bool arrowAtTo, const QBrush &toBrush,
                               qreal arrowSize);
    void paintArrows(QPainter *painter,
                     const QLineF &line,
                     bool atSource, const QBrush &sourceBrush,
                     bool atTarget, const QBrush &targetBrush,
                     qreal arrowSize);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void createPoints();
    /**
     * When this method is called, the scene should have been defined (see code to understand why).
     * In other words, scene() shouldn't return null (this item has been added to a scene for instance).
     */
    void setPointsCurve(ModelCurve *curve);

signals:

public slots:

protected slots:
    void onModelItemChanged() override;

    void onAdjustementNeeded();
    void onArrowSizeChanged();
    void onPointPosChanged(int index);
    void onPointRoleChanged();
};

#endif // VIEWCURVE_H
