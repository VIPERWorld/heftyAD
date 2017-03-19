#include "Algorithm.h"
#include "SimulationLocker.h"

Algorithm::Algorithm()
    : SimplyControllable(),
      m_model(nullptr),
      m_locker(nullptr),
      m_clarifier(nullptr),
      m_highlighter(nullptr)
{
}

bool Algorithm::requiresAModel() const {return false;}
bool Algorithm::hasAValidModel() const {return !requiresAModel();}

Model* Algorithm::model() const {return m_model;}
void Algorithm::setModel(Model *model) {m_model = model;}

void Algorithm::run()
{
    preExecute();
    execute();
    postExecute();

    /*
     * We do this since for instance:
     *     when stop() is called from the simulation controller,
     *     stopIfStopWallEnabled() may have not be called (by a person writing an algorithm).
     *     So we need to reset the values so that further executions of the same algorithm use new values.
     */
    resetAttrs();
}

void Algorithm::setLocker(SimulationLocker *locker) {m_locker = locker;}
void Algorithm::setClarifier(SimulationClarifier *clarifier) {m_clarifier = clarifier;}
void Algorithm::setHighlighter(SimulationHighlighter *highlighter) {m_highlighter = highlighter;}

int Algorithm::_sim(int duration) const
{
    return m_locker->adjusted(duration);
}

/*void Algorithm::register_clarifier_new_section(void)
{
    suspendOrStopIfRequired();
    m_clarifier->prepareNewSection();
    suspendOrStopIfRequired();
}

void Algorithm::register_classifier_shadow_message(int msgType, const QString &msg, int alinea)
{
    suspendOrStopIfRequired();
    m_clarifier->addShadowMessage(msgType, msg, alinea);
    suspendOrStopIfRequired();
}

void Algorithm::register_classifier_shadow_message_normal(const QString &msg, int alinea)
{
    register_classifier_shadow_message(SimulationClarifier::ShadowNormal, msg, alinea);
}

void Algorithm::register_classifier_shadow_message_info(const QString &msg, int alinea)
{
    register_classifier_shadow_message(SimulationClarifier::ShadowInformation, msg, alinea);
}

void Algorithm::register_classifier_message(const QString &msg, int alinea)
{
    suspendOrStopIfRequired();
    m_clarifier->addMessage(msg, alinea);
    suspendOrStopIfRequired();
}

void Algorithm::register_classifier_pause(long msecs)
{
    suspendOrStopIfRequired();
    m_clarifier->pauseFor(msecs);
    suspendOrStopIfRequired();
}*/
