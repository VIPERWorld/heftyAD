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
    void clearFields() override;

protected:
    void registerItems() override;
    void unregisterItems() override;

signals:

private slots:
    void onItemFontChanged();

    void onThisFontChanged();
};

#endif // ITEMFONTFORM_H
