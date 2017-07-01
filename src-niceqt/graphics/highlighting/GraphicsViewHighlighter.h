#ifndef GRAPHICSVIEWHIGHLIGHTER_H
#define GRAPHICSVIEWHIGHLIGHTER_H

#include <QJSValue>
#include <QObject>
#include <QPen>

class HighlightingData;

class GraphicsViewHighlighter : public QObject
{
    Q_OBJECT

public:
    explicit GraphicsViewHighlighter(QObject *parent = 0);

    void start(HighlightingData *data, int duration = -1, bool synchrone = true);
    void suspend(HighlightingData *data);
    void stop(HighlightingData *data);

    Q_INVOKABLE void start(const QJSValue &dataProxy, int duration = -1, bool synchrone = true);
    Q_INVOKABLE void suspend(const QJSValue &dataProxy);
    Q_INVOKABLE void stop(const QJSValue &dataProxy);

signals:
    void startRequested(HighlightingData *data, int duration = -1);
    void suspendRequested(HighlightingData *data);
    void stopRequested(HighlightingData *data);

public slots:
};

#endif // GRAPHICSVIEWHIGHLIGHTER_H
