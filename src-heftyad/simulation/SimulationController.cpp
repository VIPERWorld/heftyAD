#include "AlgorithmRunner.h"
#include "Resource.h"
#include "ShadowMessageBox.h"
#include "SimulationClarifier.h"
#include "SimulationController.h"
#include "SimulationLocker.h"
#include "View.h"

#include "others/ActionGroup.h"

#include <QThread>

SimulationController::SimulationController(QWidget *parent)
    : GridWidget(parent),
      m_algorithm(nullptr),
      m_algorithmRunner(nullptr),
      m_locker(nullptr),
      m_clarifier(nullptr),
      m_view(nullptr),
      m_simulation_should_restart(false),
      m_simulation_acceleration_onStop(-101) // we use an out of range acceleration value
{
    addWidget(&m_prev,         0, 0);
    addWidget(&m_resume,       0, 1);
    addWidget(&m_suspend,      0, 2);
    addWidget(&m_next,         0, 3);
    addWidget(&m_stop,         0, 4);
    addWidget(&m_restart,      0, 5);
    addWidget(&m_more,         0, 6);
    addWidget(&m_space,        0, 7);
    addWidget(&m_fullScreen,   0, 8);
    addWidget(&m_progressBar,  1, 0, 1, 9);
    addWidget(&m_acceleration, 2, 0, 1, 8);
    addWidget(&m_accelerationLCD, 2, 8, 1, 1);

    m_acceleration.setOrientation(Qt::Horizontal);
    m_acceleration.setMinimum(-100);
    m_acceleration.setMaximum(100);
    m_acceleration.setTickPosition(QSlider::TicksBelow);
    m_accelerationLCD.setDigitCount(4);
    onAccelerationValueChanged(0);

    m_fullScreen.setCheckable(true);

    m_space.setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred)); // get as much place as possible (horizontally)
    m_space.setAlignment(Qt::AlignCenter);
    //m_progressBar.setStyleSheet("background-color: pink;");

    m_prev.setAutoRepeat(true);
    m_next.setAutoRepeat(true);
    m_restart.setAutoRepeat(true);

    m_more.setMenu(&m_more_menu);
    m_ignore_pauseDuration = m_more_menu.addAction("");
        m_ignore_pauseDuration->setCheckable(true);
    m_minimize_pauseDuration = m_more_menu.addAction("");
        m_minimize_pauseDuration->setCheckable(true);

    m_actionGroup = new ActionGroup(this);
    m_actionGroup->addAction(m_minimize_pauseDuration);
    m_actionGroup->addAction(m_ignore_pauseDuration);
    m_actionGroup->setAcceptNoSelection(true);

    //setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum)); // can't remember why this were done

    connectSignalsToSlots();
    loadStateMachine();

    setIcons();
    retranslate();
}

void SimulationController::setAlgorithm(Algorithm *algorithm) {m_algorithm = algorithm;}
void SimulationController::setLocker(SimulationLocker *locker) {m_locker = locker; onAccelerationValueChanged2();}
void SimulationController::setClarifier(SimulationClarifier *clarifier) {m_clarifier = clarifier;}
void SimulationController::setSimulationView(View *view) {m_view = view; onAccelerationValueChanged2();}

void SimulationController::retranslate()
{
    m_prev.setToolTip(trUtf8("Retourner vers l'étape précédente"));
    m_suspend.setToolTip(trUtf8("Suspendre la simulation"));
    m_resume.setToolTip(trUtf8("Débuter/Continuer la simulation"));
    m_next.setToolTip(trUtf8("Avancer vers l'étape suivante"));
    m_stop.setToolTip(trUtf8("Arrêter la simulation"));
    m_more.setToolTip(trUtf8("Accéder à plus de fonctionnalités"));
        m_ignore_pauseDuration->setText(trUtf8("Ignorer les temps d'attente"));
        m_minimize_pauseDuration->setText(trUtf8("Minimiser les temps d'attente"));
    m_restart.setToolTip(trUtf8("Recommencer la simulation"));
    m_fullScreen.setToolTip(trUtf8("Passer en mode plein écran"));

    m_acceleration.setToolTip(trUtf8("Accélération des timers (en %age)"));
}

void SimulationController::setAlgorithmRunner(AlgorithmRunner *algorithmRunner)
{
    const auto badCond1 = m_algorithmRunner == algorithmRunner;
    const auto badCond2 = m_algorithmRunner && m_algorithmRunner->isRunning();
    if(badCond1 || badCond2) {
        return;
    }

    if(m_algorithmRunner) {
        m_algorithmRunner->disconnect(this);
    }

    m_algorithmRunner = algorithmRunner;
    if(m_algorithmRunner) {
        connect(m_algorithmRunner, &AlgorithmRunner::runFailsWith, this, &SimulationController::onAlgorithmRunnerFailsWith);

        connect(m_algorithmRunner, &AlgorithmRunner::resumed,   this, &SimulationController::onAlgorithmRunnerResumed);
        connect(m_algorithmRunner, &AlgorithmRunner::suspended, this, &SimulationController::onAlgorithmRunnerSuspended);
        connect(m_algorithmRunner, &AlgorithmRunner::stopped,   this, &SimulationController::onAlgorithmRunnerStopped);
        connect(m_algorithmRunner, &AlgorithmRunner::finished,  this, &SimulationController::onAlgorithmRunnerFinished);
        connect(m_algorithmRunner, &AlgorithmRunner::destroyed, this, &SimulationController::onAlgorithmRunnerDestroyed);
    }
}

void SimulationController::connectSignalsToSlots()
{
    connect(&m_resume,     &PushButton::pressed, this, &SimulationController::onResumeButtonPressed);
    connect(&m_suspend,    &PushButton::pressed, this, &SimulationController::onSuspendButtonPressed);
    connect(&m_stop,       &PushButton::pressed, this, &SimulationController::onStopButtonPressed);
    connect(&m_restart,    &PushButton::pressed, this, &SimulationController::onRestartButtonPressed);
    connect(&m_fullScreen, &PushButton::toggled, this, &SimulationController::onFullScreenButtonToggled);

    //connect(&m_prev,    &PushButton::pressed, this, &WorkSimulationController::onPrevButtonPressed);
    connect(&m_next,    &PushButton::pressed, this, &SimulationController::onNextButtonPressed);

    connect(m_actionGroup, SIGNAL(triggered(QAction*)), this, SLOT(onActionGroupTriggered(QAction*)));

    connect(&m_acceleration, &QSlider::valueChanged, this, &SimulationController::onAccelerationValueChanged);
}

void SimulationController::setIcons()
{
    m_prev.setIcon(Resource::instance().simulationIcon("previous.png"));
    m_suspend.setIcon(Resource::instance().simulationIcon("pause.png"));
    m_resume.setIcon(Resource::instance().simulationIcon("play.png"));
    m_next.setIcon(Resource::instance().simulationIcon("next.png"));
    m_stop.setIcon(Resource::instance().simulationIcon("stop.png"));
    m_restart.setIcon(Resource::instance().simulationIcon("restart.png"));
    m_more.setIcon(Resource::instance().simulationIcon("more.png"));
    //m_fullScreen.setIcon(Resource::instance().simulationIcon("close.png")); // to be changed
}

void SimulationController::loadStateMachine()
{
    m_prev.setVisible(false); // Just for the moment
    m_next.setVisible(false); // Just for the moment

    // This connection helps find out if the simulation should restart or not
    connect(&m_state_init, &QState::entered, this, &SimulationController::onStateMachineInitialStateEntered);

    // Add states

    m_stateMachine.addState(&m_state_init);
    m_stateMachine.addState(&m_state_running);
        m_state_resumed.setParent(&m_state_running);
        m_state_suspended.setParent(&m_state_running);

    // Set initial states

    m_stateMachine.setInitialState(&m_state_init);
    m_state_running.setInitialState(&m_state_resumed);

    // Register transitions

    m_state_init.addTransition(&m_resume, &PushButton::pressed, &m_state_running);
    m_state_running.addTransition(this, &SimulationController::simulationFinished, &m_state_init);

    m_state_resumed.addTransition(&m_suspend, &PushButton::pressed, &m_state_suspended);
    m_state_suspended.addTransition(&m_resume, &PushButton::pressed, &m_state_resumed);

    // Assign property changes for each state

    m_state_init.assignProperty(&m_resume,  "visible", true);
    m_state_init.assignProperty(&m_suspend, "visible", false);
    m_state_init.assignProperty(&m_stop,    "enabled", false);
    m_state_init.assignProperty(&m_restart, "enabled", false);
    m_state_init.assignProperty(&m_prev,    "enabled", false);
    m_state_init.assignProperty(&m_next,    "enabled", true);

    m_state_running.assignProperty(&m_stop,    "enabled", true);
    m_state_running.assignProperty(&m_restart, "enabled", true);

    m_state_resumed.assignProperty(&m_resume,  "visible", false);
    m_state_resumed.assignProperty(&m_suspend, "visible", true);
    m_state_resumed.assignProperty(&m_prev,    "enabled", false);
    m_state_resumed.assignProperty(&m_next,    "enabled", false);

    m_state_suspended.assignProperty(&m_resume,  "visible", true);
    m_state_suspended.assignProperty(&m_suspend, "visible", false);
    m_state_suspended.assignProperty(&m_prev,    "enabled", true); // to be changed
    m_state_suspended.assignProperty(&m_next,    "enabled", true); // to be changed

    // Start the machine

    m_stateMachine.start();
}

void SimulationController::onStateMachineInitialStateEntered()
{
    if(m_simulation_should_restart) {
        m_simulation_should_restart = false;
        m_resume.click();
    }
}

/*#include <QDebug>
#include <QTimer>
class MyThread : public QThread
{
    void run() override
    {
        QTimer *timer = new QTimer;
        timer->setInterval(0);
        connect(timer, &QTimer::timeout, [](){qDebug() << "timeout";});
        connect(this, &MyThread::finished, [timer](){timer->deleteLater();});
        timer->start();

        qDebug() << "   starts";
        exec();
    }
};*/

void SimulationController::onResumeButtonPressed()
{
    if(!m_algorithmRunner) {
        setAlgorithmRunner(new AlgorithmRunner(m_algorithm));
    }

    if(m_algorithmRunner->isRunning()) {
        m_algorithmRunner->resume();
        if(m_view) {
            m_view->resumeHighlighting();
        }
        if(m_locker) {
            m_locker->setSimulationSuspended(false);
        }
    }
    else {
        // Remove any item previously added

//        m_clarifier.removeItems();
//        m_clarifier.update();

        // Launch algorithm runner in a new thread

        if(m_view) {
            // Since it's allowed to use indefinitely looping highlithting data, any remaining timer should be stopped first
            m_view->stopHighlighting();
        }

        QThread* thread = new QThread;
        m_algorithmRunner->moveToThread(thread);
        connect(thread, &QThread::started, m_algorithmRunner, &::AlgorithmRunner::run);
        connect(m_algorithmRunner, &AlgorithmRunner::finished, thread, &QThread::quit);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();

        emit simulationStarted();
    }
}

void SimulationController::onSuspendButtonPressed()
{
    if(!m_algorithmRunner) {
        return;
    }

    m_algorithmRunner->suspend();
    if(m_view) {
        m_view->suspendHighlighting();
    }
    if(m_locker) {
        m_locker->setSimulationSuspended(true);
    }
}

void SimulationController::onStopButtonPressed()
{
    if(!m_algorithmRunner) {
        return;
    }

    setEnabled(false); // re-enabled when runner is destroyed
    m_simulation_acceleration_onStop = m_acceleration.value();
    m_acceleration.setValue(m_acceleration.maximum()); // to accelerate simulation

//    auto *clarifier =  m_clarifier.clarifier();
//    clarifier->setIgnoreClarificationRequests(true);
    m_algorithmRunner->resume(); // just in case if the simulation were suspended
    m_algorithmRunner->stop();
    if(m_view) {
        m_view->stopHighlighting();
    }
}

void SimulationController::onRestartButtonPressed()
{
    m_simulation_should_restart = true;
    m_stop.click();
}

void SimulationController::onFullScreenButtonToggled(bool checked)
{
    emit simulationFullScreenEnabled(checked);
}

void SimulationController::onNextButtonPressed()
{
    if(!m_algorithmRunner) {
        return;
    }

    if(!m_algorithmRunner->isRunning() || m_algorithmRunner->isSuspended()) {
        m_resume.click();
        return;
    }
}

void SimulationController::onActionGroupTriggered(QAction *action)
{
    Q_UNUSED(action)
//    auto *clarifier = m_clarifier.clarifier();

//    if(action == m_ignore_pauseDuration) {
//        clarifier->setMinimizePauseDuration(false);
//        clarifier->setIgnorePauseRequests(action->isChecked());
//        return;
//    }

//    if(action == m_minimize_pauseDuration) {
//        clarifier->setIgnorePauseRequests(false);
//        clarifier->setMinimizePauseDuration(action->isChecked());
//        return;
    //    }
}

void SimulationController::onAccelerationValueChanged(int value)
{
    m_accelerationLCD.display(value);
    onAccelerationValueChanged2();
}

void SimulationController::onAccelerationValueChanged2()
{
    const int &value(m_acceleration.value());

    if(m_view) {
        m_view->setHighlightingDataAcceleration(value);
    }
    if(m_locker) {
        m_locker->setSimulationAcceleration(value);
    }
}

void SimulationController::onAlgorithmRunnerFailsWith(int errorFlag, const QString &errorMessage)
{
    setEnabled(false); // re-enabled when runner is destroyed

    /*
     * We set a window incon to the message boxes shown below
     * since simulation window's icon change when it's maximized.
     */

    switch(errorFlag) {
    case AlgorithmRunner::AlreadyRunning: // won't never occurred
        break;
    case AlgorithmRunner::AlgorithmIsNull:
        ShadowMessageBox(this)
        .withIcon(QMessageBox::Critical)
        .withWindowIcon(Resource::instance().windowIcon())
        .withWindowTitle(trUtf8("La simulation ne peut démarrer"))
        .withText(trUtf8("Une simulation implique forcément un algorithme.\n"
                         "Merci de configurer votre simulation puis de recommencer."))
        .withStandardButtons(QMessageBox::Ok)
        .withDefaultButton(QMessageBox::Ok)
        .exec();
        break;
    case AlgorithmRunner::AlgorithmHasInvalidModel:
        ShadowMessageBox(this)
        .withIcon(QMessageBox::Critical)
        .withWindowIcon(Resource::instance().windowIcon())
        .withWindowTitle(trUtf8("La simulation a terminé immédiatement"))
        .withText(trUtf8("L'algorithme sollicité ne peut s'appliquer au modèle choisi.\n"
                         "Merci de choisir un modèle adéquat puis de recommencer."))
        .withStandardButtons(QMessageBox::Ok)
        .withDefaultButton(QMessageBox::Ok)
        .exec();
        break;

    case AlgorithmRunner::StdException:
        handleAlgorithmException(errorMessage);
        break;
    case AlgorithmRunner::UnknownException:
        handleAlgorithmException(trUtf8("Source de l'exception inconnue"));
        break;

    default: break;
    }
}

void SimulationController::handleAlgorithmException(const QString &message)
{
//    const auto *clarifier =  m_clarifier.clarifier();
//    clarifier->prepareNewSection();
//    clarifier->addShadowMessageError(trUtf8("Evénement indésirable"));
//    clarifier->addMessage(trUtf8("Arrêt inopiné suite à une exception"), 1);
//    clarifier->addMessage(message, 2);

    // à revoir

    const QString &str1(trUtf8("Evénement indésirable"));
    const QString &str2(trUtf8("Arrêt inopiné suite à une exception"));

    m_clarifier->prepareNewSection();
    m_clarifier->addShadowMessageError(str1);
    m_clarifier->addMessage(str2, 1);
    m_clarifier->addMessage(message, 2);

    m_clarifier->addText(""); // add new line (since texts may have been added previously)
    m_clarifier->addError(str1);
    m_clarifier->addError("    "+str2);
    m_clarifier->addError("        "+message);

    QFont font; font.setPointSize(15);
    m_view->addHighlightingText(5000, true, true,
                                "Evénement indésirable détecté.\nVoir consoles.", m_view->modelCoverage(), Qt::AlignLeft,
                                QPen(Qt::red), font,
                                Qt::NoPen, QBrush(Qt::green));
}

void SimulationController::onAlgorithmRunnerResumed()
{
    //m_simulationView.removeLastSectionItems();
}

void SimulationController::onAlgorithmRunnerSuspended()
{
    /*const auto *clarifier =  m_simulationView.clarifier();
    clarifier->prepareNewSection();
    clarifier->addShadowInfo(true, trUtf8("Evénement spécial"));
    clarifier->addTextInfo(trUtf8("Simulation suspendue"), 1);*/
}

void SimulationController::onAlgorithmRunnerStopped()
{
//    auto *clarifier = m_clarifier.clarifier();
//    clarifier->setIgnoreClarificationRequests(false);
//    clarifier->prepareNewSection();
//    clarifier->addShadowMessageNormal(trUtf8("Requête particulière"));
//    clarifier->addMessage(trUtf8("Simulation interrompue"), 1);

    // à revoir

    m_clarifier->prepareNewSection();
    m_clarifier->addShadowMessageNormal(trUtf8("Requête particulière"));
    m_clarifier->addMessage(trUtf8("Simulation interrompue"), 1);

    m_clarifier->addText("");
    m_clarifier->addText(trUtf8("Requête particulière"));
    m_clarifier->addText("    "+trUtf8("Simulation interrompue"));
}

void SimulationController::onAlgorithmRunnerFinished()
{
    m_algorithmRunner->deleteLater();
}

void SimulationController::onAlgorithmRunnerDestroyed()
{
    if(m_simulation_acceleration_onStop > -101) {
        m_acceleration.setValue(m_simulation_acceleration_onStop);
        m_simulation_acceleration_onStop = -101;
    }
    setEnabled(true);

    emit simulationFinished();
}
