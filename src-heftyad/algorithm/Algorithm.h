#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "SimplyControllable.h"

class Model;
class SimulationClarifier;
class SimulationHighlighter;
class SimulationLocker;

/**
 * The Algorithm class is the base abstract class representing what an algorithm is.
 * An algorithm is defined as a controllable object.
 */

class Algorithm : public SimplyControllable
{
    Q_OBJECT

protected:
    Model *m_model;

    SimulationLocker *m_locker;
    SimulationClarifier *m_clarifier;
    SimulationHighlighter *m_highlighter;

public:
    explicit Algorithm();

    virtual bool requiresAModel() const;
    virtual bool hasAValidModel() const;

    Model* model() const;
    void setModel(Model *model);

    virtual void preExecute() = 0;
    virtual void execute() = 0;
    virtual void postExecute() = 0;
    virtual void run();

    void setLocker(SimulationLocker *locker);
    void setClarifier(SimulationClarifier *clarifier);
    void setHighlighter(SimulationHighlighter *highlighter);

protected:
    /**
     * Adjust the given duration (take acceleration value into account).
     */
    int _sim(int duration) const;

/*protected:
    void register_clarifier_new_section(void);
    void register_classifier_shadow_message(int msgType, const QString &msg = QString(), int alinea = 0);
    void register_classifier_shadow_message_normal(const QString &msg = QString(), int alinea = 0);
    void register_classifier_shadow_message_info(const QString &msg = QString(), int alinea = 0);
    void register_classifier_message(const QString &msg = QString(), int alinea = 0);

    void register_classifier_pause(long msecs);*/
};

#endif // ALGORITHM_H
