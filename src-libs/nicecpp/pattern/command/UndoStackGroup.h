#ifndef UNDOSTACKGROUP_H
#define UNDOSTACKGROUP_H

#include "nicecpplib_global.h"

#include <list>

namespace ncpp
{
class UndoStack;

/**
 * Represents a group of UndoStack elements.
 *
 * This class is useful (and provided) for applications which allows multiple undo stacks,
 * one for each opened document.
 */

class NICECPPLIBSHARED_EXPORT UndoStackGroup
{
protected:
    std::list<UndoStack*> m_stacks;
    UndoStack *m_activeStack;

public:
    UndoStackGroup();
    ~UndoStackGroup() = default;

    std::list<UndoStack*> stacks() const noexcept;

    UndoStack* activeStack() const noexcept;
    /**
     * Sets the active stack of this group to the given stack.
     * Returns true if the given stack is either null or a member of this group (so it becomes the active stack),
     *         false otherwise.
     */
    bool setActiveStack(UndoStack *stack) noexcept;

    /**
     * Adds the given stack to this group (only if it's not null).
     * Note: This function doesn't check for duplicates.
     */
    void addStack(UndoStack *stack) noexcept;
    /**
     * Removes the given stack from this group.
     * If the removed stack were the active one, the active stack is set to null.
     */
    void removeStack(UndoStack *stack) noexcept;
    bool containsStack(UndoStack *stack) const noexcept;
};
}

#endif // UNDOSTACKGROUP_H
