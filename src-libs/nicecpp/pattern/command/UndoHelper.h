#ifndef UNDOHELPER_H
#define UNDOHELPER_H

#include "nicecpplib_global.h"

#include "UndoStackGroup.h"

namespace ncpp
{
/**
 * The UndoHelper class [To be completed]
 */
class NICECPPLIBSHARED_EXPORT UndoHelper
{
protected:
    UndoStackGroup m_undoStackGroup;

public:
    UndoHelper() = default;
    ~UndoHelper();

    /**
     * Creates a new command stack, adds it to the inner stack group and returns it.
     * This helper holds ownership of the returned stack.
     */
    UndoStack* registerNewUndoStack();
    UndoStack* undoStackAt(int index) const;
    UndoStack* activeUndoStack() const;
    bool setActiveUndoStack(UndoStack *stack);

    /**
     * Removes the stack at the given index and deletes it (if any).
     */
    void removeUndoStackAt(int index);
};
}

#endif // UNDOHELPER_H
