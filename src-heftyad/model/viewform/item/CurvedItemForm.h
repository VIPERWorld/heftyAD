#ifndef CURVEDITEMFORM_H
#define CURVEDITEMFORM_H

#include "ItemForm.h"

class CurvedItemForm : public ItemForm
{
    Q_OBJECT

public:
    explicit CurvedItemForm(QWidget *parent = 0);

    void retranslate() override;

signals:

public slots:
};

#endif // CURVEDITEMFORM_H
