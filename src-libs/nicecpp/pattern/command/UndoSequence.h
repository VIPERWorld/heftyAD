#ifndef UNDOSEQUENCE_H
#define UNDOSEQUENCE_H

#include "nicecpplib_global.h"

#include "UndoCommand.h"

#include <list>
#include <memory> // for std::unique_ptr

namespace ncpp
{
/**
 * The UndoSequence class, based on the Composite pattern,
 * represents a sequence of undoable commands.
 *
 * It represents commands which are composed of sub-commands.
 * Sub-commands can be registered either through the list passed to the constructor,
 * or via the push() function.
 *
 * In addition, depending on the argument given to this class' constructor,
 * it may take ownership of its sub-commands: this is the default behaviour
 * (so sub-commands will be deleted during this command destruction).
 */
class NICECPPLIBSHARED_EXPORT UndoSequence : public UndoCommand
{
private:
    std::list<UndoCommand*> m_subCommands;

    std::list<std::unique_ptr<UndoCommand>> m_ownerships;
    bool m_takeOwnershipOfSubCommands;

public:
    UndoSequence(bool takeOwnershipOfSubCommands = true);
    UndoSequence(const std::list<UndoCommand*> &subCommands, bool takeOwnerhipOfSubCommands = true);
    ~UndoSequence();

    std::string description() const noexcept override;

    /**
     * Undoes all sub-commands.
     */
    void undo() override;
    /**
     * Redoes all sub-commands.
     */
    void redo() override;

    /**
     * Registers the given command as a sub-command, only if it's not null.
     *
     * We highly recommend not to add the same command several times.
     * Indeed, if this command takes ownership of its children,
     * children deletion will lead to a crash.
     */
    void push(UndoCommand *cmd);

protected:
    void undoActions() override;
    void redoActions() override;
};
}

#endif // UNDOSEQUENCE_H
