#ifndef ITEMCURVEFORM_H
#define ITEMCURVEFORM_H

#include "ItemForm.h"

class ItemCurveForm : public ItemForm
{
    Q_OBJECT

public:
    explicit ItemCurveForm(QWidget *parent = 0);

    void retranslate() override;

signals:

public slots:
};

#endif // ITEMCURVEFORM_H
