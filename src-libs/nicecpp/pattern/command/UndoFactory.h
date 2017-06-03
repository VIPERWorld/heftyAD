#ifndef UNDOFACTORY_H
#define UNDOFACTORY_H

#include "nicecpplib_global.h"

#include <functional>

namespace ncpp
{
class UndoCommand;

/**
 * The UndoFactory class is the default factory provided for command creation.
 * It behaves like a command maker.
 * It can be used as base class for customized command factories.
 */
class NICECPPLIBSHARED_EXPORT UndoFactory
{
public:
    /*
     * These are what I first did.
     * But when using this library in heftyAD (via Qt Creator),
     *     in release mode: everything works fine.
     *     in debug mode:   I got the "undefined reference to <...>UndoFactory<...>" error.
     *                      The only workaround I found is to move to the alternative below,
     *                      which doesn't use the C++11 "default" keyword.
     */
    //UndoFactory() = default;
    //virtual ~UndoFactory() = default;

    UndoFactory();
    virtual ~UndoFactory();

    static UndoCommand* createShortHandUndoCommand(std::function<void ()> destructorBody,
                                                   std::function<void ()> undoBody,
                                                   std::function<void ()> redoBody,
                                                   const std::string &description);
};
}

#endif // UNDOFACTORY_H
