#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include "nicecpplib_global.h"

#include <stack>

namespace ncpp
{
class UndoCommand;
class UndoSequence;

/**
 * This class stores undoable commands.
 * It contains all the commands executed on a given document
 * and can roll the document' state backwards or forwards by undoing or redoing them.
 *
 * It owns two stacks:
 *     one for commands that are already done and can later be undone.
 *     the other one for commands that are undone and can later be (re)done.
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

private:
    std::stack<UndoSequence*> m_macros;

public:
    explicit UndoStack() = default;
    ~UndoStack();

    /**
     * Pushes the given command to the stack of commands-to-undo.
     * If there are commands-to-redo when the given one is pushed, they are first deleted.
     *
     * Once pushed, the given command will be the next one to be undone when undo() is called.
     * Moreover there is no need to alter the state of pushed commands from the outside of this class.
     * Indeed this stack will handle further UndoCommand::undo() and UndoCommand::redo() operations.
     * But the state of commands may be altered prior to push().
     */
    void push(UndoCommand *command);
    const UndoCommand* nextCommandToUndo() const;
    const UndoCommand* nextCommandToRedo() const;

    /**
     * Begins composition of a macro command.
     * A macro command is merely an instance of UndoSequence with the given shortDescriptionPrefix.
     * The created macro is not pushed to this stack.
     * But subsequent commands pushed on this stack will be appended to the macro' sub-commands until endMacro() is called.
     *
     * Calls to beginMacro() and endMacro() may be nested, but every call to beginMacro() must have a matching call to endMacro().
     * Plus, while a macro is being composed, make sure you only keep push()ing commands.
     * More accurately you shouldn't call undo() or redo() for instance.
     *
     * Note: That notion of macros is inspired by http://doc.qt.io/qt-4.8/qundostack.html#beginMacro.
     */
    void beginMacro(const std::string &shortDescriptionPrefix = std::string());
    /**
     * Ends composition of a macro command (the one previously begun and not yet ended).
     * Then pushes it to this stack.
     */
    void endMacro();

    /**
     * Tells whether there is a command to undo().
     */
    bool canUndo() const;
    /**
     * Undoes the previously (re)done command.
     * You Should call canUndo() prior to this function.
     */
    void undo();
    void undoAll();

    /**
     * Tells whether there is a command to redo().
     */
    bool canRedo() const;
    /**
     * Redoes the previously undone command.
     * You Should call canRedo() prior to this function.
     */
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

private:
    void pushDirectly(UndoCommand *command);
};
}

#endif // UNDOSTACK_H
