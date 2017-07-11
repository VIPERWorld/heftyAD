#ifndef MODEL_H
#define MODEL_H

#include <QBrush>
#include <QDebugStateSaver>
#include <QJSValue>
#include <QObject>
#include <QRectF>

class ModelReader;
class ModelWriter;

class QJSEngine;

class Model : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QBrush bgBrush READ backgroundBrush WRITE setBackgroundBrush NOTIFY backgroundBrushChanged)

protected:
    ModelReader *m_reader; // default reader
    ModelWriter *m_writer; // default writer

    QBrush m_bgBrush;

    int m_animationAcceleration;

private:
    // Only required for JavaScript binding to work
    QJSEngine *m_jsEngine;
    QMap<QObject*, QJSValue> m_jsItems;

public:
    explicit Model(QObject *parent = 0);

    QBrush backgroundBrush() const;
    void setBackgroundBrush(const QBrush &brush, bool notify = true);

    /**
     * Notifies any listener (a view for instance) about attribute changes.
     *
     * Since listeners are notified as attributes change,
     * this function is only useful when you want the listeners to be notified of changes they are not aware of.
     * For instance when an item is created listeners may not exist (and vice versa).
     */
    virtual void sendAttributeChanges();

    int animationAcceleration() const;
    void setAnimationAcceleration(int acc);

    void setJSEngine(QJSEngine *engine);

    Q_INVOKABLE virtual void saveState(int version = 0);
    Q_INVOKABLE virtual bool restoreState(int version = 0);
    Q_INVOKABLE virtual void discardStates();

    /**
     * This function is not named toString() since it may be exposed to JS environment and
     * calling toString() from JS code always default to Object.prototype.toString().
     * In addition we don't want to override Object.prototype.toString since
     * it's used by JS environment to generate errors.
     */
    Q_INVOKABLE virtual QString toStr() const;
    /**
     * Return the scene bounding rectangle that covers all items.
     */
    Q_INVOKABLE virtual QRectF coverage() const;

    Q_INVOKABLE virtual void empty();
    Q_INVOKABLE virtual void shuffle();
    Q_INVOKABLE virtual void layout();

protected:
    /**
     * Returns a proxy of the given item.
     * The given item is most likely meant to be of type ModelItem.
     */
    QJSValue getJSProxyOf(QObject *object);

public slots:
    bool saveTo(const QString &filePath, ModelWriter &writer);
    bool saveTo(const QString &filePath);

    bool loadFrom(const QString &filePath, ModelReader &reader);
    bool loadFrom(const QString &filePath);

signals:
    void backgroundBrushChanged();
};

inline std::ostream& operator<<(std::ostream &out, const Model &model) {
    out << model.toStr().toStdString();
    return out;
}
inline QDebug operator<<(QDebug debug, const Model &model) {
    QDebugStateSaver saver(debug);
    Q_UNUSED(saver)

    debug.nospace() << model.toStr();
    return debug;
}

#endif // MODEL_H
