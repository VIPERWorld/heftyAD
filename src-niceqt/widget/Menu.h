#ifndef MENU_H
#define MENU_H

#include <QMenu>

/**
 * The Menu class represents a generic menu.
 * It can be used in any Qt application.
 */

class Menu : public QMenu
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = 0);

    /**
     * Does the same job as QMenu::clear(). But never deletes the removed actions,
     * as opposed to what's stated in QMenu::clear() documentation.
     *
     * So this function merely iterates on this menu' actions
     * and removes any entry using QMenu::removeAction(QAction*).
     */
    void clearAndDontDelete();

    /**
     * This function is supposed to be overloaded and called in SUBCLASS' CONSTRUCTORS.
     * Its rule is to set shortcuts for each action added to this menu.
     * Its default implementation simply does nothing.
     */
    void setShortcuts();
    /**
     * This function is supposed to be overloaded and called in SUBCLASS' CONSTRUCTORS.
     * Its rule is to make all signal-slot binds within this menu's actions.
     * Its default implementation simply does nothing.
     */
    void connectSignalsToSlots();
    /**
     * This function is supposed to be overloaded and called in SUBCLASS' CONSTRUCTORS.
     * Its rule is to retranslate this menu.
     * Its default implementation simply does nothing.
     */
    void retranslate();

    QAction* execAt(const QPoint &pos, QWidget *source);

signals:

public slots:
};

#endif // MENU_H
