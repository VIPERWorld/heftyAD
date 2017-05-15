#ifndef UTILITY_H
#define UTILITY_H

#include <QList>

class QAction;
class QFrame;
class QMenu;
class QPoint;
class QWidget;

class Utility
{
public:
    Utility() = delete;

    /**
     * Pops up the given menu at the given position (whithin the given source widget coordinate).
     */
    static QAction* execMenuAt(QMenu *menu, const QPoint &pos, QWidget *source);

    static QFrame* lineWidget();

    /**
     * Returns a list which contains all widgets at the given global screen position.
     */
    static QList<QWidget*> widgetsAt(const QPoint &pos);
};

#endif // UTILITY_H
