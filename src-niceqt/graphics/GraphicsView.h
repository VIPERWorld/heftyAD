#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    typedef enum {
        NoWheel=0,      // Zooming in or out simply does nothing.
        WheelDir1=1,    // When wheeling the mouse towards, the view is zoomed in.
        WheelDir2=-1    // When wheeling the mouse towards, the view is zoomed out.
    } WheelDirection;

private:
    qreal m_zoomInScalFactor; // Zoom in scale factor : its default value is 1.2
    WheelDirection m_wheelDirection; // Indicates how the mouse wheel event is handled.

public:
    explicit GraphicsView(QWidget *parent = 0);
    virtual ~GraphicsView();

    qreal zoomInScaleFactor() const;
    void setZoomInScaleFactor(qreal scaleFactor);

    WheelDirection wheelDirection() const;
    void setWheelDirection(WheelDirection direction);

    QGraphicsItem* itemUnderMouseEvent(QMouseEvent *event) const;
    QGraphicsItem* itemUnderContextMenuEvent(QContextMenuEvent *event) const;

protected:
    virtual bool scaleView(qreal scaleFactor);

    void wheelEvent(QWheelEvent *event) override;

signals:

public slots:
    bool saveImageTo(const QString &filePath, const char *format = nullptr);

    virtual void setEnabledItemEdition(bool enabled);
    virtual void setEnabledItemMove(bool enabled);

    void selectAll();
    void deselectAll();

    virtual void removeSelectedItems();

    bool zoomIn();
    bool zoomOut();
};

#endif // GRAPHICSVIEW_H
