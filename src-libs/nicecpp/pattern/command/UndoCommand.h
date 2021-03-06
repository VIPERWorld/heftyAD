#ifndef UNDOCOMMAND_H
#define UNDOCOMMAND_H

#include "nicecpplib_global.h"

#include <string>

namespace ncpp
{
/**
 * The UndoCommand class represents an abstract undoable command.
 * Such a command can be undone and redone purposely.
 * The undo() and redo() functions should both be implemented in subclasses,
 * in order to define the way the command is undone and (re)done.
 */
class NICECPPLIBSHARED_EXPORT UndoCommand
{
public:
    explicit UndoCommand() = default;
    virtual ~UndoCommand() = default;

    /**
     * Returns a preferably short description of this command.
     * The default implementation merely returns "UndoCommand".
     */
    virtual std::string shortDescription() const;
    /**
     * Returns a possibly long description of this command.
     * The default implementation merely returns shortDescription().
     */
    virtual std::string longDescription() const;

    virtual void undo() = 0;
    virtual void redo() = 0;
};
}

#endif // UNDOCOMMAND_H
