#ifndef TEXTANIMATIONWIDGET_H
#define TEXTANIMATIONWIDGET_H

#include "GridWidget.h"

class TextAnimation;

class TextAnimationWidget : public GridWidget
{
    Q_OBJECT

protected:
    TextAnimation *m_textAnimation;

public:
    explicit TextAnimationWidget(QWidget *parent = 0);

    TextAnimation* textAnimation() const;
    void setTextAnimation(TextAnimation *animation);

signals:

public slots:
};

#endif // TEXTANIMATIONWIDGET_H
