#ifndef ITEMFONTFORM_H
#define ITEMFONTFORM_H

#include "ItemForm.h"

#include "widget/dataform/FontFormWidget.h"

class ItemFontForm : public ItemForm
{
    Q_OBJECT

protected:
    FontFormWidget m_font;

public:
    explicit ItemFontForm(QWidget *parent = 0);

    void retranslate() override;

signals:

public slots:
};

#endif // ITEMFONTFORM_H
