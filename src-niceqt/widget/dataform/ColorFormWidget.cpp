#include "ColorFormWidget.h"

ColorFormWidget::ColorFormWidget(QWidget *parent)
    : FormWidget(parent)
{
    addRow("rgb",   &m_rgb);
    addRow("alpha", &m_alpha);

    m_rgb.setInputMask("000, 000, 000;_");
}
