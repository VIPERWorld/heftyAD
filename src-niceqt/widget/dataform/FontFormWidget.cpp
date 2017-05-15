#include "FontFormWidget.h"

#include <QFontDialog>

FontFormWidget::FontFormWidget(QWidget *parent)
    : DataFormWidget(parent)
{
    // add widgets

    addRow(&m_familyLabel,    &m_family);
    addRow(&m_pointSizeLabel, &m_pointSize);
    addRow(QWidgetList() << &m_bold      << &m_italic    << &m_space,    Qt::Horizontal);
    addRow(QWidgetList() << &m_strikeOut << &m_underline << &m_overline, Qt::Horizontal);

    addRow(&m_advanced);
    formLayout()->setAlignment(&m_advanced, Qt::AlignRight);

    // customize widgets

    setFieldTexts1("Family", "Size");
    setFieldTexts2("Bold", "Italic");
    setFieldTexts3("Strikeout", "Underline", "Overline");

    m_family.setContextMenuPolicy(Qt::NoContextMenu);
    m_pointSize.setContextMenuPolicy(Qt::NoContextMenu);
    m_pointSize.setMinimum(1.);

    m_advanced.setCursor(Qt::PointingHandCursor);
    m_advanced.setText("---");

    // connect signals/slots

    connect(&m_family,    static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),       this, &FontFormWidget::onFieldsEdited);
    connect(&m_pointSize, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &FontFormWidget::onFieldsEdited);

    connect(&m_bold,      &QCheckBox::released, this, &FontFormWidget::onFieldsEdited);
    connect(&m_italic,    &QCheckBox::released, this, &FontFormWidget::onFieldsEdited);

    connect(&m_strikeOut, &QCheckBox::released, this, &FontFormWidget::onFieldsEdited);
    connect(&m_underline, &QCheckBox::released, this, &FontFormWidget::onFieldsEdited);
    connect(&m_overline,  &QCheckBox::released, this, &FontFormWidget::onFieldsEdited);

    connect(&m_advanced, &QToolButton::pressed, this, &FontFormWidget::onAdvancedButtonPressed);
}

QFont FontFormWidget::fontz() const {return m_fontz;}
void FontFormWidget::setFontz(const QFont &fontz)
{
    m_fontz = fontz;
    updateFields();

    emit fontChanged();
}

void FontFormWidget::setAdvancedButtonIcon(const QIcon &icon) {m_advanced.setIcon(icon);}
void FontFormWidget::setAdvancedButtonVisible(bool visible) {m_advanced.setVisible(visible);}

void FontFormWidget::setFieldTexts1(const QString &familyText, const QString &pointSizeText)
{
    m_familyLabel.setText(familyText);
    m_pointSizeLabel.setText(pointSizeText);
}

void FontFormWidget::setFieldTexts2(const QString &boldText, const QString &italicText)
{
    m_bold.setText(boldText);
    m_italic.setText(italicText);
}

void FontFormWidget::setFieldTexts3(const QString &strikeOutText, const QString &underlineText, const QString &overlineText)
{
    m_strikeOut.setText(strikeOutText);
    m_underline.setText(underlineText);
    m_overline.setText(overlineText);
}

void FontFormWidget::updateFields()
{
    const QSignalBlocker blocker1(&m_family);    Q_UNUSED(blocker1)
    const QSignalBlocker blocker2(&m_pointSize); Q_UNUSED(blocker2)

    m_family.setCurrentText(m_fontz.family());
    m_pointSize.setValue(m_fontz.pointSizeF());

    m_bold.setChecked(m_fontz.bold());
    m_italic.setChecked(m_fontz.italic());

    m_strikeOut.setChecked(m_fontz.strikeOut());
    m_underline.setChecked(m_fontz.underline());
    m_overline.setChecked(m_fontz.overline());
}

void FontFormWidget::onFieldsEdited()
{
    m_fontz.setFamily(m_family.currentText());
    m_fontz.setPointSizeF(m_pointSize.value());

    m_fontz.setBold(m_bold.isChecked());
    m_fontz.setItalic(m_italic.isChecked());

    m_fontz.setStrikeOut(m_strikeOut.isChecked());
    m_fontz.setUnderline(m_underline.isChecked());
    m_fontz.setOverline(m_overline.isChecked());

    emit fontEdited();
    emit fontChanged();
}

void FontFormWidget::onAdvancedButtonPressed()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, m_fontz, this);
    if(ok) {
        font.setOverline(m_fontz.overline()); // just not to loose previous overline value
        setFontz(font); // will emit fontChanged();
        emit fontEdited();
    }
}
