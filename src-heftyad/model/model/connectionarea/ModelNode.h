#ifndef MODELNODE_H
#define MODELNODE_H

#include "ModelMultiShapeItem.h"

class ModelNode : public ModelMultiShapeItem
{
    Q_OBJECT

public:
    ModelNode(QObject *parent = 0);
};

#endif // MODELNODE_H
