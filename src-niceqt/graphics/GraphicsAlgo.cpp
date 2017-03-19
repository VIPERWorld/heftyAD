#include "GraphicsAlgo.h"

#include <QLineF>
#include <QRectF>

GraphicsAlgo::GraphicsAlgo()
{
}

QPointF GraphicsAlgo::getPointOnCircle(const QPointF &M, const QPointF &O, qreal r)
{
    /*
     * Mathematical proof
     * ==================
     *
     * Let say I is the point we want to find (the one returned by this function).
     *
     * vect(MI) = (MI)        * vect(MO)
     *          = (MO - r)/MO * vect(MO)
     * Now vect(MI) can be computed using only known data (M, O and r).
     * So are I coordinates (which can then be deduced from M coordinates).
     */

    QPointF I;

    const qreal &MO = QLineF(M, O).length();
    const QPointF &vMO = O-M; // vector MO
    const QPointF &vMI = ((MO-r)/MO)*vMO; // vector MI

    I.setX(vMI.x() + M.x());
    I.setY(vMI.y() + M.y());

    return I;
}

QPointF GraphicsAlgo::getPointOnRectangle(const QPointF &M, const QPointF &O, qreal w, qreal h)
{
    const QPointF &pt = getPointOnCircle(M, O, std::max(w, h));

    // E, F, G and H determine the rectangular item corners (in clockwise orientation).
    const QPointF &E = O - QPointF(w/2, h/2);
    const QPointF &F = E + QPointF(w, 0);
    const QPointF &G = F + QPointF(0, h);
    const QPointF &H = G - QPointF(w, 0);

    const bool &onLeft   = pt.x() <= E.x(); // A is at the rectangle left
    const bool &onTop    = pt.y() <= E.y();
    const bool &onRight  = pt.x() >= F.x();
    const bool &onBottom = pt.y() >= H.y();

    // Handle specific cases

    if(onTop && onLeft) {
        return E;
    }
    if(onTop && onRight) {
        return F;
    }
    if(onBottom && onLeft) {
        return H;
    }
    if(onBottom && onRight) {
        return G;
    }

    // Handle general cases

    const QLineF &line(QLineF(pt, O));
    QPointF I; // the intersection point

    if(onLeft) {
        line.intersect(QLineF(E, H), &I);
    }
    else if(onTop) {
        line.intersect(QLineF(E, F), &I);
    }
    else if(onRight) {
        line.intersect(QLineF(F, G), &I);
    }
    else if(onBottom) {
        line.intersect(QLineF(H, G), &I);
    }

    return I;
}

QPointF GraphicsAlgo::getPointOnLine(const QPointF &point1, const QPointF &point2, qreal t)
{
    return (1-t)*point1 + t*point2; // QLineF::pointAt does the same job
}

/**
 * Algorithm based on this MARVELOUS article
 *     http://polymathprogrammer.com/2007/06/27/reverse-engineering-bezier-curves/
 */
QPair<QPointF, QPointF> GraphicsAlgo::getControlPoints(const QPointF &from, const QPointF &by1, const QPointF &by2, const QPointF &to,
                                                       qreal u, qreal v)
{
    const QPointF &c = by1 - pow(1-u, 3)*from - pow(u, 3)*to;
    const QPointF &d = by2 - pow(1-v, 3)*from - pow(v, 3)*to;

    const qreal &det = 9*u*v*(1-u)*(1-v)*(v-u);
    const qreal mat[2][2] = {
        { 3*(1-v)*pow(v, 2), -3*(1-u)*pow(u, 2) },
        { -3*pow(1-v, 2)*v,  3*pow(1-u, 2)*u    }
    };

    const QPointF &p1 = 1/det * (mat[0][0]*c + mat[0][1]*d);
    const QPointF &p2 = 1/det * (mat[1][0]*c + mat[1][1]*d);

    return QPair<QPointF, QPointF>(p1, p2);
}
