#include "BrushFormWidget.h"

BrushFormWidget::BrushFormWidget(QWidget *parent)
    : TabWidget(parent)
{
    addTab(&m_color,    "");
    addTab(&m_gradient, "");
    addTab(&m_pixmap,   "");

    connect(&m_color,    &ColorFormWidget::colorEdited,       this, &BrushFormWidget::brushEdited);
    //connect(&m_gradient, &GradientFormWidget::gradientEdited, this, &BrushFormWidget::brushEdited);
    connect(&m_pixmap,   &PixmapFormWidget::pixmapEdited,     this, &BrushFormWidget::brushEdited);

    setBrush(Qt::NoBrush);
}

QBrush BrushFormWidget::brush() const
{
    QBrush retVal(Qt::SolidPattern);

    const QPixmap &pix = m_pixmap.pixmap();
    if(!pix.isNull()) {
        retVal.setTexture(pix);
    }
    retVal.setColor(m_color.color());

    return retVal;
}

void BrushFormWidget::setBrush(const QBrush &brush)
{
    m_color.setColor(brush.color());
    //m_gradient.
    m_pixmap.setPixmap(brush.texture());

    emit brushChanged();
}

void BrushFormWidget::setFormTitles(const QString &color, const QString &gradient, const QString &pixmap)
{
    setTabText(0, color);
    setTabText(1, gradient);
    setTabText(2, pixmap);
}
