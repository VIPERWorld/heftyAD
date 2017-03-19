#ifndef MODELCURVE_H
#define MODELCURVE_H

#include "ModelItem.h"

class ModelNode;

class ModelCurve : public ModelItem
{
    Q_OBJECT

public:
    typedef enum {
        BezierCtrlPoint, // the 2 points in the middle behave like bezier curve control points.
        CurvePoint       // the 2 points in the middle lays on the bezier curve determining the curve path
    } PointRole;

protected:
    ModelNode &m_node1;
    ModelNode &m_node2;
    bool m_bidir; // bidirectional
    qreal m_arrowSize;

    QPointF m_points_pos[4]; // These 4 points determine the curve path. They are used to draw a bezier curve.
    PointRole m_points_role;

public:
    ModelCurve(const ModelNode &node1, const ModelNode &node2,
               bool bidir = false,
               QObject *parent = 0);

    ModelNode& node1() const;
    ModelNode& node2() const;
    bool isBidir() const;
    bool isLoop() const;

    qreal arrowSize() const;
    void setArrowSize(int size, bool notify = true);

    bool isValidPointIndex(int index) const;

    QPointF pointPos(int index) const;
    void setPointPos(int index, const QPointF &pos, bool notify = true);

    PointRole pointRole() const;
    void setPointRole(int role, bool notify = true);

    void alignMidPoints();
    void alignPoints();

    QPair<QPointF, QPointF> controlPoints() const;
    QPainterPath path() const;
    /**
     * Return the rectangle defined by the four points.
     *
     * The value returned by this function may not include this item' shape (for some layout of the points).
     * If you want shape() to be included, use rect() or shape().boundingRect() instead.
     */
    QRectF minimalRect() const;

    qreal scenePenWidth() const override;
    QRectF rect() const override;
    qreal extra() const override;
    QPainterPath shape() const override;

    void sendAttributeChanges() override;

private:
    void initPointsPos();
    void initEndPoints();
    void initMidPoints();

signals:
    void needAdjustment();

    void arrowSizeChanged();
    void pointPosChanged(int index);
    void pointRoleChanged();
};

#endif // MODELCURVE_H
