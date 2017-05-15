#ifndef ITEMFORM_H
#define ITEMFORM_H

#include "widget/FormWidget.h"

class ViewItem;

class ItemForm : public FormWidget
{
    Q_OBJECT

protected:
    QList<ViewItem*> m_items;

public:
    explicit ItemForm(QWidget *parent = 0);

    void setItems(const QList<ViewItem*> &items);
    virtual void retranslate();

protected:
    virtual void registerItems();
    virtual void unregisterItems();

signals:

public slots:
};

#endif // ITEMFORM_H
