#ifndef PENFORMWIDGET_H
#define PENFORMWIDGET_H

#include "ColorFormWidget.h"
#include "DataFormWidget.h"

#include "widget/GridGroupBox.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>

class PenFormWidget : public DataFormWidget
{
    Q_OBJECT

private:
    QPen m_pen;

    QLabel m_styleLabel; QComboBox m_style;
    QLabel m_widthLabel; QDoubleSpinBox m_width;
    GridGroupBox m_colorGroup;
        ColorFormWidget m_color;

public:
    explicit PenFormWidget(QWidget *parent = 0);

    QPen pen() const;
    void setPen(const QPen &pen);
    void setFieldTexts(const QString &styleText, const QString &widthText, const QString &colorTitle);

private:
    void updateFields();

signals:
    /**
     * This signal is emitted whenever the pen changes, even programmatically.
     */
    void penChanged();
    /**
     * This signal is emitted whenever the pen is edited (i.e. whenever it changes excluded programmatically).
     */
    void penEdited();

private slots:
    void onFieldsEdited();
};

#endif // PENFORMWIDGET_H
