#ifndef ITEMFORM_H
#define ITEMFORM_H

#include "widget/FormWidget.h"

class ItemForm : public FormWidget
{
    Q_OBJECT

public:
    explicit ItemForm(QWidget *parent = 0);

    virtual void retranslate();

signals:

public slots:
};

#endif // ITEMFORM_H
