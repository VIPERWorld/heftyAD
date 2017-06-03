#ifndef SIMULATIONSHADOWCONSOLE_H
#define SIMULATIONSHADOWCONSOLE_H

#include "graphics/BasicGraphicsView.h"

class SimulationClarifier;

class SimulationShadowConsole : public BasicGraphicsView
{
    Q_OBJECT

protected:
    typedef struct It {
        QGraphicsItem *m_item;
        int m_step;
        int m_alinea;

        bool operator==(const struct It &item) const
        {
            return m_item==item.m_item && m_step==item.m_step && m_alinea==item.m_alinea;
        }
    } Item;

protected:
    SimulationClarifier *m_clarifier;

    int m_next_section;
    qreal m_next_row;

    qreal m_max_col;

    QList<Item> m_items;

public:
    explicit SimulationShadowConsole(QWidget *parent = 0);
    ~SimulationShadowConsole();

    void setClarifier(SimulationClarifier *clarifier);

    void removeLastSectionItems();
    void removeItems();

protected:
    void addItem(const Item &item, int alinea);
    void removeItem(const Item &item);

    void updateMaxCol();

    void resizeEvent(QResizeEvent *event);

signals:

protected slots:
    void onNewSectionNeeded();

    void onMessageNeeded(const QString &msg = QString(), int alinea = 0);
    void onShadowMessageNeeded(int msgType, const QString &msg = QString(), int alinea = 0);

    void onSceneChanged(QList<QRectF> region = QList<QRectF>());
};

#endif // SIMULATIONSHADOWCONSOLE_H
