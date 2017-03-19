#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include "nicecpplib_global.h"

#include <stack>

namespace ncpp
{
class UndoCommand;

/**
 * This class stores undoable commands.
 * It contains all the commands executed on a given document and
 * can roll the document' state backwards or forwards by undoing or redoing them.
 *
 * /!\ Since this class takes ownership of the commands stored (it destroys them),
 *     each of them should be created using the operator new.
 */
class NICECPPLIBSHARED_EXPORT UndoStack
{
protected:
    std::stack<UndoCommand*> m_commandsToBeUndone;
    std::stack<UndoCommand*> m_commandsToBeRedone;

    /**
     * Indicates whether the latest call to undo() results in the stack
     * to have no more commands to undo.
     */
    bool m_bool;

public:
    explicit UndoStack();
    ~UndoStack();

    void push(UndoCommand *command);

    /**
     * Tells whether there is a command to undo().
     * Should be called before undo().
     */
    bool canUndo() const;
    void undo();
    void undoAll();

    /**
     * Tells whether there is a command to redo().
     * Should be called before redo().
     */
    bool canRedo() const;
    void redo();
    void redoAll();

    /**
     * Clears history.
     * Pending and Already-done commands are all destroyed.
     */
    void clear();

protected:
    /**
     * Destroys each command in the given command list.
     */
    static void destroyCommandsIn(std::stack<UndoCommand*> &commands);
};
}

#endif // UNDOSTACK_H
