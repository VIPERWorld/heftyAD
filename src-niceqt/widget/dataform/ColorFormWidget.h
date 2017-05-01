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
    void setAdvancedButtonVisible(bool visible);

protected:
    void changeColor(const QColor &color, bool emitColorEdited);

private:
    void updateRgbTexts();
    void updateHsvTexts();
    void updateAlphaText();
    void updateVisual();

    void commonUpdate(bool emitColorEdited);
    void selectStaticColorIfNeeded();

signals:
    /**
     * This signal is emitted whenever the color changes, even programmatically.
     */
    void colorChanged();
    /**
     * This signal is emitted whenever the color is edited (i.e. whenever it changes excluded programmatically).
     */
    void colorEdited();

    void rgbEdited();
    void hsvEdited();
    void alphaEdited();

private slots:
    void onStaticColorChoosed();
    void onRgbEdited();
    void onHsvEdited();
    void onAlphaEdited();
    void onAdvancedButtonPressed();
};

#endif // COLORFORMWIDGET_H
