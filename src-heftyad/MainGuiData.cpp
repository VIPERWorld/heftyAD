#include "EditMenu.h"
#include "MainGuiData.h"
#include "Work.h"

#include "command/UndoStack.h"

#include <QObject>

EditMenu* MainGuiData::m_editMenu = nullptr;
//
Work* MainGuiData::m_currentWork = nullptr;
QMetaObject::Connection MainGuiData::m_currentWorkConn;

void MainGuiData::setEditMenu(EditMenu *menu) {m_editMenu = menu;}

Work* MainGuiData::currentWork() {return m_currentWork;}
void MainGuiData::setCurrentWork(Work *work)
{
    if(m_currentWork == work) {
        return;
    }

    if(m_currentWork) {
        QObject::disconnect(m_currentWorkConn);
    }

    m_currentWork = work;

    // Listen to undo redo changes if relevant

    m_editMenu->setUndoRedoEnabled(false, false);
    ncpp::UndoStack *stack = m_currentWork ? m_currentWork->undoStack() : nullptr;
    if(stack) {
        auto lambda = [stack]() {
            m_editMenu->setUndoRedoEnabled(stack->canUndo(), stack->canRedo());
        };

        m_currentWorkConn = QObject::connect(m_currentWork, &Work::dirtyChanged, lambda);
        lambda();
    }
}
