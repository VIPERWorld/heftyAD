#include "PenFormWidget.h"

PenFormWidget::PenFormWidget(QWidget *parent)
    : DataFormWidget(parent)
{
    // add widgets

    addRow(&m_styleLabel, &m_style);
    addRow(&m_widthLabel, &m_width);
    addRow(&m_colorGroup);
    m_colorGroup.gridWidget()->addWidget(&m_color, 0, 0);

    // customize widgets

    setFieldTexts("Style", "Width", "Color");
    m_width.setContextMenuPolicy(Qt::NoContextMenu);

    m_style.addItem("No Pen");
    m_style.addItem("Solid Line");
    m_style.addItem("Dash Line");
    m_style.addItem("Dot Line");
    m_style.addItem("Dash Dot Line");
    m_style.addItem("Dash Dot Dot Line");
    m_style.addItem("Custom Dash Line");

    // connect signals/slots

    connect(&m_style, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),       this, &PenFormWidget::onFieldsEdited);
    connect(&m_width, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &PenFormWidget::onFieldsEdited);

    connect(&m_color, &ColorFormWidget::colorEdited, this, &PenFormWidget::onFieldsEdited);
}

QPen PenFormWidget::pen() const {return m_pen;}
void PenFormWidget::setPen(const QPen &pen)
{
    m_pen = pen;
    updateFields();

    emit penChanged();
}

void PenFormWidget::setFieldTexts(const QString &styleText, const QString &widthText, const QString &colorTitle)
{
    m_styleLabel.setText(styleText);
    m_widthLabel.setText(widthText);
    m_colorGroup.setTitle(colorTitle);
}

void PenFormWidget::updateFields()
{
    const QSignalBlocker blocker1(&m_style); Q_UNUSED(blocker1)
    const QSignalBlocker blocker2(&m_width); Q_UNUSED(blocker2)
    const QSignalBlocker blocker3(&m_color); Q_UNUSED(blocker3)

    m_style.setCurrentIndex(m_pen.style());
    m_width.setValue(m_pen.widthF());
    m_color.setColor(m_pen.color());
}

void PenFormWidget::onFieldsEdited()
{
    m_pen.setStyle((Qt::PenStyle)m_style.currentIndex());
    m_pen.setWidthF(m_width.value());
    m_pen.setColor(m_color.color());

    emit penEdited();
    emit penChanged();
}
