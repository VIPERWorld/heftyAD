#ifndef SINGLETON_H
#define SINGLETON_H

#include "nicecpplib_global.h"

namespace ncpp
{
/**
 * The Singleton class provides a common base for classes
 * which hold the same semantic as the Singleton design pattern.
 *
 * You can also use this class without templates. To do so, simply remove
 * the template attributes and replace the corresponding type-name by "Singleton".
 */
template <typename T>
class NICECPPLIBSHARED_EXPORT Singleton
{
protected:
    Singleton() = default;
    ~Singleton() = default;

public:
    /* Disable copy of this class (using C++11 syntax). */
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(const Singleton&&) = delete;

    /**
     * Returns a unique instance of T (presumably a class).
     *
     * This function is thread safe. Only the first access to the instance
     * is guaranteed thread safe because local static variable initialization is thread safe since C++11.
     */
    static T& instance()
    {
        static T singleton;
        return singleton;
    }
};
}

#endif // SINGLETON_H
