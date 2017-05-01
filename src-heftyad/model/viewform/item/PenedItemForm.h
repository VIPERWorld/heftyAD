#ifndef PENEDITEMFORM_H
#define PENEDITEMFORM_H

#include "ItemForm.h"

#include "widget/dataform/PenFormWidget.h"

class PenedItemForm : public ItemForm
{
    Q_OBJECT

protected:
    PenFormWidget m_pen;

public:
    explicit PenedItemForm(QWidget *parent = 0);

    void retranslate() override;

signals:

public slots:
};

#endif // PENEDITEMFORM_H
