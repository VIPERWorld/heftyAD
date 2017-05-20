#ifndef ITEMBRUSHFORM_H
#define ITEMBRUSHFORM_H

#include "ItemForm.h"

#include "widget/dataform/BrushFormWidget.h"

class ItemBrushForm : public ItemForm
{
    Q_OBJECT

private:
    BrushFormWidget m_brush;

public:
    explicit ItemBrushForm(QWidget *parent = 0);

    void retranslate() override;
    void clearFields() override;

protected:
    void registerItems() override;
    void unregisterItems() override;

signals:

private slots:
    void onItemBrushChanged();

    void onThisBrushChanged();
};

#endif // ITEMBRUSHFORM_H
