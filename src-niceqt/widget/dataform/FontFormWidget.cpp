#include "FontFormWidget.h"

#include <QFontDialog>

FontFormWidget::FontFormWidget(QWidget *parent)
    : FormWidget(parent)
{
    // add widgets

    setFieldTexts1("Family", "Size");
    /*
     * Setting parent to null isn't normally needed but due to the use of clear() in setFieldTexts1 (previous instruction),
     * any further call to that function puts the combo box in a strange state: enabled, visible, but not clickable.
     * NB: if we remove the previous instruction,
     *     further calls to the culprit function work fine and setting parent to null is no longer required.
     */
    m_family.setParent(nullptr);
    m_pointSize.setParent(nullptr); // required for the same reason as above

    // customize widgets

    QWidgetList widgets = QWidgetList() << &m_family << &m_pointSize;
    for(auto *widget : widgets) {
        widget->setContextMenuPolicy(Qt::NoContextMenu);
    }

    setFieldTexts2("Bold", "Italic", "Strikeout", "Underline");
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

void FontFormWidget::setFieldTexts1(const QString &family, const QString pointSize)
{
    clear();
    addRow(family,    &m_family);
    addRow(pointSize, &m_pointSize);
    addRow(QWidgetList() << &m_bold << &m_italic << &m_underline << &m_strikeOut, Qt::Horizontal);

    addRow(&m_advanced);
    formLayout()->setAlignment(&m_advanced, Qt::AlignRight);
}

void FontFormWidget::setFieldTexts2(const QString &bold, const QString &italic, const QString &strikeOut, const QString &underline)
{
    m_bold.setText(bold);
    m_italic.setText(italic);
    m_strikeOut.setText(strikeOut);
    m_underline.setText(underline);
}

void FontFormWidget::updateFields()
{
    const bool f = m_family.blockSignals(true);
    const bool p = m_pointSize.blockSignals(true);

    m_family.setCurrentText(m_fontz.family());
    m_pointSize.setValue(m_fontz.pointSizeF());

    m_bold.setChecked(m_fontz.bold());
    m_italic.setChecked(m_fontz.italic());
    m_strikeOut.setChecked(m_fontz.strikeOut());
    m_underline.setChecked(m_fontz.underline());

    m_family.blockSignals(f);
    m_pointSize.blockSignals(p);
}

void FontFormWidget::onFieldsEdited()
{
    m_fontz.setFamily(m_family.currentText());
    m_fontz.setPointSizeF(m_pointSize.value());

    m_fontz.setBold(m_bold.isChecked());
    m_fontz.setItalic(m_italic.isChecked());
    m_fontz.setStrikeOut(m_strikeOut.isChecked());
    m_fontz.setUnderline(m_underline.isChecked());

    emit fontEdited();
    emit fontChanged();
}

void FontFormWidget::onAdvancedButtonPressed()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, fontz(), this);
    if(ok) {
        setFontz(font);
        emit fontEdited();
    }
}
