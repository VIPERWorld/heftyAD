#ifndef FONTFORMWIDGET_H
#define FONTFORMWIDGET_H

#include "DataFormWidget.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFontComboBox>
#include <QLabel>
#include <QToolButton>

class FontFormWidget : public DataFormWidget
{
    Q_OBJECT

private:
    QFont m_fontz;

    QLabel m_familyLabel; QFontComboBox m_family;
    QLabel m_pointSizeLabel; QDoubleSpinBox m_pointSize;
    QCheckBox m_bold;      QCheckBox m_italic;    QLabel m_space;
    QCheckBox m_strikeOut; QCheckBox m_underline; QCheckBox m_overline;
    QToolButton m_advanced;

public:
    explicit FontFormWidget(QWidget *parent = 0);

    QFont fontz() const;
    void setFontz(const QFont &fontz);

    void setAdvancedButtonIcon(const QIcon &icon);
    void setAdvancedButtonVisible(bool visible);

    void setFieldTexts1(const QString &familyText, const QString &pointSizeText);
    void setFieldTexts2(const QString &boldText, const QString &italicText);
    void setFieldTexts3(const QString &strikeOutText, const QString &underlineText, const QString &overlineText);

private:
    void updateFields();

signals:
    /**
     * This signal is emitted whenever the font changes, even programmatically.
     */
    void fontChanged();
    /**
     * This signal is emitted whenever the font is edited (i.e. whenever it changes excluded programmatically).
     */
    void fontEdited();

private slots:
    void onFieldsEdited();
    void onAdvancedButtonPressed();
};

#endif // FONTFORMWIDGET_H
