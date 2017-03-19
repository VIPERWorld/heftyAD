#include "ArrayModelItem.h"

ArrayModelItem::ArrayModelItem(QObject *parent)
    : ModelMultiShapeItem(parent)
{
    m_brush = QBrush(Qt::green);
}

ArrayModelItem::~ArrayModelItem()
{
}
