#include "EditMenu.h"
#include "MainData.h"
#include "MainGuiData.h"

#include "command/UndoStack.h"

EditMenu::EditMenu(QWidget *parent)
    : Menu(parent)
{
    MainGuiData::setEditMenu(this);

    m_undo = addAction(QIcon(""), "");  m_undo->setEnabled(false);
    m_redo = addAction(QIcon(""), "");  m_redo->setEnabled(false);

    addSeparator();
    m_cut = addAction(QIcon(""), "");
    m_copy = addAction(QIcon(""), "");
    m_paste = addAction(QIcon(""), "");

    addSeparator();
    m_selectAll = addAction(QIcon(""), "");

    setShortcuts();
    connectSignalsToSlots();
}

void EditMenu::setUndoRedoEnabled(bool undo, bool redo)
{
    m_undo->setEnabled(undo);
    m_redo->setEnabled(redo);
}

void EditMenu::setShortcuts()
{
    m_undo->setShortcut(QKeySequence::Undo);
    m_redo->setShortcut(QKeySequence::Redo);

    m_cut->setShortcut(QKeySequence::Cut);
    m_copy->setShortcut(QKeySequence::Copy);
    m_paste->setShortcut(QKeySequence::Paste);

    m_selectAll->setShortcut(QKeySequence::SelectAll);
}

void EditMenu::connectSignalsToSlots()
{   
    connect(m_undo, &QAction::triggered, this, &EditMenu::onUndoActionTriggered);
    connect(m_redo, &QAction::triggered, this, &EditMenu::onRedoActionTriggered);

    connect(m_cut,   &QAction::triggered, this, &EditMenu::onCutActionTriggered);
    connect(m_copy,  &QAction::triggered, this, &EditMenu::onCopyActionTriggered);
    connect(m_paste, &QAction::triggered, this, &EditMenu::onPasteActionTriggered);

    connect(m_selectAll, &QAction::triggered, this, &EditMenu::onSelectAllActionTriggered);
}

void EditMenu::retranslate()
{
    setTitle("&"+trUtf8("Edition"));

    m_undo->setText(trUtf8("Annuler"));
    m_redo->setText(trUtf8("Refaire"));
    m_cut->setText(trUtf8("Couper"));
    m_copy->setText(trUtf8("Copier"));
    m_paste->setText(trUtf8("Coller"));
    m_selectAll->setText(trUtf8("Tout Sélectionner"));
}

#include <QDebug>
#include "command/UndoCommand.h"
void debug(const QString &text, ncpp::UndoCommand *cmd)
{
    qDebug() << text.toStdString().c_str() << QString(cmd->description().c_str());
}

void EditMenu::onUndoActionTriggered()
{
    ncpp::UndoStack *undoStack = MainData::currentUndoStack();
    if(!undoStack) {
        return;
    }

    if(undoStack->canUndo()) {
        debug("Undo", undoStack->nextCommandToUndo());
        undoStack->undo();
    }
    setUndoRedoEnabled(undoStack->canUndo(), undoStack->canRedo());
}

void EditMenu::onRedoActionTriggered()
{
    ncpp::UndoStack *undoStack = MainData::currentUndoStack();
    if(!undoStack) {
        return;
    }

    if(undoStack->canRedo()) {
        debug("Redo", undoStack->nextCommandToRedo());
        undoStack->redo();
    }
    setUndoRedoEnabled(undoStack->canUndo(), undoStack->canRedo());
}

void EditMenu::onCutActionTriggered()
{
}

void EditMenu::onCopyActionTriggered()
{
}

void EditMenu::onPasteActionTriggered()
{
}

void EditMenu::onSelectAllActionTriggered()
{
}
