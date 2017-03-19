#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "nicecpplib_global.h"

#include <list>

namespace ncpp
{
class Observer;

/**
 * @brief The Observable class
 */
class NICECPPLIBSHARED_EXPORT Observable
{
protected:
    std::list<Observer*> m_observers;

public:
    Observable() = default;
    virtual ~Observable() = default;

    std::list<Observer*> observers() const;

    void attach(Observer *observer);
    void detach(Observer *observer);

    void notify();
    void notify(int reason);
    void notify(char *reason);
};
}

#endif // OBSERVABLE_H
