#ifndef EDITMENU_H
#define EDITMENU_H

#include "widget/Menu.h"

class EditMenu : public Menu
{
    Q_OBJECT

protected:
    QAction *m_undo;
    QAction *m_redo;

public:
    explicit EditMenu(QWidget *parent = 0);

    void setUndoRedoEnabled(bool undo, bool redo);

    void setShortcuts();
    void connectSignalsToSlots();
    void retranslate();

signals:

public slots:
    void onUndoActionTriggered();
    void onRedoActionTriggered();
};

#endif // EDITMENU_H
