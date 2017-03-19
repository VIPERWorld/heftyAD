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
    std::function<void ()> m_destructor;
    std::function<void ()> m_undoActions;
    std::function<void ()> m_redoActions;
    std::string m_description;

public:
    ShortHandUndoCommand(std::function<void ()> destructor,
                         std::function<void ()> undoActions,
                         std::function<void ()> redoActions,
                         const std::string &description = "ShortHandUndoCommand");

    ~ShortHandUndoCommand();

    /**
     * Returns the description given to this class constructor.
     */
    std::string description() const noexcept override;

protected:
    void undoActions() override;
    void redoActions() override;
};
}

#endif // SHORTHANDUNDOCOMMAND_H
