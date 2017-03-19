#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include "WelcomeTextAnimationWidget.h"

#include "widget/GridWidget.h"

class WelcomeWidget : public GridWidget
{
    Q_OBJECT

protected:
    WelcomeTextAnimationWidget m_textAnimationWidget;

public:
    explicit WelcomeWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void needToBeHidden();
};

#endif // WELCOMEWIDGET_H
