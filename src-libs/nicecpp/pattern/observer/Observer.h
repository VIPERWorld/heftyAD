#ifndef OBSERVER_H
#define OBSERVER_H

#include "nicecpplib_global.h"

class Observable;

namespace ncpp
{
/**
 * The Observer class ... Observer pattern. [To be completed]
 * Currently unused in heftyAD source code.
 */
class NICECPPLIBSHARED_EXPORT Observer
{
protected:
    Observable *m_observable;

public:
    Observer();
    virtual ~Observer() = default;

    Observable* observable() const;
    void setObservable(Observable *observable);

    virtual void update();
    virtual void update(int reason);
    virtual void update(char* reason);
};
}

#endif // OBSERVER_H
