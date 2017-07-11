#include "BasicGraphicsView.h"

#include "command/UndoFactory.h"

#include <QGraphicsItem>
#include <QMouseEvent>
#include <QScrollBar>

BasicGraphicsView::BasicGraphicsView(QWidget *parent)
    : HighlightableGraphicsView(parent),
      m_undoRedoCountSinceLastSave(0),
      m_dirty(false),
      m_onMouseReleased_thereAreUnregisteredItemMoveCommands(false),
      m_involvedInASimulation(false)
{
    setScene(new QGraphicsScene(this));
    scene()->setSceneRect(-300, -300, 600, 600);
    connect(scene(), &QGraphicsScene::selectionChanged, this, &BasicGraphicsView::sceneSelectionChanged);

    horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);

    // --

//    setBackgroundBrush(QBrush(Qt::lightGray, Qt::CrossPattern));

//    QRadialGradient radialGrad(sceneRect().center(), 100);
//    radialGrad.setColorAt(0, Qt::lightGray); // Qt::red
//    radialGrad.setColorAt(0.5, Qt::blue);
//    radialGrad.setColorAt(1, Qt::green);
//    radialGrad.setSpread(QGradient::RepeatSpread);
//    setBackgroundBrush(QBrush(radialGrad));

//    QLinearGradient linearGrad(QPointF(100, 100), QPointF(200, 200));
//    linearGrad.setColorAt(0, Qt::lightGray); // Qt::black
//    linearGrad.setColorAt(1, Qt::white); // Qt::green
//    linearGrad.setSpread(QGradient::RepeatSpread);
//    setBackgroundBrush(QBrush(linearGrad));

    // --

    setRenderHint(QPainter::Antialiasing);

    setInteractive(true);
    setDragMode(RubberBandDrag);
    setRubberBandSelectionMode(Qt::ContainsItemShape);
}

BasicGraphicsView::~BasicGraphicsView()
{
    /*
     * Clearing scene isn't normally required but it currently solves the issue.
     * Indeed if selection was not cleared, the app crashes when a view is deleted from a simulation
     * (for instance when a simulation window is closed).
     * That said, nothing goes wrong when a model is created and then closed (even if there are seleceted items in the scene).
     * Should investigate later.
     */
    scene()->clearSelection();
}

ncpp::UndoStack* BasicGraphicsView::undoStack() const {return const_cast<ncpp::UndoStack*>(&m_undoStack);}

void BasicGraphicsView::resetUndoRedoCountSinceLastSave() {m_undoRedoCountSinceLastSave = 0;}

bool BasicGraphicsView::isDirty() const {return m_dirty;}
void BasicGraphicsView::setDirty(bool dirty)
{
    if(m_dirty != dirty) {
        m_dirty = dirty;
        emit dirtyChanged();
    }
}

QList<QGraphicsItem*> BasicGraphicsView::sceneSelectedItems() const {return scene()->selectedItems();}
QList<QPointF> BasicGraphicsView::sceneSelectedItemPositions() const
{
    QList<QPointF> res;
    for(QGraphicsItem *item : scene()->selectedItems()) {
        res.append(item->pos());
    }

    return res;
}

bool BasicGraphicsView::isInvolvedInASimulation() const {return m_involvedInASimulation;}
void BasicGraphicsView::setInvolvedInASimulation(bool involved)
{
    m_involvedInASimulation = involved;
    setInteractive(!m_involvedInASimulation);
}

void BasicGraphicsView::setDirtyFromCommandUndo()
{
    m_undoRedoCountSinceLastSave--;
    setDirty(m_undoRedoCountSinceLastSave != 0);
}

void BasicGraphicsView::setDirtyFromCommandRedo()
{
    m_undoRedoCountSinceLastSave++;
    setDirty(m_undoRedoCountSinceLastSave != 0);
}

void BasicGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    GraphicsView::contextMenuEvent(event);

    auto *itemUnderMouse = dynamic_cast<QGraphicsItem*>(itemUnderContextMenuEvent(event));
    if(!itemUnderMouse) {
        scene()->clearSelection();
    }
    else {
        if(!itemUnderMouse->isSelected()) {
            scene()->clearSelection();
            itemUnderMouse->setSelected(true);
        }
    }
}

void BasicGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    GraphicsView::mouseMoveEvent(event);

    QGraphicsItem *item = itemUnderMouseEvent(event);
    const bool mouseLeftButtonIsPressed = (event->buttons() & Qt::LeftButton);
    if(mouseLeftButtonIsPressed && item && item->isSelected()) {
        m_onMouseReleased_thereAreUnregisteredItemMoveCommands = true;
    }
}

void BasicGraphicsView::mousePressEvent(QMouseEvent *event)
{
    GraphicsView::mousePressEvent(event);

    // Disable interactions (for example RubberBandDrag mode): re-enable them on mouse released.
    const bool mouseLeftButtonIsPressed = (event->buttons() & Qt::LeftButton);
    if(!mouseLeftButtonIsPressed) {
        setInteractive(false);
        return;
    }

    // Save the position of each selected item before it is moved.
    m_onMousePressed_selectedItemsPos.clear();
    m_onMousePressed_selectedItemsPos = sceneSelectedItemPositions();
    if(event->modifiers() == Qt::ControlModifier) { // to make sure all items are taken into account
        m_onMousePressed_selectedItemsPos.append(itemUnderMouseEvent(event)->pos());
    }
}

void BasicGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    GraphicsView::mouseReleaseEvent(event);

    // Re-enable interactions (disabled on mouse pressed)
    setInteractive(!m_involvedInASimulation);

    // Register commands for selected items which has moved.

    registerMoveCommandIfAvailable();
    m_onMouseReleased_thereAreUnregisteredItemMoveCommands = false;
}

void BasicGraphicsView::registerMoveCommandIfAvailable()
{
    if(!m_onMouseReleased_thereAreUnregisteredItemMoveCommands) {
        return;
    }

    const QList<QGraphicsItem*> &items = sceneSelectedItems();
    const QList<QPointF>        &from  = m_onMousePressed_selectedItemsPos;
    const QList<QPointF>        &to    = sceneSelectedItemPositions();

    QString description = "Move Items [" + QString::number(items.count()) + "]";
    m_undoStack.push(ncpp::UndoFactory::createShortHandUndoCommand(
        []() {},
        /**
         * Undo Move Items
         */
        [=]() {
            const int itemsCount = items.count();
            for(int i=0; i<itemsCount; i++) {
                QGraphicsItem *item = items.at(i);
                item->setPos(from.at(i));
            }

            setDirtyFromCommandUndo();
        },
        /**
         * Redo Move Items
         */
        [=]() {
            const int itemsCount = items.count();
            for(int i=0; i<itemsCount; i++) {
                QGraphicsItem *item = items.at(i);
                item->setPos(to.at(i));
            }

            setDirtyFromCommandRedo();
        },
        description.toStdString()
    ));
    setDirtyFromCommandRedo();
}
