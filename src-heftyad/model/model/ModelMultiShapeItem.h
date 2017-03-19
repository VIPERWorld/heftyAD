#ifndef MODELMULTISHAPEITEM_H
#define MODELMULTISHAPEITEM_H

#include "ModelItem.h"

#include <QVariant>

/**
 * The ModelMultiShapeItem class represents an item which can be drawn using different shapes.
 * Contrary to what one may think, this class doesn't really support multiple shapes.
 * That is, at a given time, only one shape can be used.
 * So only switching between shapes is allowed.
 */
class ModelMultiShapeItem : public ModelItem
{
    Q_OBJECT

    Q_PROPERTY(ShapeKind shapeKind READ shapeKind WRITE switchToShapeKind NOTIFY shapeKindChanged)
    Q_PROPERTY(bool      rounded   READ isRounded WRITE setRounded        NOTIFY roundedChanged)

    Q_PROPERTY(QVariant dimension READ dimension WRITE setDimension NOTIFY dimensionChanged)

public:
    typedef enum {
        Circle,
        Rectangle
    } ShapeKind;

protected:
    ShapeKind m_shapeKind;
    bool m_rounded;

    /*
     * Possible dimensions
     * ===================
     *
     * Circle:      diameter    -> m_dim1
     * Rectangle:   width       -> m_dim1
     *              height      -> m_dim2
     */
    qreal m_dim1;
    qreal m_dim2;

public:
    explicit ModelMultiShapeItem(QObject *parent = 0);

    ShapeKind shapeKind() const;
    void switchToShapeKind(int shapeKind, bool notify = true);

    /*
     * These functions are provided solely for the convenience of the user.
     * There are not used in heftyAD source code.
     * Indeed, they provide the same information as shapeKind() which is more generic.
     */
    bool isCircular() const;
    bool isRectangular() const;

    bool isRounded() const;
    void setRounded(bool rounded, bool notify = true);

    QVariant dimension() const;
    void setDimension(const QVariant &dim, bool notify = true);

    void saveState(QVariantList &state) const override;
    int restoreState(const QVariantList &state) override;

    /**
     * Returns this item's effective rectangle (see doc in base class).
     *
     * This item's effective rectangle is a w*h sized rectangle centered around its origin (0, 0).
     * If this item is a circle, we will surely have w = h = d = 2*r.
     */
    QRectF rect() const override;
    QPainterPath shape() const override;
    QRectF rectToScene(const QRectF &rect) const override;

    void sendAttributeChanges() override;

signals:
    void shapeKindChanged();
    void roundedChanged();
    void dimensionChanged();

public slots:
};

#endif // MODELMULTISHAPEITEM_H
