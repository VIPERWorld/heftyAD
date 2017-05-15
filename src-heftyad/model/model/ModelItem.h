#ifndef MODELITEM_H
#define MODELITEM_H

#include <QBrush>
#include <QDebugStateSaver>
#include <QFont>
#include <QObject>
#include <QPen>
#include <QPointF>

class ModelItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)

    Q_PROPERTY(bool  enabled  READ isEnabled  WRITE setEnabled  NOTIFY enabledChanged)
    Q_PROPERTY(bool  selected READ isSelected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(qreal opacity  READ opacity    WRITE setOpacity  NOTIFY opacityChanged)
    Q_PROPERTY(bool  visible  READ isVisible  WRITE setVisible  NOTIFY visibilityChanged)

    Q_PROPERTY(QBrush  brush         READ brushColor    WRITE setBrush         NOTIFY brushChanged)
    Q_PROPERTY(QColor  brushColor    READ brushColor    WRITE setBrushColor    NOTIFY brushChanged)
    Q_PROPERTY(QPixmap brushTexture  READ brushTexture  WRITE setBrushTexture  NOTIFY brushChanged)
    //
    Q_PROPERTY(QPen    pen           READ pen           WRITE setPen           NOTIFY penChanged)
    Q_PROPERTY(QColor  penColor      READ penColor      WRITE setPenColor      NOTIFY penChanged)
    Q_PROPERTY(int     penStyle      READ penStyle      WRITE setPenStyle      NOTIFY penChanged)
    Q_PROPERTY(qreal   penWidth      READ penWidth      WRITE setPenWidth      NOTIFY penChanged)
    //
    Q_PROPERTY(QFont   font          READ font          WRITE setFont          NOTIFY fontChanged)
    Q_PROPERTY(QString fontFamily    READ fontFamily    WRITE setFontFamily    NOTIFY fontChanged)
    Q_PROPERTY(qreal   fontPointSize READ fontPointSize WRITE setFontPointSize NOTIFY fontChanged)

    Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY posChanged)

protected:
    QString m_value;

    bool m_enabled;
    bool m_selected;
    qreal m_opacity;
    bool m_visible;

    QBrush m_brush;
    QPen   m_pen;
    QFont  m_font;

    QPointF m_pos;

    QPointF m_valueEditorPos;

public:
    explicit ModelItem(QObject *parent = 0);

    QString value() const;
    void setValue(const QString &value, bool notify = true);

    bool isEnabled() const;
    void setEnabled(bool enabled, bool notify = true);

    bool isSelected() const;
    void setSelected(bool selected, bool notify = true);

    qreal opacity() const;
    void setOpacity(qreal opacity, bool notify = true);

    bool isVisible() const;
    void setVisible(bool visible, bool notify = true);

    QBrush brush() const;
    QColor brushColor() const;
    QPixmap brushTexture() const;
    void setBrush(const QBrush &brush, bool notify = true);
    void setBrush(const QGradient &gradient, bool notify = true);
    void setBrushColor(const QColor &color, bool notify = true);
    void setBrushTexture(const QPixmap &pixmap, bool notify = true);

    QPen pen() const;
    QColor penColor() const;
    Qt::PenStyle penStyle() const;
    qreal penWidth() const;
    void setPen(const QPen &pen, bool notify = true);
    void setPenColor(const QColor &color, bool notify = true);
    void setPenStyle(int style, bool notify = true); // we use int for style to ensure it can be animated
    void setPenWidth(qreal width, bool notify = true);

    QFont font() const;
    QString fontFamily() const;
    qreal fontPointSize() const;
    void setFont(const QFont &font, bool notify = true);
    void setFontFamily(const QString &family, bool notify = true);
    void setFontPointSize(qreal pointSize, bool notify = true);

    QPointF pos() const;
    void setPos(const QPointF &pos, bool notify = true);
    void setPos(qreal x, qreal y, bool notify = true);
    void moveBy(qreal dx, qreal dy, bool notify = true);

    qreal x() const;
    void setX(qreal x, bool notify = true);

    qreal y() const;
    void setY(qreal y, bool notify = true);

    QPointF valueEditorPos() const;
    void setValueEditorPos(const QPointF &pos, bool notify = true);
    void setValueEditorPos(qreal x, qreal y, bool notify = true);

    /**
     * Notifies any listener (a view for instance) about attribute changes.
     *
     * Since listeners are notified as attributes change,
     * this function is only useful when you want the listeners to be notified of changes they are not aware of.
     * For instance when an item is created listeners may not exist (and vice versa).
     */
    virtual void sendAttributeChanges();

    /**
     * Saves the current state of this item to the given parameter.
     */
    virtual void saveState(QVariantList &state) const;
    /**
     * Restores the state of this item.
     * The given state parameter may not be the result of a previous call to saveState().
     * But it SHOULD contain enough fields so that this item's attributes are well initialized.
     * Plus, if the view watching this item doesn't get updated (because you're not interacting with it
     * and so it won't get painted), you may consider calling sendAttributeChanges().
     *
     * Note: The returned value doesn't bring any useful info you should know about.
     *       Anyway it tells subclasses the index at which they should start reading (from the given state).
     */
    virtual int restoreState(const QVariantList &state);

    /**
     * Returns an adjusted version of this item' pen width.
     * The returned value is such that the view watching this item
     * will have its edge slightly increased when selected.
     *
     * By default, the value returned by this function is
     *     pen().widthF() + C, where C = isSelected() ? 1.5 : 0.
     */
    virtual qreal scenePenWidth() const;

    /**
     * Returns this item' rectangle in LOCAL coordinates.
     *
     * Since we know nothing about this item' geometry, there's no way to compute its rectangle.
     * So this function is made pure virtual.
     *
     * Note: To compute this item' rectangle in subclasses, no stroke should be taken into account.
     *       Use boundingRect() if stroke is supposed to be taken into account.
     */
    virtual QRectF rect() const = 0;
    /**
     * Returns this item bounding rectangle in LOCAL coordinates,
     * which is an adjusted version of its rectangle (returned by rect()).
     *
     * This item' bounding rectangle is its rectangle,
     * adjusted by some units in all directions to compensate for its outline stroke.
     * Those units corresponds to a half of the value returned by extra().
     *
     * This function is not intend to be virtual,
     * since it provides the algorithm to compute the bounding rectangle of an item,
     * knowing its rectangle and the value returned by extra().
     */
    QRectF boundingRect() const;
    /**
     * Returns scenePenWidth() by default.
     * See boundingRect() to know how the value returned by this function is used.
     */
    virtual qreal extra() const;

    /**
     * Returns the shape of this item as a QPainterPath in LOCAL coordinates.
     * Default implementation:
           QPainterPath path;
           path.addRect(boundingRect());
           return path;
     *
     * If you want to check whether items intersect,
     * use sceneBoundingRect(), sceneRect() or the convenient intersects() function instead.
     */
    virtual QPainterPath shape() const;

    /**
     * Converts the given rectangle to SCENE coordinates and returns it.
     *
     * Since we know nothing about this item' geometry,
     * the default implementation merely returns the given rect parameter.
     */
    virtual QRectF rectToScene(const QRectF &rect) const;
    /**
     * Returns the scene rectangle of this item.
     * Put simply, it returns this item' rectangle in view' scene coordinnates.
     * That is, it merely returns rectToScene(rect()).
     */
    QRectF sceneRect() const;
    /**
     * Returns the scene bounding rectangle of this item.
     * This function merely returns rectToScene(boundingRect()).
     *
     * When the scene bounding rectangles of two items intersect, that means they collide.
     */
    QRectF sceneBoundingRect() const;

    /**
     * Tells whether this item collides with the given one.
     * This function is a shortcut for:
     *     this->sceneBoundingRect().intersects(item.sceneBoundingRect())
     *
     * The value returned by this function may not the the one expected,
     * since only the scene  bounding rectangle is taken into account.
     * For instance two edges will be said to collide as soon as their scene bounding rectangle collide.
     */
    bool intersects(const ModelItem &item);

signals:
    void valueChanged();

    void enabledChanged();
    void selectedChanged();
    void opacityChanged();
    void visibilityChanged();

    void brushChanged();
    void fontChanged();
    void penChanged();

    void posChanged();

    void valueEditorPosChanged();
};

inline bool operator< (const ModelItem &item1, const ModelItem &item2) {
    /*
     * We were obliged to convert string values to numbers since strings comparison behaves a little bit stranger,
     * due to numeric unicode value comparison. For instance: "10" < "100" < "25" since unicode(1) < unicode(2).
     *
     * We use QString::localeAwareCompare instead of QString::operator< since the later one may behave strangely:
     * read QString::compare documention.
     */

    const QString &val1(item1.value());
    bool ok1 = false; // is number
    const double &d1(val1.toDouble(&ok1));

    const QString &val2(item2.value());
    bool ok2 = false; // is number
    const double &d2(val2.toDouble(&ok2));

    return (ok1 && ok2) ? (d1 < d2) : (val1.localeAwareCompare(val2) < 0);
}
inline bool operator> (const ModelItem &item1, const ModelItem &item2) {return item2 < item1;}
inline bool operator<=(const ModelItem &item1, const ModelItem &item2) {return !(item1 > item2);}
inline bool operator>=(const ModelItem &item1, const ModelItem &item2) {return !(item1 < item2);}

inline bool operator==(const ModelItem &item1, const ModelItem &item2) {return item1.value() == item2.value();}
inline bool operator!=(const ModelItem &item1, const ModelItem &item2) {return !(item1 == item2);}

inline std::ostream& operator<<(std::ostream &out, const ModelItem &item) {
    out << item.value().toStdString();
    return out;
}
inline QDebug operator<<(QDebug debug, const ModelItem &item) {
    QDebugStateSaver saver(debug);
    Q_UNUSED(saver)

    debug.nospace() << item.value();
    return debug;
}

//inline std::istream& operator>>(std::istream &in, ModelItem &item) {
//    in >> item;
//    return in;
//}

#endif // MODELITEM_H
