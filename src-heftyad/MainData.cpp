#include "MainData.h"

ncpp::UndoStack* MainData::m_currentUndoStack = nullptr;

ncpp::UndoStack* MainData::currentUndoStack() {return m_currentUndoStack;}
void MainData::setCurrentUndoStack(ncpp::UndoStack *stack) {m_currentUndoStack = stack;}
