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
    if(m_currentWork) {
        m_currentWorkConn = QObject::connect(m_currentWork, &Work::dirtyChanged, []() {
            ncpp::UndoStack *stack = m_currentWork->undoStack();
            m_editMenu->setUndoRedoEnabled(stack->canUndo(), stack->canRedo());
        });
    }
}
