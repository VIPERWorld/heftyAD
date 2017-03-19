#include "ColorFormWidget.h"

#include <QColorDialog>
#include <QIntValidator>

ColorFormWidget::ColorFormWidget(QWidget *parent)
    : FormWidget(parent)
{
    // Add widgets

    addRow("rgb",   QWidgetList() << &m_r << &m_g << &m_b, Qt::Horizontal);
    addRow("hsv",   QWidgetList() << &m_h << &m_s << &m_v, Qt::Horizontal);
    addRow("alpha", &m_alpha);
    addRow("",      &m_visual);
    addRow("",      &m_advanced);

    // Customize widgets

    QList<QLineEdit*> list;
    list << &m_r << &m_g << &m_b << &m_h << &m_s << &m_v << &m_alpha << &m_visual;
    for(QLineEdit *edit : list) {
        edit->setContextMenuPolicy(Qt::NoContextMenu);
        edit->setAlignment(Qt::AlignCenter);
    }

    const int C0 = 0;
    const int C1 = 255;
    const int C2 = 359;
    QIntValidator *v1 = new QIntValidator(C0, C1, this);
    QIntValidator *v2 = new QIntValidator(C0, C2, this);

    m_r.setValidator(v1);       m_r.setToolTip(QString("%1-%2").arg(C0).arg(C1));
    m_g.setValidator(v1);       m_g.setToolTip(QString("%1-%2").arg(C0).arg(C1));
    m_b.setValidator(v1);       m_b.setToolTip(QString("%1-%2").arg(C0).arg(C1));
    //
    m_h.setValidator(v2);       m_h.setToolTip(QString("%1-%2").arg(C0).arg(C2));
    m_s.setValidator(v1);       m_s.setToolTip(QString("%1-%2").arg(C0).arg(C1));
    m_v.setValidator(v1);       m_v.setToolTip(QString("%1-%2").arg(C0).arg(C1));
    //
    m_alpha.setValidator(v1);   m_alpha.setToolTip(QString("%1-%2").arg(C0).arg(C1));
    //
    m_visual.setReadOnly(true);
    m_visual.setToolTip("#AARRGGBB");
    //
    m_advanced.setText("---");
    m_advanced.setCursor(Qt::PointingHandCursor);
    formLayout()->setAlignment(&m_advanced, Qt::AlignRight);

    // Register signals/slots connections

    connect(this, &ColorFormWidget::colorChanged, this, &ColorFormWidget::onColorChanged);
    //
    connect(&m_r, &QLineEdit::textEdited, this, &ColorFormWidget::onRgbEdited);
    connect(&m_g, &QLineEdit::textEdited, this, &ColorFormWidget::onRgbEdited);
    connect(&m_b, &QLineEdit::textEdited, this, &ColorFormWidget::onRgbEdited);
    //
    connect(&m_h, &QLineEdit::textEdited, this, &ColorFormWidget::onHsvEdited);
    connect(&m_s, &QLineEdit::textEdited, this, &ColorFormWidget::onHsvEdited);
    connect(&m_v, &QLineEdit::textEdited, this, &ColorFormWidget::onHsvEdited);
    //
    connect(&m_alpha, &QLineEdit::textEdited, this, &ColorFormWidget::onAlphaEdited);
    //
    connect(&m_advanced, &QToolButton::pressed,  this, &ColorFormWidget::onAdvancedButtonPressed);

    setColor(Qt::gray); // set initial color
}

void ColorFormWidget::setColor(const QColor &color)
{
    if(color.isValid() && m_color!=color) {
        m_color = color;
        emit colorChanged();
    }
}

void ColorFormWidget::setAdvancedButtonIcon(const QIcon &icon) {m_advanced.setIcon(icon);}

void ColorFormWidget::updateRgbTexts()
{
    m_r.setText(QString::number(m_color.red()));
    m_g.setText(QString::number(m_color.green()));
    m_b.setText(QString::number(m_color.blue()));
}

void ColorFormWidget::updateHsvTexts()
{
    m_h.setText(QString::number(m_color.hue()));
    m_s.setText(QString::number(m_color.saturation()));
    m_v.setText(QString::number(m_color.value()));
}

void ColorFormWidget::updateAlphaText()
{
    m_alpha.setText(QString::number(m_color.alpha()));
}

void ColorFormWidget::updateVisual()
{
    m_visual.setText(m_color.name(QColor::HexArgb));
    m_visual.setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);")
                           .arg(m_color.red()).arg(m_color.green()).arg(m_color.blue())
                           .arg(m_color.alpha()));
}

void ColorFormWidget::relevantUpdate()
{
    updateVisual();
    emit colorEdited();
}

void ColorFormWidget::onColorChanged()
{
    updateRgbTexts();
    updateHsvTexts();
    updateAlphaText();
    relevantUpdate();
}

void ColorFormWidget::onRgbEdited()
{
    m_color.setRed(QVariant(m_r.text()).toInt());
    m_color.setGreen(QVariant(m_g.text()).toInt());
    m_color.setBlue(QVariant(m_b.text()).toInt());

    updateHsvTexts();
    relevantUpdate();
}

void ColorFormWidget::onHsvEdited()
{
    const int h = QVariant(m_h.text()).toInt();
    const int s = QVariant(m_s.text()).toInt();
    const int v = QVariant(m_v.text()).toInt();
    m_color.setHsv(h, s, v, m_color.alpha());

    updateRgbTexts();
    relevantUpdate();
}

void ColorFormWidget::onAlphaEdited()
{
    m_color.setAlpha(QVariant(m_alpha.text()).toInt());
    relevantUpdate();
}

void ColorFormWidget::onAdvancedButtonPressed()
{
    const QColor color = QColorDialog::getColor(m_color, this, "", QColorDialog::ShowAlphaChannel);
    setColor(color);
}
