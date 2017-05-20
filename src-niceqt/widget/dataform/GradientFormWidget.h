#ifndef GRADIENTFORMWIDGET_H
#define GRADIENTFORMWIDGET_H

#include "DataFormWidget.h"

#include <QComboBox>
#include <QLabel>

class GradientFormWidget : public DataFormWidget
{
    Q_OBJECT

protected:
    QGradient *m_gradient;

    QLabel m_typeLabel; QComboBox m_type;

public:
    explicit GradientFormWidget(QWidget *parent = 0);

    void setFieldTexts(const QString &typeText);

signals:

public slots:
};

#endif // GRADIENTFORMWIDGET_H
