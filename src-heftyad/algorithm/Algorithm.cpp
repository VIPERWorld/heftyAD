#include "Algorithm.h"
#include "SimulationClarifier.h"
#include "SimulationLocker.h"
#include "SimulationHighlighter.h"

Algorithm::Algorithm()
    : SimplyControllable(),
      m_model(nullptr),
      m_locker(nullptr),
      m_clarifier(nullptr),
      m_highlighter(nullptr)
{
    /*
     * The following objects may be exposed to script environment (see JavaScript algorithms).
     * Then they are set up with a parent so that they won't get deleted by script engine.
     */
    m_locker = new SimulationLocker(this);
    m_clarifier = new SimulationClarifier(this);
    m_highlighter = new SimulationHighlighter(this);
}

bool Algorithm::requiresAModel() const {return false;}
bool Algorithm::hasAValidModel() const {return !requiresAModel();}

Model* Algorithm::model() const {return m_model;}
void Algorithm::setModel(Model *model)
{
    if(m_model != model) {
        m_model = model;
        emit modelChanged();
    }
}

void Algorithm::run()
{
    preExecute();
    execute();
    postExecute();

    /*
     * We reset base class attributes since for instance:
     *     when stop() is called from the simulation controller,
     *     stopIfStopWallEnabled() may have not been called (by a person subclassing this class to create a custom algorithm).
     *     So we need to reset the values so that further executions of the same algorithm use new values.
     */
    resetAttrs();
}

SimulationLocker* Algorithm::locker() const {return m_locker;}
SimulationClarifier* Algorithm::clarifier() const {return m_clarifier;}
SimulationHighlighter* Algorithm::highlighter() const {return m_highlighter;}

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
