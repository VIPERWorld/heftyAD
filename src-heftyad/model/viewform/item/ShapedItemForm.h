#ifndef SHAPEDITEMFORM_H
#define SHAPEDITEMFORM_H

#include "ItemForm.h"

class ShapedItemForm : public ItemForm
{
    Q_OBJECT

public:
    explicit ShapedItemForm(QWidget *parent = 0);

    void retranslate() override;

signals:

public slots:
};

#endif // SHAPEDITEMFORM_H
