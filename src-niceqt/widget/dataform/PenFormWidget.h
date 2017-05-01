#ifndef PENFORMWIDGET_H
#define PENFORMWIDGET_H

#include "ColorFormWidget.h"

#include "widget/FormWidget.h"
#include "widget/GridGroupBox.h"

#include <QComboBox>
#include <QDoubleSpinBox>

class PenFormWidget : public FormWidget
{
    Q_OBJECT

private:
    QPen m_pen;

    QComboBox m_style;
    QDoubleSpinBox m_width;
    GridGroupBox m_colorGroup;
        ColorFormWidget m_color;

public:
    explicit PenFormWidget(QWidget *parent = 0);

    QPen pen() const;
    void setPen(const QPen &pen);
    void setFieldTexts(const QString &style, const QString &width, const QString &color);

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
