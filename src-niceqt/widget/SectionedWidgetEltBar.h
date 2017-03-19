#ifndef SECTIONEDWIDGETELTBAR_H
#define SECTIONEDWIDGETELTBAR_H

#include <QPen>
#include <QPushButton>

class SectionedWidgetEltBar : public QPushButton
{
    Q_OBJECT

    friend class SectionedWidgetElt;

private:
    typedef enum {
        UP,
        RIGHT,
        BOTTOM,
        LEFT
    } TriangleDirection;

private:
    bool m_widgetIsUnrolled; // set by SectionedWidgetElt

    QBrush m_indicatorBrush;
    QPen m_indicatorPen;

public:
    explicit SectionedWidgetEltBar(QWidget *parent = 0);

    void setIndicatorBrush(const QBrush &brush);
    void setIndicatorPen(const QPen &pen);

    static void drawTriangle(QPainter *painter, const QPointF &A, const QPointF &B, const QPointF &C);
    static void drawTriangle(QPainter *painter, const QList<QPointF> &ABC);

    /**
     * Returns a list of 3 points.
     */
    static QList<QPointF> getTriangle(const QRectF &rect, TriangleDirection dir);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // SECTIONEDWIDGETELTBAR_H
