#include "SciCodeEditor.h"

#include <Qsci/qscilexerxml.h>

#include <QWheelEvent>

SciCodeEditor::SciCodeEditor(QWidget *parent)
    : QsciScintilla(parent)
{
    setBraceMatching(QsciScintilla::SloppyBraceMatch);
    SendScintilla(QsciScintilla::SCI_SETHSCROLLBAR, 0); // Hide the horizontal scrollbar

    initializeFont();
    initializeMargin();
    initializeCaretLine();
    initializeLexer();
    initializeFolding();

    connect(this, &SciCodeEditor::textChanged, this, &SciCodeEditor::onTextChanged);
}

void SciCodeEditor::initializeFont()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    setFont(font);
}

void SciCodeEditor::initializeMargin()
{
    QFontMetrics fontmetrics = QFontMetrics(font());
    setMarginsFont(font());
    setMarginWidth(0, fontmetrics.width(QString::number(lines())) + 6);
    setMarginLineNumbers(0, true);
    setMarginsBackgroundColor(QColor("#cccccc"));
}

void SciCodeEditor::initializeCaretLine()
{
    // Current line visible with special background color
    setCaretLineVisible(true);
    setCaretLineBackgroundColor(QColor("#ffe4e4"));
}

void SciCodeEditor::initializeLexer()
{
    auto *lexer = new QsciLexerXML;
    lexer->setDefaultFont(font());
    setLexer(lexer);
}

void SciCodeEditor::initializeFolding()
{
    auto state = static_cast<QsciScintilla::FoldStyle>((!folding()) * 5);
    if(!state) {
        foldAll(false);
    }
    setFolding(state);
}

void SciCodeEditor::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier) {
        event->delta() > 0 ? zoomIn(2) : zoomOut(2);
        return;
    }

    QsciScintilla::wheelEvent(event);
}

void SciCodeEditor::onTextChanged()
{
    QFontMetrics fontmetrics = fontMetrics();
    setMarginWidth(0, fontmetrics.width(QString::number(lines())) + 6);
}
