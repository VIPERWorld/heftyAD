#ifndef COLORFORMWIDGET_H
#define COLORFORMWIDGET_H

#include "widget/FormWidget.h"

#include <QLineEdit>

class ColorFormWidget : public FormWidget
{
    Q_OBJECT

private:
    QLineEdit m_rgb;
    QLineEdit m_alpha;

public:
    explicit ColorFormWidget(QWidget *parent = 0);

signals:

public slots:
};

#endif // COLORFORMWIDGET_H
