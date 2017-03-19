#ifndef UNDOCOMMAND_H
#define UNDOCOMMAND_H

#include "nicecpplib_global.h"

#include <string>

namespace ncpp
{
/**
 * The UndoCommand class represents an abstract undoable command.
 * Such a command can be undone and redone purposely. Nevertheless,
 * the undoActions() and redoActions() functions should both be implemented in subclasses,
 * in order to define the way the command is undone and (re)done.
 *
 * When created, a command is supposed done. Thus, unless you undo it or
 * change its state using the setDone() function, any call to isDone() will return true.
 *
 * Plus, the canUndo() and the canRedo() functions help find out if a command is
 * allowed to be undone or redone. Both can be overridden but their default
 * implementations simply return isDone() and !isDone() respectively.
 * That is by default a command can be undone only if it was done
 * and it can be (re)done only if it was not done.
 *
 * Why are commands supposed done at creation?
 * ===========================================
 * In some gui libraries, ...
 * Item Move in Qt for instance, using the QGraphicsItem::ItemIsMovable flag.
 * [To be completed]
 */
class NICECPPLIBSHARED_EXPORT UndoCommand
{
private:
    bool m_done;

public:
    UndoCommand();
    virtual ~UndoCommand() = default;

    bool isDone() const noexcept;
    void setDone(bool done) noexcept;

    /**
     * Returns a preferably short description of this command.
     * Its default implementation merely returns "Undoable Command".
     */
    virtual std::string description() const noexcept;

    /**
     * Indicates whether this command can be undone.
     * Its default implementation merely returns isDone().
     */
    virtual bool canUnDo() noexcept;
    /**
     * Indicates whether this command can be redone.
     * Its default implementation merely returns !isDone().
     */
    virtual bool canRedo() noexcept;

    /**
     * Undoes this command if it can be undone (i.e. canUndo() returns true).
     * After a call to this function, isDone() will return false.
     */
    virtual void undo();
    /**
     * Redoes this command if it can be redone (i.e. canRedo() returns true).
     * After a call to this function, isDone() will return true.
     */
    virtual void redo();

protected:
    /**
     * Specifies how this command is undone.
     */
    virtual void undoActions() = 0;
    /**
     * Specifies how this command is redone.
     */
    virtual void redoActions() = 0;
};
}

#endif // UNDOCOMMAND_H
