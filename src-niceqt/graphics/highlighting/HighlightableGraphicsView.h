#ifndef HIGHLIGHTABLEGRAPHICSVIEW_H
#define HIGHLIGHTABLEGRAPHICSVIEW_H

#include "graphics/GraphicsView.h"

#include <functional>

class GraphicsViewHighlighter;
class HighlightingData;
class HighlightingTextData;

/**
 * The name of this class may be changed later.
 */
class HighlightableGraphicsView : public GraphicsView
{
    Q_OBJECT

protected:
    GraphicsViewHighlighter *m_highlighter;

    QList<HighlightingData*> m_highlightingData;
    QList<HighlightingData*> m_highlightingNewedData;

    int m_highlightingDataAcceleration;

public:
    HighlightableGraphicsView(QWidget *parent = 0);
    ~HighlightableGraphicsView();

    void setHighlighter(GraphicsViewHighlighter *highlighter);

    int highlightingDataAcceleration() const;
    void setHighlightingDataAcceleration(int acc);

    /*
     * When adding a highlighting data, make sure it won't get deleted before this view is deleted.
     * You shouldn't for instance add a locally created data and start it asynchronously.
     * Indeed since this view uses the data's attributes to draw stuffs on screen,
     * the application may crash if the data is deleted first.
     */
    void addHighlighting(HighlightingData *data);
    void addHighlighting(HighlightingData *data, bool inForeground);
    void addHighlightingNewedData(HighlightingData *data, bool inForeground);

    HighlightingTextData *addHighlightingText(int duration, bool inForeground, bool singleShotTimer,
                                              const QString &text, const QRectF &rect, int flags,
                                              const QPen &textPen = QPen(), const QFont &textFont = QFont(),
                                              const QPen &rectPen = Qt::NoPen, const QBrush &rectBrush = Qt::NoBrush);

    void suspendHighlighting();
    void resumeHighlighting();
    void stopHighlighting();

    void doTest();

protected:
    void onHighlighterNeedsStart(HighlightingData *data, int duration = -1);
    void onHighlighterNeedsSuspend(HighlightingData *data);
    void onHighlighterNeedsStop(HighlightingData *data);

    void drawAllHighlightingData(QPainter *painter);
    void drawAllHighlightingData(QPainter *painter, std::function<bool (HighlightingData *data)> predicate);
    void drawHighlightingData(QPainter *painter, HighlightingData *data);

    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;

signals:
    void highlightingDataAccelerationChanged();
};

#endif // HIGHLIGHTABLEGRAPHICSVIEW_H
