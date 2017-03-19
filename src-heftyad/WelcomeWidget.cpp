#include "WelcomeWidget.h"

#include <QPainter>

WelcomeWidget::WelcomeWidget(QWidget *parent)
    : GridWidget(parent)
{
    gridLayout()->setMargin(100);

    addWidget(&m_textAnimationWidget, 0, 0);

    connect(&m_textAnimationWidget, &WelcomeTextAnimationWidget::textAnimationFinished, [this](){emit needToBeHidden();});
}

void WelcomeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    QBrush background(QColor(25, 25, 25));

    painter.setBrush(background);
    painter.setPen(Qt::NoPen ); // No stroke
    painter.drawRect(0, 0, width(), height());
}
