#ifndef NICECPPLIB_H
#define NICECPPLIB_H

#include "nicecpplib_global.h"

#include <string>

namespace ncpp
{
class NICECPPLIBSHARED_EXPORT Nicecpplib
{
public:
    Nicecpplib() = delete;
    Nicecpplib(const Nicecpplib&) = delete;
    Nicecpplib& operator=(const Nicecpplib&) = delete;
    Nicecpplib(const Nicecpplib&&) = delete;

    static std::string author(void);
    static std::string description(void);
};
}

#endif // NICECPPLIB_H
