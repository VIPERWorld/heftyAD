#include "TextEdit.h"

TextEdit::TextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    setReadOnly(true);
    setTextInteractionFlags(Qt::NoTextInteraction);
    setAlignment(Qt::AlignJustify);
}

void TextEdit::setQLabelAppearance()
{
    setFocusPolicy(Qt::NoFocus);

    QWidget *v = viewport();
    v->setCursor(Qt::ArrowCursor);

    // Customize the background to be "QLabel-like"
    QPalette pal = palette();
    pal.setColor(QPalette::Base, pal.color(QPalette::Window));
    setPalette(pal);

    setStyleSheet("TextEdit{border: none;}");
}
