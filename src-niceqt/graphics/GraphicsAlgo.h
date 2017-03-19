#ifndef GRAPHICSALGO_H
#define GRAPHICSALGO_H

#include <QPair>
#include <QPointF>

class GraphicsAlgo
{
private:
    GraphicsAlgo();

    /* Disable copy of this class (using C++11 syntax). */
    GraphicsAlgo(const GraphicsAlgo&) = delete;
    GraphicsAlgo& operator=(const GraphicsAlgo&) = delete;
    GraphicsAlgo(const GraphicsAlgo&&) = delete;

public:

    /**
     * Returns a point (explanation below).
     *
     * To have a better comprehension of what is going to be said,
     * please take a pencil and draw a sketch as you're reading.
     *
     * Let say we have a given point M.
     * Let say we also have another point O corresponding to the center of some circle item.
     * Now assume r is that circle item radius.
     *
     * This function returns the point I
     *     laying on the line (OM)
     * AND bordering (intersecting) that said circle.
     *
     * Note: This function may be useful if you are coding a graph drawing software
     *       and want to make sure any edge joining (the borders of) two nodes is always well drawn (as those nodes move around).
     */
    static QPointF getPointOnCircle(const QPointF &M, const QPointF &O, qreal r);

    /**
     * Returns a point (explanation below).
     *
     * To have a better comprehension of what is going to be said,
     * please take a pencil and draw a sketch as you're reading.
     *
     * Let say we have a given point M.
     * Let say we also have another point O corresponding to the center of some rectangular item.
     * Now assume w and h are that rectangular item width and height.
     *
     * This function returns the point I
     *     laying on the line (OM)
     * AND bordering (intersecting) that said rectangle.
     */
    static QPointF getPointOnRectangle(const QPointF &M, const QPointF &O, qreal w, qreal h);

    /**
     * Returns a point laying on the line joining point1 and point2.
     * That point is computed using the parameterized position specified by t:
     *     point = (1-t)*point1 + t*point2
     */
    static QPointF getPointOnLine(const QPointF &point1, const QPointF &point2, qreal t);

    /**
     * Returns the control points p1 and p2 such that there is a Bezier curve
     *     from "from",
     *     to "to",
     *     passing through "by1" and "by2".
     *
     * If we draw this Bezier curve, it'll be likely that
     *     "by1" is somewhere u (e.g. 1/3) of the way between "from" and "to",
     * and "by2" is somewhere v (e.g. 2/3) of the way between "from" and "to".
     *
     * u and v MUST be two real such that
     *     0.0 < u < 1.0
     *     0.0 < v < 1.0
     * If "by1" is before "by2", we MAY have u<v (and vice versa).
     */
    static QPair<QPointF, QPointF> getControlPoints(const QPointF &from, const QPointF &by1, const QPointF &by2, const QPointF &to,
                                                    qreal u, qreal v);
};

#endif // GRAPHICSALGO_H
