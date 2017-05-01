#ifndef FONTEDITEMFORM_H
#define FONTEDITEMFORM_H

#include "ItemForm.h"

#include "widget/dataform/FontFormWidget.h"

class FontedItemForm : public ItemForm
{
    Q_OBJECT

protected:
    FontFormWidget m_font;

public:
    explicit FontedItemForm(QWidget *parent = 0);

    void retranslate() override;

signals:

public slots:
};

#endif // FONTEDITEMFORM_H
