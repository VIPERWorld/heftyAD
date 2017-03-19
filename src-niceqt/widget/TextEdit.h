#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>

class TextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit TextEdit(QWidget *parent = 0);

    void setQLabelAppearance();

signals:

public slots:
};

#endif // TEXTEDIT_H
