#ifndef BRUSHEDITEMFORM_H
#define BRUSHEDITEMFORM_H

#include "ItemForm.h"

#include "widget/dataform/BrushFormWidget.h"

class BrushedItemForm : public ItemForm
{
    Q_OBJECT

private:
    BrushFormWidget m_brush;

public:
    explicit BrushedItemForm(QWidget *parent = 0);

    void retranslate() override;

signals:

public slots:
};

#endif // BRUSHEDITEMFORM_H
