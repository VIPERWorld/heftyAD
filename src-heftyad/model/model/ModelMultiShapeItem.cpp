#include "ModelMultiShapeItem.h"

ModelMultiShapeItem::ModelMultiShapeItem(QObject *parent)
    : ModelItem(parent)
{
    m_shapeKind = Rectangle;
    m_rounded = false;

    m_dim1 = m_dim2 = 50.;
}

ModelMultiShapeItem::ShapeKind ModelMultiShapeItem::shapeKind() const {return m_shapeKind;}
void ModelMultiShapeItem::switchToShapeKind(int shapeKind, bool notify)
{
    bool validShapeKind = false;
    switch(shapeKind) {
    case Circle: case Rectangle:
        validShapeKind = true;
        break;
    }

    if(validShapeKind && m_shapeKind!=shapeKind) {
        m_shapeKind = (ShapeKind) shapeKind;
        if(notify) {
            emit shapeKindChanged();
        }
    }
}

bool ModelMultiShapeItem::isCircular() const {return m_shapeKind == Circle;}
bool ModelMultiShapeItem::isRectangular() const {return m_shapeKind == Rectangle;}

bool ModelMultiShapeItem::isRounded() const {return m_rounded;}
void ModelMultiShapeItem::setRounded(bool rounded, bool notify)
{
    if(m_rounded != rounded) {
        m_rounded = rounded;
        if(notify) {
            emit roundedChanged();
        }
    }
}

QVariant ModelMultiShapeItem::dimension() const
{
    switch(m_shapeKind) {
    case Circle:    return m_dim1;
    case Rectangle: return QSizeF(m_dim1, m_dim2);
    }

    return QVariant(); // returns an invalid QVariant
}

void ModelMultiShapeItem::setDimension(const QVariant &dim, bool notify)
{
    const auto &minDim = 20.;

    switch(m_shapeKind) {
    case Circle:
        if(!dim.canConvert<qreal>() || dim.toReal()<minDim) {
            return;
        }
        m_dim1 = dim.toReal();
        break;

    case Rectangle:
        if(!dim.canConvert<QSizeF>() || dim.toSizeF().width()<minDim || dim.toSizeF().height()<minDim) {
            return;
        }
        m_dim1 = dim.toSizeF().width();
        m_dim2 = dim.toSizeF().height();
        break;
    }

    if(notify) {
        emit dimensionChanged();
    }
}

void ModelMultiShapeItem::saveState(QVariantList &state) const
{
    ModelItem::saveState(state);

    state << m_shapeKind << m_rounded;
    state << m_dim1 << m_dim2;
}

int ModelMultiShapeItem::restoreState(const QVariantList &state)
{
    int i = ModelItem::restoreState(state);

    m_shapeKind = (ShapeKind) state[i++].toInt();
    m_rounded   = state[i++].toBool();

    m_dim1 = state[i++].toReal();
    m_dim2 = state[i++].toReal();

    return i;
}

QRectF ModelMultiShapeItem::rect() const
{
    // Short names for attributes
    const qreal &d1(m_dim1);
    const qreal &d2(m_dim2);

    switch(m_shapeKind) {
    case Circle:
        return QRectF(-d1/2, -d1/2, d1, d1); // d1: diameter

    case Rectangle:
        return QRectF(-d1/2, -d2/2, d1, d2); // d1: width AND d2: height
    }

    return QRectF();
}

QPainterPath ModelMultiShapeItem::shape() const
{
    QPainterPath path;

    switch(m_shapeKind) {
    case Circle:
        // The shape is a simple ellipse.
        // This ensures that you must click inside the node's elliptic shape in order to drag it around.
        path.addEllipse(boundingRect());
        break;

    case Rectangle:
        path.addRect(boundingRect());
        break;
    }

    return path;
}

QRectF ModelMultiShapeItem::rectToScene(const QRectF &rect) const
{
    /*
     * We use the following axiom: this item is centered around its origin (0, 0).
     * See rect().
     */

    const QRectF &r(rect); // rename rect
    return QRectF(m_pos - QPointF(r.width()/2, r.height()/2), r.size());
}

void ModelMultiShapeItem::sendAttributeChanges()
{
    ModelItem::sendAttributeChanges();

    emit shapeKindChanged();
    emit roundedChanged();
    emit dimensionChanged();
}
