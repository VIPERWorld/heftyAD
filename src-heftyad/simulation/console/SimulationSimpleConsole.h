#ifndef SIMULATIONSIMPLECONSOLE_H
#define SIMULATIONSIMPLECONSOLE_H

#include <QTextEdit>

class SimulationClarifier;

class SimulationSimpleConsole : public QTextEdit
{
    Q_OBJECT

public:
    typedef struct colorScheme {
        QColor backgroundColor;
        QColor foregroundColor;

        QColor infoColor;
        QColor warningColor;
        QColor errorColor;

        void lighter(bool excludeBackground = true, int factor = 150);
        void darker(bool excludeBackground = true, int factor = 200);
    } ColorScheme;

    /**
     * In this enum:
     *     Dark<...>  means dark  background
     *     White<...> means white background
     *     ...
     */
    typedef enum {
        DarkScheme,
        DarkSchemeLighten,

        GrayScheme,

        WhiteScheme,
        WhiteSchemeDarken,
    } BuiltinColorScheme;

protected:
    ColorScheme m_colorScheme;
    SimulationClarifier *m_clarifier;

public:
    explicit SimulationSimpleConsole(QWidget *parent = 0);

    void setColorScheme(const ColorScheme &scheme);
    void setColorScheme(int builtinColorScheme);

    void setClarifier(SimulationClarifier *clarifier);

signals:

protected slots:
    void onTextNeeded(const QString &text, int kind);
    void onTextRemovalNeeded();

    void doTest();
};

#endif // SIMULATIONSIMPLECONSOLE_H
