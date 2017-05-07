#ifndef ITEMPENFORM_H
#define ITEMPENFORM_H

#include "ItemForm.h"

#include "widget/dataform/PenFormWidget.h"

class ItemPenForm : public ItemForm
{
    Q_OBJECT

protected:
    PenFormWidget m_pen;

public:
    explicit ItemPenForm(QWidget *parent = 0);

    void retranslate() override;

signals:

public slots:
};

#endif // ITEMPENFORM_H
