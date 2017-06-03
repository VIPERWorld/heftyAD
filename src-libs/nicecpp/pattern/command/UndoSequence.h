#ifndef UNDOSEQUENCE_H
#define UNDOSEQUENCE_H

#include "nicecpplib_global.h"

#include "UndoCommand.h"

#include <list>
#include <memory> // for std::unique_ptr

namespace ncpp
{
/**
 * The UndoSequence class implements the Composite pattern and defines a sequence of undoable commands.
 * It represents commands which are composed of sub-commands.
 * Sub-commands can be registered via the convenient push() functions.
 *
 * This command may also take ownership of sub-commands, in which case it will delete them upon destruction.
 */
class NICECPPLIBSHARED_EXPORT UndoSequence : public UndoCommand
{
private:
    std::string m_shortDescriptionPrefix;

    std::list<UndoCommand*> m_subCommands;
    std::list<std::unique_ptr<UndoCommand>> m_ownerships;

public:
    UndoSequence() = default;
    UndoSequence(const std::string &shortDescriptionPrefix);
    ~UndoSequence();

    /**
     * Registers the given command as a sub-command, only if it's not null.
     *
     * We highly recommend not to add the same command several times.
     * Indeed in the case this command takes ownership of its children,
     * children deletion will lead to a crash since the same child command will be deleted more than once.
     */
    void push(UndoCommand *cmd, bool takeOwnerhipOfCommand = true);
    void push(const std::list<UndoCommand*> &subCommands, bool takeOwnerhipOfSubCommands = true);

    std::string shortDescription() const override;
    std::string longDescription() const override;

    /**
     * Undoes all sub-commands.
     */
    void undo() override;
    /**
     * Redoes all sub-commands.
     */
    void redo() override;
};
}

#endif // UNDOSEQUENCE_H
