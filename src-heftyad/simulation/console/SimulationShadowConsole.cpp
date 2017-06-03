#include "Resource.h"
#include "SimulationClarifier.h"
#include "SimulationShadowConsole.h"

#include "graphics/GraphicsPixmapItem.h"

#include <QGraphicsTextItem>
#include <QScrollBar>

SimulationShadowConsole::SimulationShadowConsole(QWidget *parent)
    : BasicGraphicsView(parent),
      m_clarifier(nullptr),
      m_next_section(0),
      m_next_row(0),
      m_max_col(0)
{
    scene()->setBackgroundBrush(QBrush(Qt::lightGray, Qt::DiagCrossPattern));

    connect(scene(), &QGraphicsScene::changed, this, &SimulationShadowConsole::onSceneChanged);
}

SimulationShadowConsole::~SimulationShadowConsole()
{
    removeItems();
}

void SimulationShadowConsole::setClarifier(SimulationClarifier *clarifier)
{
    if(m_clarifier) {
        disconnect(m_clarifier, 0, this, 0);
    }

    m_clarifier = clarifier;
    if(m_clarifier) {
        connect(m_clarifier, &SimulationClarifier::needNewSection,    this, &SimulationShadowConsole::onNewSectionNeeded);
        connect(m_clarifier, &SimulationClarifier::needMessage,       this, &SimulationShadowConsole::onMessageNeeded);
        connect(m_clarifier, &SimulationClarifier::needShadowMessage, this, &SimulationShadowConsole::onShadowMessageNeeded);
    }
}

void SimulationShadowConsole::removeLastSectionItems()
{
    QList<Item>::iterator it;
    for(it = m_items.end(); it != m_items.begin(); --it) {
        const auto item = *it;
        if(item.m_step == m_next_section) {
            removeItem(item);
        }
    }

    m_next_section--;
    updateMaxCol();
}

void SimulationShadowConsole::removeItems()
{
    setUpdatesEnabled(false);
    foreach(const Item &item, m_items) {
        scene()->removeItem(item.m_item);
        delete item.m_item;
    }
    m_items.clear();
    setUpdatesEnabled(true);

    // reset values

    m_next_section = 0;
    m_next_row = 0;

    m_max_col = 0;
}

void SimulationShadowConsole::addItem(const SimulationShadowConsole::Item &item, int alinea)
{
    auto xPos = 0;
    while(alinea-- > 0) {
        xPos += 30;
    }

    m_items.append(item);
    scene()->addItem(item.m_item);
    item.m_item->setPos(xPos, m_next_row);

    m_next_row += item.m_item->boundingRect().height();
    const auto right = item.m_item->x() + item.m_item->boundingRect().width();
    if(right > m_max_col) {
        m_max_col = right;
    }
}

void SimulationShadowConsole::removeItem(const SimulationShadowConsole::Item &item)
{
    scene()->removeItem(item.m_item);
    m_items.removeOne(item);

    m_next_row -= item.m_item->boundingRect().height();
    delete item.m_item;
}

void SimulationShadowConsole::updateMaxCol()
{
    m_max_col = 0;
    foreach(const Item &item, m_items) {
        if(!item.m_item->isVisible()) {
            continue;
        }

        const qreal right = item.m_item->x() + item.m_item->boundingRect().width();
        if(right > m_max_col) {
            m_max_col = right;
        }
    }
}

void SimulationShadowConsole::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    onSceneChanged();
}

void SimulationShadowConsole::onNewSectionNeeded()
{
    m_next_section++;

    if(!m_items.isEmpty()) {
        onMessageNeeded(""); // add separator (empty line)
    }
}

void SimulationShadowConsole::onMessageNeeded(const QString &msg, int alinea)
{
    auto *item = new QGraphicsTextItem(msg);
    item->setFont(QFont("Time News Roman", 15));

    const Item i = {item, m_next_section, alinea};
    addItem(i, alinea);
}

void SimulationShadowConsole::onShadowMessageNeeded(int msgType, const QString &msg, int alinea)
{
    auto *item = new GraphicsPixmapItem;

    QString image = "shadow.png";
    switch(msgType) {
        case SimulationClarifier::NormalText:                                    break;
        case SimulationClarifier::InformativeText: image = "shadow_info.png";    break;
        case SimulationClarifier::WarningText:     image = "shadow_warning.png"; break;
        case SimulationClarifier::ErrorText:       image = "shadow_error.png";   break;
    }

    item->setPixmapImage(Resource::instance().shadowFileName(image));

    const Item i = {item, m_next_section, alinea};
    addItem(i, alinea);

    if(!msg.isEmpty()) {
        onMessageNeeded("["+msg+"]", alinea);
    }
}

void SimulationShadowConsole::onSceneChanged(QList<QRectF> region)
{
    Q_UNUSED(region)

    setSceneRect(0, 0, m_max_col, m_next_row);
    if(!region.isEmpty()) {
        verticalScrollBar()->setValue(verticalScrollBar()->maximum());
    }
}
