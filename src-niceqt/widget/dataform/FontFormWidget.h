#ifndef FONTFORMWIDGET_H
#define FONTFORMWIDGET_H

#include "widget/FormWidget.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFontComboBox>
#include <QToolButton>

class FontFormWidget : public FormWidget
{
    Q_OBJECT

private:
    QFont m_fontz;

    QFontComboBox m_family;
    QDoubleSpinBox m_pointSize;
    QCheckBox m_bold; QCheckBox m_italic; QCheckBox m_strikeOut; QCheckBox m_underline;
    QToolButton m_advanced;

public:
    explicit FontFormWidget(QWidget *parent = 0);

    QFont fontz() const;
    void setFontz(const QFont &fontz);

    void setAdvancedButtonIcon(const QIcon &icon);
    void setAdvancedButtonVisible(bool visible);

    void setFieldTexts1(const QString &family, const QString pointSize);
    void setFieldTexts2(const QString &bold, const QString &italic, const QString &strikeOut, const QString &underline);

private:
    void updateFields();

signals:
    void fontChanged();
    void fontEdited();

private slots:
    void onFieldsEdited();
    void onAdvancedButtonPressed();
};

#endif // FONTFORMWIDGET_H
