#ifndef BASICGRAPHICSVIEW_H
#define BASICGRAPHICSVIEW_H

#include "HighlightableGraphicsView.h"

#include "command/UndoStack.h"

/**
 * The BasicGraphicsView class is the base class for all heftyAD GraphicsView subclasses.
 * It owns a scene and enables specific attributes such as:
 *      render hint QPainter::Antialiasing,
 *      scene interaction,
 *      ...
 *
 * It also owns an undo stack and register move commands when items are moved around.
 */
class BasicGraphicsView : public HighlightableGraphicsView
{
    Q_OBJECT

protected:
    ncpp::UndoStack m_undoStack;

private:
    bool m_dirty;

    QList<QPointF> m_onMousePressed_selectedItemsPos;
    /**
     * Indicates whether there is a move command to register (helps determine if an item has moved)
     */
    bool m_onMouseReleased_thereAreUnregisteredItemMoveCommands;

public:
    explicit BasicGraphicsView(QWidget *parent = 0);

    ncpp::UndoStack* undoStack() const;

    bool isDirty() const;
    void setDirty(bool dirty);

    QList<QGraphicsItem*> sceneSelectedItems() const;
    QList<QPointF> sceneSelectedItemPositions() const;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void registerMoveCommandIfAvailable();

signals:
    void dirtyChanged();
    void sceneSelectionChanged();

public slots:
};

#endif // BASICGRAPHICSVIEW_H
