#include "EditMenu.h"
#include "MainData.h"
#include "MainGuiData.h"

#include "command/UndoStack.h"

EditMenu::EditMenu(QWidget *parent)
    : Menu(parent)
{
    MainGuiData::setEditMenu(this);

    m_undo = addAction(QIcon(""), ""); m_undo->setEnabled(false);
    m_redo = addAction(QIcon(""), ""); m_redo->setEnabled(false);

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
}

void EditMenu::connectSignalsToSlots()
{   
    connect(m_undo, &QAction::triggered, this, &EditMenu::onUndoActionTriggered);
    connect(m_redo, &QAction::triggered, this, &EditMenu::onRedoActionTriggered);
}

void EditMenu::retranslate()
{
    setTitle("&"+trUtf8("Edition"));

    m_undo->setText(trUtf8("Annuler"));
    m_redo->setText(trUtf8("Refaire"));
}

#include <QDebug>
#include "command/UndoCommand.h"
void debug(const QString &text, const ncpp::UndoCommand *cmd)
{
//    qDebug() << text.toStdString().c_str() << QString(cmd->shortDescription().c_str());

    const QStringList &lines = QString(cmd->longDescription().c_str()).split("\n");
    const int len = lines.length();

    qDebug() << text.toStdString().c_str() << lines.first();
    for(int i=1; i<len; i++) {
        const QString &line = lines.at(i);
        qDebug() << line.toStdString().c_str();
    }
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
