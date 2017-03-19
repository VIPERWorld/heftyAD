#include "WelcomeTextAnimationWidget.h"

#include "animation/TextAnimation.h"

WelcomeTextAnimationWidget::WelcomeTextAnimationWidget(QWidget *parent)
    : TextAnimationWidget(parent)
{
    addWidgets();
    setWidgetAttributes();

    const auto &html = false;
    m_textAnimation = html ? createHtmlTextAnimation() : createPlainTextAnimation();

    connect(m_textAnimation, &TextAnimation::started,  this, &WelcomeTextAnimationWidget::enableSkipButton);
    connect(m_textAnimation, &TextAnimation::finished, [this](){emit textAnimationFinished();});
    QTimer::singleShot(1000, m_textAnimation, SLOT(start())); // Prevent the animation from starting immediately after the widget shows up.

    connect(&m_skip, &PushButton::pressed, m_textAnimation, &TextAnimation::stop);
    connect(m_textAnimation, &TextAnimation::finished, this, &WelcomeTextAnimationWidget::clearSkipButtonFocus);
}

WelcomeTextAnimationWidget::~WelcomeTextAnimationWidget()
{
    delete m_textAnimation;
}

void WelcomeTextAnimationWidget::addWidgets()
{
    addWidget(&m_space, 0, 0);
    addWidget(&m_skip, 0, 1);
    addWidget(&m_textEdit, 1, 0, 1, 2);
}

void WelcomeTextAnimationWidget::setWidgetAttributes()
{
    m_space.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_skip.setText(trUtf8("Ignorer"));
    m_skip.setEnabled(false);

    m_textEdit.setTextInteractionFlags(Qt::NoTextInteraction);
    m_textEdit.setEnabled(false);
}

TextAnimation* WelcomeTextAnimationWidget::createPlainTextAnimation()
{
    auto *textAnimation = new TextAnimation;
    textAnimation->setText(
        trUtf8("Hello !\n\n"
               "Vous souhaitez expliquer ou comprendre un certain algorithme. "
               "Mais vous avez beau chercher, les différents moyens auxquels vous avez pu avoir accès ne vous satisfont point.\n\n"
               "Et bien vous avez de la chance. "
               "heftyAD, votre vulgarisateur d'algorithmes qui se veut complet, pédagogique et ludique, est là pour vous aider.\n\n"
               "Très simplement, créez un modèle, créez un algorithme, puis appliquez-le à votre modèle via de très belles simulations animées.\n\n"
               "Bonne découverte !"
        )
    );
    connect(textAnimation, &TextAnimation::nextSubStr, &m_textEdit, &QTextEdit::insertPlainText);

    return textAnimation;
}

TextAnimation* WelcomeTextAnimationWidget::createHtmlTextAnimation()
{
    auto *textAnimation = new TextAnimation;
    textAnimation->setText(
        trUtf8("Hello !<br/><br/>"
               "Vous souhaitez <U>expliquer</U> ou <U>comprendre</U> un certain algorithme. "
               "Mais vous avez beau chercher, les différents moyens auxquels vous avez pu avoir accès ne vous satisfont point.<br/><br/>"
               "Et bien vous avez de la chance. "
               "<B><I>heftyAD</I></B>, votre vulgarisateur d'algorithmes qui se veut complet, pédagogique et ludique, est là pour vour aider.<br/><br/>"
               "Très simplement, créez un modèle, créez un algorithme, puis appliquez-le à votre modèle via de très belles simulations animées.<br/><br/>"
               "Bonne découverte !"
        )
    );
    connect(textAnimation, &TextAnimation::nextTotalStr, &m_textEdit, &QTextEdit::setHtml);

    return textAnimation;
}

void WelcomeTextAnimationWidget::enableSkipButton()
{
    m_skip.setEnabled(true);
}

void WelcomeTextAnimationWidget::clearSkipButtonFocus()
{
    m_skip.clearFocus();
}
