#ifndef ITERABLE_H
#define ITERABLE_H

/**
 * NO LONGER USED IN heftyAD.
 *
 * The Iterable class is an abstract class representing the concept of iterable objects.
 * Its core is owed to its run() function.
 *
 * For instance, an algorithm is iterable.
 */

class Iterable
{
public:
    explicit Iterable() = default;
    virtual ~Iterable() = default;

    /**
     * This function is called before this iterable starts iterating (see run()).
     * For instance, this function is the right place to initialize variables
     * that are involved in this iterable's iterations.
     */
    virtual void preIterations() = 0;
    /**
     * This function is called after all iterations have been played.
     */
    virtual void postIterations();

    /**
     * Tells whether there is an iteration to play.
     */
    virtual bool canIterate() const = 0;
    /**
     * Executes an iteration (the next one).
     */
    virtual void iterate() = 0;
    /**
     * This function is called after each call to iterate() in the run() method.
     * For example, this is where you have to increment a for-loop index.
     */
    virtual void postIterate() = 0;

    /**
     * Runs this iterable.
     * Here is a code summurizing what this function does:

            preIterations();

            while(canIterate()) {
                iterate();
                postIterate();
            }

            postIterations();
     */
    virtual void run();
};

#endif // ITERABLE_H
