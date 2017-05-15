#include "Menu.h"

#include "utility/Utility.h"

Menu::Menu(QWidget *parent)
    : QMenu(parent)
{
}

void Menu::clearAndDontDelete()
{
    foreach(QAction *action, actions()) {
        removeAction(action);
    }
}

void Menu::setShortcuts()
{
}

void Menu::connectSignalsToSlots()
{
}

void Menu::retranslate()
{
}

QAction* Menu::execAt(const QPoint &pos, QWidget *source)
{
    return Utility::execMenuAt(this, pos, source);
}
