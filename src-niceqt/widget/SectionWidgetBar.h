#ifndef SECTIONWIDGETBAR_H
#define SECTIONWIDGETBAR_H

#include <QPen>
#include <QPushButton>

class SectionWidgetBar : public QPushButton
{
    Q_OBJECT

    friend class SectionWidget; // to access m_widgetIsUnrolled

private:
    typedef enum {
        UP,
        RIGHT,
        BOTTOM,
        LEFT
    } TriangleDirection;

private:
    bool m_widgetIsUnrolled; // set by SectionWidget

    QBrush m_indicatorBrush;
    QPen m_indicatorPen;

public:
    explicit SectionWidgetBar(QWidget *parent = 0);

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

#endif // SECTIONWIDGETBAR_H
