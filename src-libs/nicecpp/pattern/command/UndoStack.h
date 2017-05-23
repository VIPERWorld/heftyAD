#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include "nicecpplib_global.h"

#include <stack>

namespace ncpp
{
class UndoCommand;

/**
 * This class stores undoable commands.
 * It contains all the commands executed on a given document
 * and can roll the document' state backwards or forwards by undoing or redoing them.
 *
 * /!\ Once a command is pushed, this stack takes ownership of it.
 *     So commands should be created using the operator new.
 *     Moreover a command should be pushed only once.
 */
class NICECPPLIBSHARED_EXPORT UndoStack
{
protected:
    std::stack<UndoCommand*> m_commandsToUndo;
    std::stack<UndoCommand*> m_commandsToRedo;

public:
    explicit UndoStack() = default;
    ~UndoStack();

    /**
     * Pushes the given command to the stack of commands to undo.
     * Thus the given command will be the next one to be undone when undo() is called.
     *
     * If there are undone commands when the given one is pushed, they are first deleted.
     */
    void push(UndoCommand *command);
    UndoCommand* nextCommandToUndo() const;
    UndoCommand* nextCommandToRedo() const;

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
