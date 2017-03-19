#ifndef ARRAYMODELITEM_H
#define ARRAYMODELITEM_H

#include "ModelMultiShapeItem.h"

class ArrayModelItem : public ModelMultiShapeItem
{
    Q_OBJECT

public:
    explicit ArrayModelItem(QObject *parent = nullptr);
    ~ArrayModelItem();

public slots:
};

#endif // ARRAYMODELITEM_H
