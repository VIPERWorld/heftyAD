#ifndef WELCOMETEXTANIMATIONWIDGET_H
#define WELCOMETEXTANIMATIONWIDGET_H

#include "widget/PushButton.h"
#include "widget/TextAnimationWidget.h"

#include <QLabel>
#include <QTextEdit>

class WelcomeTextAnimationWidget : public TextAnimationWidget
{
    Q_OBJECT

private:
    QLabel m_space; PushButton m_skip;
    QTextEdit m_textEdit;

public:
    explicit WelcomeTextAnimationWidget(QWidget *parent = 0);
    ~WelcomeTextAnimationWidget();

private:
    void addWidgets();
    void setWidgetAttributes();

    TextAnimation* createPlainTextAnimation();
    TextAnimation* createHtmlTextAnimation();

signals:
    void textAnimationFinished();

private slots:
    void enableSkipButton();
    void clearSkipButtonFocus();
};

#endif // WELCOMETEXTANIMATIONWIDGET_H
