#include "ArrayForm.h"
#include "ArrayModel.h"
#include "ArrayModelItem.h"
#include "ArrayView.h"
#include "ArrayViewItem.h"

#include "command/UndoFactory.h"
#include "command/UndoStack.h"

#include <QApplication>
#include <QMouseEvent>

ArrayView::ArrayView(QWidget *parent)
    : View(parent),
      m_modelAsArray(nullptr)
{
    m_editionForm = std::move(std::unique_ptr<ViewForm>(new ArrayForm(this)));

    m_onMousePressedAddItem = false;
    m_visibleItemIndexesInArray = false;
}

ArrayView::~ArrayView()
{
    // ~QGraphicsScene() will remove and delete all items from the scene object.
}

bool ArrayView::acceptModel(Model *model) {return model==nullptr || dynamic_cast<ArrayModel*>(model);}

void ArrayView::mousePressEvent(QMouseEvent *event)
{
    View::mousePressEvent(event);

    const bool &isMouseLeftButtonPressed = (event->buttons() & Qt::LeftButton);
    if(!isMouseLeftButtonPressed) {
        return;
    }

    const QGraphicsItem *itemUnderMouse = itemUnderMouseEvent(event);
    if(itemUnderMouse) {
        return;
    }

    if(m_onMousePressedAddItem) {
        ArrayModelItem &item = m_modelAsArray->addNewItem();
        item.setPos(mapToScene(mapFromGlobal(event->globalPos())));
    }
}

void ArrayView::removeSelectedItems()
{
    /*
     * Gather data required for the undo/redo to work.
     * Note: The 1st for-loop is separated from the 2nd one since
     *       we want to get the indexes of the selected items before removing any of them from the model.
     */

    QList<ArrayModelItem*> selectedModelItems;
    QList<int> selectedModelItemIndexes;
    QList<ArrayViewItem*> selectedViewItems;
    for(QGraphicsItem *item : scene()->selectedItems()) {
        auto *viewItem = dynamic_cast<ArrayViewItem*>(item);
        auto *modelItem = dynamic_cast<ArrayModelItem*>(viewItem->modelItem());

        selectedModelItems.append(modelItem);
        selectedModelItemIndexes.append(m_modelAsArray->indexOf(*modelItem));
        selectedViewItems.append(viewItem);
    }

    /*
     * Remove selected items from the model.
     * Note: View items are not removed from the scene.
     *           They're just hidden and will later be deleted by QGraphicsScene.
     *       Model items aren't deleted either (since the array model handles that).
     */

    for(QGraphicsItem *item : scene()->selectedItems()) {
        auto *viewItem = dynamic_cast<ArrayViewItem*>(item);
        auto *modelItem = dynamic_cast<ArrayModelItem*>(viewItem->modelItem());

        m_modelAsArray->removeItem(*modelItem, false);
        viewItem->setParentItem(nullptr);
        viewItem->setVisible(false);
    }

    /*
     * Register undo/redo
     */

    if(selectedModelItems.isEmpty()) {
        return;
    }

    const bool dirty = isDirty();
    QString description = "Remove Array Items [" + QString::number(selectedModelItems.size()) + "]";
    m_undoStack.push(ncpp::UndoFactory::createCommand(
        []() {},
        /**
         * Undo Remove Items
         */
        [=]() {
            const int &s(selectedModelItems.size());

            // Add null items to make sure it'll always be possible to add the removed items to their respective index.
            for(int i=0; i<s; i++) {
                m_modelAsArray->m_array.add(nullptr);
            }

            // Add items back to the array
            for(int i=0; i<s; i++) {
                ArrayModelItem *modelItem = selectedModelItems.at(i);
                ArrayViewItem *viewItem = selectedViewItems.at(i);
                int index = selectedModelItemIndexes.at(i);

                m_modelAsArray->m_array.add(modelItem, index);
                viewItem->setVisible(true);
            }

            // Remove the null items added above
            m_modelAsArray->m_array.remove(nullptr);

            if(m_visibleItemIndexesInArray) {
                scene()->update(); // to make sure indexes (if painted) are well painted.
            }

            setDirty(dirty);
        },
        /**
         * Redo Remove Items
         */
        [=]() {
            for(int i=0, s=selectedModelItems.size(); i<s; i++) {
                ArrayModelItem *modelItem = selectedModelItems.at(i);
                ArrayViewItem *viewItem = selectedViewItems.at(i);

                m_modelAsArray->m_array.remove(modelItem);
                viewItem->setParentItem(nullptr);
                viewItem->setVisible(false);
            }

            setDirty(true);
        },
        description.toStdString()
    ));
    setDirty(true);
}

void ArrayView::showItemsAsInArray()
{
    const auto &centerItemsVertically = QApplication::keyboardModifiers() & Qt::ControlModifier;

    if(m_modelAsArray && !m_modelAsArray->isEmpty()) {
        const QPointF &start = m_modelAsArray->first().sceneRect().topLeft();
        m_modelAsArray->layout(start, centerItemsVertically);
    }
}

void ArrayView::shuffleItems()
{
    if(m_modelAsArray) {
        m_modelAsArray->shuffle();
        update(); // to make sure painted indexes are kept up do date
    }
}

void ArrayView::sortItemsBySceneRectX()
{
    if(m_modelAsArray) {
        m_modelAsArray->sortBySceneRectX();
        update(); // to make sure painted indexes are kept up do date
    }
}

void ArrayView::sortItemsByValue()
{
    if(m_modelAsArray) {
        m_modelAsArray->sortByValue();
        update(); // to make sure painted indexes are kept up do date
    }
}

void ArrayView::setOnMousePressedAddItem(bool value)
{
    m_onMousePressedAddItem = value;
    setDragMode(m_onMousePressedAddItem ? NoDrag : RubberBandDrag);
}

void ArrayView::setVisibleItemIndexesInArray(bool visible)
{
    m_visibleItemIndexesInArray = visible;

    for(QGraphicsItem *item : scene()->items()) {
        auto *arrayItem = dynamic_cast<ArrayViewItem*>(item);
        if(arrayItem) {
            arrayItem->setDrawItemIndexInArray(m_visibleItemIndexesInArray);
            arrayItem->update();
        }
    }
}

void ArrayView::onModelChanged()
{
    m_modelAsArray = dynamic_cast<ArrayModel*>(model());
    if(m_modelAsArray) {
        // First clear Undo/Redo (to do)
        // Then remove items from the scene and delete them
        // Create a view for any item in the model (NOTE: change this later so that we don't get Undo/Redo for each "add" action)
        for(ArrayModelItem *item : m_modelAsArray->items()) {
            onItemAddedToModel(item);
        }

        // Finally add new connections

        connect(m_modelAsArray, &ArrayModel::itemAdded,   this, &ArrayView::onItemAddedToModel);
        connect(m_modelAsArray, &ArrayModel::itemRemoved, this, &ArrayView::onItemRemovedFromModel);
        connect(m_modelAsArray, &ArrayModel::cleared,     this, &ArrayView::onModelCleared);
    }

    View::onModelChanged();
}

void ArrayView::onItemAddedToModel(ArrayModelItem *item)
{
    auto *itemView = new ArrayViewItem();
    itemView->setModelItem(item);
    itemView->setArrayContainer(m_modelAsArray); // for index painting

    scene()->addItem(itemView);

    itemView->setDrawItemIndexInArray(m_visibleItemIndexesInArray); // to make sure index will be painted if needed

    /*
     * Register undo/redo
     */

    const bool dirty = isDirty();
    m_undoStack.push(ncpp::UndoFactory::createCommand(
        /**
         * Destructor body
         */
        [=]() {
            // There is nothing to do since
            //     model item deletion is handled by the model
            // AND item views are kept in the scene until QGraphicsScene removes and destroys them (during its destruction).
        },
        /**
         * Undo Add Item
         */
        [=]() {
            m_modelAsArray->m_array.remove(item);
            itemView->setParentItem(nullptr);
            itemView->setVisible(false);

            setDirty(dirty);
        },
        /**
         * Redo Add Item
         */
        [=]() {
            m_modelAsArray->m_array.add(item);
            itemView->setVisible(true);
            itemView->setDrawItemIndexInArray(m_visibleItemIndexesInArray); // to make sure index will be painted if needed

            setDirty(true);
        },
        "Add Array Item"
    ));
    setDirty(true);
}

void ArrayView::onItemRemovedFromModel(ArrayModelItem *item, bool viewDeletionRequested)
{
    if(viewDeletionRequested) {
        ArrayViewItem *view = nullptr;
        for(QGraphicsItem *gi : items()) {
            view = dynamic_cast<ArrayViewItem*>(gi);
            if(view && view->modelItem()==item) {
                break;
            }
        }
        if(view) {
            scene()->removeItem(view);
            delete view;
        }
    }

    if(m_visibleItemIndexesInArray) {
        scene()->update(); // To make sure indexes are repainted
    }
}

void ArrayView::onModelCleared()
{
    // There is nothing to do since the array model handles model item deletion
    // and QGraphicsScene handles view item deletion.
}
