#include "ItemShapeForm.h"

ItemShapeForm::ItemShapeForm(QWidget *parent)
    : ItemForm(parent)
{
    // add widgets

    addRow(&m_shapeKindLabel, QWidgetList() << &m_shapeKind << &m_rounded, Qt::Vertical);
    addRow(&m_dim1Label, &m_dim1);
    addRow(&m_dim2Label, &m_dim2);

    // customize widgets

    const int M = 1e4;
    m_dim1.setRange(-M, M);
    m_dim2.setRange(-M, M);

    m_shapeKind.addItem("Circle");
    m_shapeKind.addItem("Rectangle");

    // connect signals to slots

    connect(&m_shapeKind, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ItemShapeForm::onCurrentShapeChanged);
}

void ItemShapeForm::retranslate()
{
    m_shapeKindLabel.setText(trUtf8("Forme"));
    m_rounded.setText(trUtf8("Arrondir la forme ?"));
    updateDimensionWidgets();
}

void ItemShapeForm::updateDimensionWidgets()
{
    switch(m_shapeKind.currentIndex()) {
    case 0: // circle
        m_dim1Label.setText(trUtf8("Diamètre"));

        m_dim2Label.setVisible(false);
        m_dim2.setVisible(false);
        break;

    case 1: // rectangle
        m_dim1Label.setText(trUtf8("Largeur"));

        m_dim2Label.setVisible(true);
        m_dim2Label.setText(trUtf8("Hauteur"));
        m_dim2.setVisible(true);
        break;
    }
}

void ItemShapeForm::onCurrentShapeChanged()
{
    updateDimensionWidgets();
}
