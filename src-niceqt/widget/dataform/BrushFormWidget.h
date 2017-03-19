#ifndef BRUSHFORMWIDGET_H
#define BRUSHFORMWIDGET_H

#include "ColorFormWidget.h"
#include "GradientFormWidget.h"
#include "PixmapFormWidget.h"

#include "widget/TabWidget.h"

class BrushFormWidget : public TabWidget
{
    Q_OBJECT

private:
    ColorFormWidget m_color;
    GradientFormWidget m_gradient;
    PixmapFormWidget m_pixmap;

public:
    explicit BrushFormWidget(QWidget *parent = 0);

    void setFormTitles(const QString &color, const QString &gradient, const QString &pixmap);

signals:

public slots:
};

#endif // BRUSHFORMWIDGET_H
