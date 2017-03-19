#include "ModelNode.h"

ModelNode::ModelNode(QObject *parent)
    : ModelMultiShapeItem(parent)
{
    m_brush = QBrush(QColor(43, 140, 190));

    /*QLinearGradient linearGrad(QPointF(100, 100), QPointF(110, 110));
    linearGrad.setColorAt(0, Qt::lightGray); // Qt::black
    linearGrad.setColorAt(1, Qt::white); // Qt::green
    linearGrad.setSpread(QGradient::RepeatSpread);
    m_brush = QBrush(linearGrad);*/

    m_shapeKind = Circle;
}
