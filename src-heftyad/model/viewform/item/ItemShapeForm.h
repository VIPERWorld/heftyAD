#ifndef ITEMSHAPEFORM_H
#define ITEMSHAPEFORM_H

#include "ItemForm.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>

class ModelMultiShapeItem;

class ItemShapeForm : public ItemForm
{
    Q_OBJECT

    QLabel m_shapeKindLabel; QComboBox m_shapeKind; QCheckBox m_rounded;
    QLabel m_dim1Label; QDoubleSpinBox m_dim1;
    QLabel m_dim2Label; QDoubleSpinBox m_dim2;

public:
    explicit ItemShapeForm(QWidget *parent = 0);

    void retranslate() override;
    void clearFields() override;

protected:
    void registerItems() override;
    void unregisterItems() override;

private:
    void setDimensionFrom(ModelMultiShapeItem *item);

signals:

private slots:
    void updateDimensionWidgets();

    void onItemShapeKindChanged();
    void onItemRoundedChanged();
    void onItemDimensionChanged();

    void onThisShapeKindChanged();
    void onThisRoundedChanged();
    void onThisDimensionChanged();
};

#endif // ITEMSHAPEFORM_H
