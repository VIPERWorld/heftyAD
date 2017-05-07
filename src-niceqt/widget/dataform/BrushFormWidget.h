#ifndef BRUSHFORMWIDGET_H
#define BRUSHFORMWIDGET_H

#include "ColorFormWidget.h"
#include "DataFormWidget.h"
#include "GradientFormWidget.h"
#include "PixmapFormWidget.h"

#include "widget/TabWidget.h"

class BrushFormWidget : public DataFormWidget
{
    Q_OBJECT

private:
    TabWidget m_tab;
        ColorFormWidget m_color;
        GradientFormWidget m_gradient;
        PixmapFormWidget m_pixmap;

public:
    explicit BrushFormWidget(QWidget *parent = 0);

    QBrush brush() const;
    void setBrush(const QBrush &brush);

    void setFormTitles(const QString &colorTitle, const QString &gradientTitle, const QString &pixmapTitle);

signals:
    void brushChanged();
    void brushEdited();

public slots:
};

#endif // BRUSHFORMWIDGET_H
