#ifndef SHORTHANDUNDOCOMMAND_H
#define SHORTHANDUNDOCOMMAND_H

#include "nicecpplib_global.h"

#include "UndoCommand.h"

#include <functional>

namespace ncpp
{
/**
 * The ShortHandUndoCommand class is a shorthand and convenient
 * class provided in order to make command creation easier.
 * See its constructor arguments for more information.
 */
class NICECPPLIBSHARED_EXPORT ShortHandUndoCommand : public UndoCommand
{
protected:
    std::function<void ()> m_destructorBody;
    std::function<void ()> m_undoBody;
    std::function<void ()> m_redoBody;
    std::string m_description;

public:
    ShortHandUndoCommand(std::function<void ()> destructorBody,
                         std::function<void ()> undoBody,
                         std::function<void ()> redoBody,
                         const std::string &description = "ShortHandUndoCommand");

    ~ShortHandUndoCommand();

    /**
     * Returns the description given to this class constructor.
     */
    std::string description() const override;
    void undo() override;
    void redo() override;
};
}

#endif // SHORTHANDUNDOCOMMAND_H
