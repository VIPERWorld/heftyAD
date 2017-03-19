#include "TextAnimationWidget.h"

TextAnimationWidget::TextAnimationWidget(QWidget *parent)
    : GridWidget(parent),
      m_textAnimation(nullptr)
{
}

TextAnimation* TextAnimationWidget::textAnimation() const {return m_textAnimation;}
void TextAnimationWidget::setTextAnimation(TextAnimation *animation) {m_textAnimation = animation;}
