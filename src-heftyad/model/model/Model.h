#ifndef MODEL_H
#define MODEL_H

#include <QBrush>
#include <QDebugStateSaver>
#include <QObject>
#include <QRectF>

class ModelReader;
class ModelWriter;

class Model : public QObject
{
    Q_OBJECT

protected:
    ModelReader *m_reader; // default reader
    ModelWriter *m_writer; // default writer

    QBrush m_bgBrush;

    int m_animationAcceleration;

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

    virtual void saveState(int version = 0);
    virtual bool restoreState(int version = 0);
    virtual void discardStates();

    virtual QString toString() const;
    /**
     * Return the scene bounding rectangle that covers all items.
     */
    virtual QRectF coverage() const;

    virtual void shuffle();
    virtual void layout();

public slots:
    bool saveTo(const QString &filePath, ModelWriter &writer);
    bool saveTo(const QString &filePath);

    bool loadFrom(const QString &filePath, ModelReader &reader);
    bool loadFrom(const QString &filePath);

signals:
    void backgroundBrushChanged();
};

inline std::ostream& operator<<(std::ostream &out, const Model &model) {
    out << model.toString().toStdString();
    return out;
}
inline QDebug operator<<(QDebug debug, const Model &model) {
    QDebugStateSaver saver(debug);
    Q_UNUSED(saver)

    debug.nospace() << model.toString();
    return debug;
}

#endif // MODEL_H
