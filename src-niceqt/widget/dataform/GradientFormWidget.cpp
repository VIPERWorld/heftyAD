#include "GradientFormWidget.h"

GradientFormWidget::GradientFormWidget(QWidget *parent)
    : DataFormWidget(parent),
      m_gradient(nullptr)
{
    // add widgets

    addRow(&m_typeLabel, &m_type);

    // customize widgets

    setFieldTexts("type");

    m_type.addItems(QStringList() << "Linear" << "Radial" << "Conical" << "None");
    m_type.setCurrentIndex(3);
}

void GradientFormWidget::setFieldTexts(const QString &typeText)
{
    m_typeLabel.setText(typeText);
}
