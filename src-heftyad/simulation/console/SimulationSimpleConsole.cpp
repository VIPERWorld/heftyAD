#include "SimulationClarifier.h"
#include "SimulationSimpleConsole.h"

SimulationSimpleConsole::SimulationSimpleConsole(QWidget *parent)
    : QTextEdit(parent),
      m_clarifier(nullptr)
{
    setColorScheme(GrayScheme);

    setReadOnly(true);
    setStyleSheet("SimulationSimpleConsole {font-size: 16px;}");
}

void SimulationSimpleConsole::setColorScheme(const SimulationSimpleConsole::ColorScheme &scheme)
{
    m_colorScheme = scheme;

    QPalette p = palette();
    p.setColor(QPalette::Base, m_colorScheme.backgroundColor);
    p.setColor(QPalette::Text, m_colorScheme.foregroundColor);
    setPalette(p);
}

void SimulationSimpleConsole::setColorScheme(int builtinColorScheme)
{
    switch(builtinColorScheme) {
    case DarkScheme:
        setColorScheme({QColor(32, 32, 32), Qt::white, Qt::blue, Qt::yellow, QColor(Qt::red).lighter()});
        break;
    case DarkSchemeLighten:
        setColorScheme({QColor(32, 32, 32), Qt::white, Qt::blue, Qt::yellow, Qt::red});
        m_colorScheme.lighter();
        break;

    case GrayScheme:
        setColorScheme({QColor(Qt::gray).lighter(), Qt::black, Qt::blue, QColor(Qt::yellow).darker(), Qt::red});
        break;

    case WhiteScheme:
        setColorScheme({Qt::white, Qt::black, Qt::blue, QColor(Qt::yellow).darker(), Qt::red});
        break;
    case WhiteSchemeDarken:
        setColorScheme({Qt::white, Qt::black, Qt::blue, Qt::yellow, Qt::red});
        m_colorScheme.darker();
        break;
    }
}

void SimulationSimpleConsole::setClarifier(SimulationClarifier *clarifier)
{
    if(m_clarifier) {
        disconnect(m_clarifier, 0, this, 0);
    }

    m_clarifier = clarifier;
    if(m_clarifier) {
        connect(m_clarifier, &SimulationClarifier::needText,        this, &SimulationSimpleConsole::onTextNeeded);
        connect(m_clarifier, &SimulationClarifier::needTextRemoval, this, &SimulationSimpleConsole::onTextRemovalNeeded);
    }

//    doTest();
}

void SimulationSimpleConsole::onTextNeeded(const QString &text, int kind)
{
    switch(kind) {
    case SimulationClarifier::InformativeText: setTextColor(m_colorScheme.infoColor);       break;
    case SimulationClarifier::WarningText:     setTextColor(m_colorScheme.warningColor);    break;
    case SimulationClarifier::ErrorText:       setTextColor(m_colorScheme.errorColor);      break;
    default:                                   setTextColor(m_colorScheme.foregroundColor); break;
    }

    append(text);
}

void SimulationSimpleConsole::onTextRemovalNeeded()
{
    clear();
}

void SimulationSimpleConsole::doTest()
{
    m_clarifier->addText("<I>Yes I can</I>\n");
    //
    m_clarifier->addInfo("Yes we can");
    m_clarifier->addText("Yes we can");
    m_clarifier->addInfo("Yes we can\n");
    //
    m_clarifier->addWarning("Yes you can\n");
    //
    m_clarifier->addError("Yes they can");
}

void SimulationSimpleConsole::colorScheme::lighter(bool excludeBackground, int factor)
{
    if(!excludeBackground) {
        backgroundColor = backgroundColor.lighter(factor);
    }
    foregroundColor = foregroundColor.lighter(factor);
    infoColor       = infoColor.lighter(factor);
    warningColor    = warningColor.lighter(factor);
    errorColor      = errorColor.lighter(factor);
}

void SimulationSimpleConsole::colorScheme::darker(bool excludeBackground, int factor)
{
    if(!excludeBackground) {
        backgroundColor = backgroundColor.darker(factor);
    }
    foregroundColor = foregroundColor.darker(factor);
    infoColor       = infoColor.darker(factor);
    warningColor    = warningColor.darker(factor);
    errorColor      = errorColor.darker(factor);
}
