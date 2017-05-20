#include "BrushFormWidget.h"

BrushFormWidget::BrushFormWidget(QWidget *parent)
    : DataFormWidget(parent)
{
    // add widgets

    addRow(&m_tab);

    m_tab.addTab(&m_color,    "");
    m_tab.addTab(&m_gradient, "");
    m_tab.addTab(&m_pixmap,   "");

    // connect signals to slots

    connect(&m_color,    &ColorFormWidget::colorEdited,       this, &BrushFormWidget::brushEdited);
    //connect(&m_gradient, &GradientFormWidget::gradientEdited, this, &BrushFormWidget::brushEdited);
    connect(&m_pixmap,   &PixmapFormWidget::pixmapEdited,     this, &BrushFormWidget::brushEdited);

    connect(this, &BrushFormWidget::brushEdited, this, &BrushFormWidget::brushChanged);

    // set initial brush

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

void BrushFormWidget::setFormTitles(const QString &colorTitle, const QString &gradientTitle, const QString &pixmapTitle)
{
    m_tab.setTabText(0, colorTitle);
    m_tab.setTabText(1, gradientTitle);
    m_tab.setTabText(2, pixmapTitle);
}
