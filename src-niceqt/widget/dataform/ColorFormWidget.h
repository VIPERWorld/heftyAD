#ifndef COLORFORMWIDGET_H
#define COLORFORMWIDGET_H

#include "widget/FormWidget.h"

#include <QComboBox>
#include <QLineEdit>
#include <QToolButton>

class ColorFormWidget : public FormWidget
{
    Q_OBJECT

private:
    QColor m_color;

    QComboBox m_staticColors;
    QLineEdit m_r, m_g, m_b;
    QLineEdit m_h, m_s, m_v;
    QLineEdit m_alpha;
    QLineEdit m_visual;
    QToolButton m_advanced;

public:
    explicit ColorFormWidget(QWidget *parent = 0);

    QColor color() const;
    void setColor(const QColor &color);

    void setAdvancedButtonIcon(const QIcon &icon);

private:
    void selectStaticColorsIfNeeded();

    void updateRgbTexts();
    void updateHsvTexts();
    void updateAlphaText();
    void updateVisual();
    void commonUpdate();

signals:
    /**
     * This signal is emitted whenever setColor successfully changes the color.
     * Unlike colorEdited(), this signal is not emitted when the user initiates an edition,
     * unless that edition involves the color picker dialog or the static colors combobox.
     */
    void colorChanged();
    /**
     * This signal is emitted whenever the content of a field changes:
     *     either because setColor successfully changes the color
     *     or because the user initiates an edition.
     */
    void colorEdited();

    void rgbEdited();
    void hsvEdited();
    void alphaEdited();

private slots:
    void onColorChanged();

    void onStaticColorChoosed();
    void onRgbEdited();
    void onHsvEdited();
    void onAlphaEdited();
    void onAdvancedButtonPressed();
};

#endif // COLORFORMWIDGET_H
