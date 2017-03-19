#ifndef SCICODEEDITOR_H
#define SCICODEEDITOR_H

#include <Qsci/qsciscintilla.h>

class SciCodeEditor : public QsciScintilla
{
    Q_OBJECT

public:
    explicit SciCodeEditor(QWidget *parent = 0);

private:
    void initializeFont();
    void initializeMargin();
    void initializeCaretLine();
    void initializeLexer();
    void initializeFolding();

protected:
    void wheelEvent(QWheelEvent *event) override;

signals:

public slots:
    void onTextChanged();
};

#endif // SCICODEEDITOR_H
