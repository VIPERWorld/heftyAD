#ifndef TEXTANIMATION_H
#define TEXTANIMATION_H

#include <QMap>
#include <QObject>
#include <QTimer>

/**
 * The TextAnimation class provides an engine for text animation.
 * Any instance of this class behaves like an engine which
 * controls how a given text can be animated.
 *
 * This class can therefore be coupled with a text widget,
 * in order to display the text as soon as the engine animates it.
 * But you're not limited to using text widgets only.
 *
 * The TextAnimation class can animate only one text at a given time.
 * The setText function sets the text to animate. If a text is already being animated
 * when this function is called, the engine registeres the new text and then animates it
 * once the current animation terminates. Texts are animated in the order they are registered.
 *
 * [Now tell how this engine works]
 */

class TextAnimation : public QObject
{
    Q_OBJECT

protected:
    bool m_isRunning; // Tells whether this animation is running.

    QString m_text; // The text being animated (or to animate).
    QStringList m_waintingTexts; // Texts that will be animated once the current animation finishes.

    QTimer m_timer; // The timer that controls how often the engine skipped from
                    // a certain character (within the text) to another one during animation.
    int m_frequence; // Indicates the frequence of the animation in milliseconds.

    int m_index; // Value of current index: it varies from 0 to m_text.length()-1.
    bool m_advanceByCharacter;

    QMap<QString, int> m_breakPoints;

public:
    explicit TextAnimation(QObject *parent = 0);
    ~TextAnimation();

    bool isRunning() const;

    bool advanceByCharacter() const;
    void setAdvanceByCharacter(bool value);

    QString text() const;
    /**
     * Sets the text to animate and eventually starts animating.
     * If this engine is already running, the given text is registered
     * and then animated once the current animation finishes.
     * Texts are animated in the order they are registered.
     */
    void setText(const QString &text, bool startAnimation = false);

    int frequence() const;
    void setFrequence(int milliseconds);

    QMap<QString, int> breakPoints() const;
    void setBreakPoints(const QMap<QString, int> &map);

    void addBreakPoint(const QString &str, int duration);
    void addBreakPoints(const QMap<QString, int> &map);
    void removeBreakPoint(const QString &str);
    void removeBreakPoints(const QMap<QString, int> &map);

protected:
    /**
     * Returns additional milliseconds break fo the given string.
     * The given string should have been registered as break point.
     * If not, this function returns 0.
     */
    int extraBreakFor(const QString &str);

    /**
     * Returns the next index value, basing on the registered break points.
     * The given string will be set to the next break point (if any).
     */
    int nextIndex(QString &nextBreakPoint) const;
    void advanceIndex(QString &nextBreakPoint);

private:
    /**
     * Adds default break points for this animation.
     * The registered break points are:
     *     . ! ? with the same pause duration.
     *     , ; : with the same but shorter pause duration.
     */
    void registerDefaultBreakPoints();

signals:
    void started();
    void finished();

    void nextSubStr(QString text);
    void nextTotalStr(QString text);

public slots:
    /**
     * Starts the animation and emits the started() signal.
     * The animation is started only if it were not running.
     */
    void start();
    /**
     * Stops the animation and emits the finished() signal.
     * The animation is stopped only if it were not running.
     * If there is a wainting text, the engine will the animate it.
     */
    void stop();
    /**
     * Does the same thing as stop() but first clears the wainting text list.
     * So waiting texts won't be animated.
     */
    void stopAll();

protected slots:
    /**
     * Advances this animation.
     */
    virtual void advance();

    /**
     * This function is called each time the started() signal is emitted.
     * Does nothing.
     */
    void onStarted();
    /**
     * This function is called each time the finished() signal is emitted.
     * Triggers the animation of the first waiting text.
     */
    void onFinished();
};

#endif // TEXTANIMATION_H
