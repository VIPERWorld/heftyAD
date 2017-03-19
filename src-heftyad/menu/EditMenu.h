#ifndef EDITMENU_H
#define EDITMENU_H

#include "widget/Menu.h"

class EditMenu : public Menu
{
    Q_OBJECT

protected:
    QAction *m_undo;
    QAction *m_redo;
    QAction *m_cut;
    QAction *m_copy;
    QAction *m_paste;
    QAction *m_selectAll;

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
    void onCutActionTriggered();
    void onCopyActionTriggered();
    void onPasteActionTriggered();
    void onSelectAllActionTriggered();
};

#endif // EDITMENU_H
