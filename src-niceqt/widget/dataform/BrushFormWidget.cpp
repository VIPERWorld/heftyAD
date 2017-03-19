#include "BrushFormWidget.h"

BrushFormWidget::BrushFormWidget(QWidget *parent)
    : TabWidget(parent)
{
    addTab(&m_color,    "");
    addTab(&m_gradient, "");
    addTab(&m_pixmap,   "");
}

void BrushFormWidget::setFormTitles(const QString &color, const QString &gradient, const QString &pixmap)
{
    setTabText(0, color);
    setTabText(1, gradient);
    setTabText(2, pixmap);
}
