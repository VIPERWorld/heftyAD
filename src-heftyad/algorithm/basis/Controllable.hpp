#ifndef CONTROLLABLE_H
#define CONTROLLABLE_H

/**
 * The Controllable class is the commun interface representing the concept of controllable objects.
 * For instance, a thread may be controllable: so that it can be suspended (paused), resumed and stopped purposely.
 */

class Controllable
{
public:
    explicit Controllable() = default;
    virtual ~Controllable() = default;

    virtual bool isSuspended() const = 0;

    virtual void suspend() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;
};

#endif // CONTROLLABLE_H
