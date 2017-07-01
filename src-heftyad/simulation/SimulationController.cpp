#include "AlgorithmRunner.h"
#include "Resource.h"
#include "ShadowMessageBox.h"
#include "SimulationClarifier.h"
#include "SimulationController.h"
#include "SimulationLocker.h"
#include "View.h"

#include "graphics/highlighting/HighlightingTextData.h"

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
    m_hideProgressBar = m_more_menu.addAction(""); m_hideProgressBar->setCheckable(true);
    m_blockClarifier = m_more_menu.addAction(""); m_blockClarifier->setCheckable(true);

    //setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum)); // can't remember why this were done

    connectSignalsToSlots();
    loadStateMachine();
    setIcons();
}

void SimulationController::setAlgorithm(Algorithm *algorithm) {m_algorithm = algorithm;}
void SimulationController::setLocker(SimulationLocker *locker) {m_locker = locker; useAccelerationValue();}
void SimulationController::setClarifier(SimulationClarifier *clarifier) {m_clarifier = clarifier;}
void SimulationController::setSimulationView(View *view) {m_view = view; useAccelerationValue();}

void SimulationController::retranslate()
{
    m_prev.setToolTip(trUtf8("Retourner vers l'étape précédente"));
    m_suspend.setToolTip(trUtf8("Suspendre"));
    m_resume.setToolTip(trUtf8("Débuter/Continuer"));
    m_next.setToolTip(trUtf8("Avancer vers l'étape suivante"));
    m_stop.setToolTip(trUtf8("Arrêter"));
    m_more.setToolTip(trUtf8("Plus de fonctionnalités"));
        m_hideProgressBar->setText(trUtf8("Cacher la barre de progression"));
        m_blockClarifier->setText(trUtf8("Bloquer le Clarificateur"));
    m_restart.setToolTip(trUtf8("Recommencer"));
    m_fullScreen.setToolTip(trUtf8("Plein écran"));

    m_acceleration.setToolTip(trUtf8("Accélération des timers (en %age)"));
}

void SimulationController::setAlgorithmRunner(AlgorithmRunner *algorithmRunner)
{
    const bool badCond1 = m_algorithmRunner == algorithmRunner;
    const bool badCond2 = m_algorithmRunner && m_algorithmRunner->isRunning();
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

    //connect(&m_prev,       &PushButton::pressed, this, &WorkSimulationController::onPrevButtonPressed);
    connect(&m_next,       &PushButton::pressed, this, &SimulationController::onNextButtonPressed);

    connect(&m_more_menu, &QMenu::triggered, this, &SimulationController::onMoreMenuTriggered);

    connect(&m_acceleration, &QSlider::valueChanged, this, &SimulationController::onAccelerationValueChanged);
}

void SimulationController::setIcons()
{
    m_prev.setIcon(Resource::simulationIcon("previous.png"));
    m_suspend.setIcon(Resource::simulationIcon("pause.png"));
    m_resume.setIcon(Resource::simulationIcon("play.png"));
    m_next.setIcon(Resource::simulationIcon("next.png"));
    m_stop.setIcon(Resource::simulationIcon("stop.png"));
    m_restart.setIcon(Resource::simulationIcon("restart.png"));
    m_more.setIcon(Resource::simulationIcon("more.png"));
    m_fullScreen.setIcon(Resource::simulationIcon(".png")); // to be changed
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
        // First clear clarifier

        if(m_clarifier) {
            m_clarifier->removeTexts();
        }

        // Since it is allowed to use indefinitely looping highlithting data,
        // any remaining timer should be stopped first

        if(m_view) {
            m_view->stopHighlighting();
        }

        // Launch algorithm runner in a new thread

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

void SimulationController::onMoreMenuTriggered(QAction *action)
{
    if(action == m_hideProgressBar) {
        m_progressBar.setVisible(!action->isChecked());
        return;
    }

    if(action == m_blockClarifier && m_clarifier) {
        m_clarifier->blockSignals(action->isChecked());
        return;
    }
}

void SimulationController::onAccelerationValueChanged(int value)
{
    m_accelerationLCD.display(value);
    useAccelerationValue();
}

void SimulationController::useAccelerationValue()
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
        .withWindowIcon(Resource::windowIcon())
        .withWindowTitle(trUtf8("La simulation ne peut démarrer"))
        .withText(trUtf8("Une simulation implique forcément un algorithme.%1"
                         "Merci de configurer votre simulation puis de recommencer.").arg("\n"))
        .withStandardButtons(QMessageBox::Ok)
        .withDefaultButton(QMessageBox::Ok)
        .exec();
        break;
    case AlgorithmRunner::AlgorithmHasInvalidModel:
        ShadowMessageBox(this)
        .withIcon(QMessageBox::Critical)
        .withWindowIcon(Resource::windowIcon())
        .withWindowTitle(trUtf8("La simulation a terminé immédiatement"))
        .withText(trUtf8("L'algorithme sollicité ne peut s'appliquer au modèle choisi.%1"
                         "Merci de choisir un modèle adéquat puis de recommencer.").arg("\n"))
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
    if(m_clarifier) {
        const bool blocked = m_clarifier->blockSignals(false);
        const QString &str(trUtf8("Arrêt inopiné suite à une exception"));

        m_clarifier->prepareNewSection();
        m_clarifier->addShadowMessageError(str);
        m_clarifier->addMessage(message, 1);

        m_clarifier->addText(""); // add new line (since texts may have been added previously)
        m_clarifier->addError(str);
        m_clarifier->addError("    "+message);

        m_clarifier->blockSignals(blocked);
    }

    if(m_view) {
        const QString &text = trUtf8("Evénement indésirable détecté.") + "\n" + trUtf8("Voir consoles de log.");
        QFont font;
        font.setPointSize(20);

        const int k = 1e4;
        QRectF r = QRectF(-k, -k, 2*k, 2*k);
        HighlightingTextData *data = m_view->addHighlightingText(4000, true, true,
                                                                 text, r, Qt::AlignCenter,
                                                                 QPen(Qt::white), font,
                                                                 Qt::NoPen, QBrush(Qt::black));
        data->rectOpacity = 0.90;
    }

    emit simulationFailedWithException();
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
    if(m_clarifier) {
        m_clarifier->prepareNewSection();
        m_clarifier->addShadowMessageNormal(trUtf8("Requête particulière"));
        m_clarifier->addMessage(trUtf8("Simulation interrompue"), 1);

        m_clarifier->addText("");
        m_clarifier->addText(trUtf8("Requête particulière"));
        m_clarifier->addText("    "+trUtf8("Simulation interrompue"));
    }
}

void SimulationController::onAlgorithmRunnerFinished()
{
    m_algorithmRunner->deleteLater();
}

void SimulationController::onAlgorithmRunnerDestroyed()
{
    // We check the value since this function is called even if algorithm terminates without being stopped (button stop)
    if(m_simulation_acceleration_onStop != -101) {
        m_acceleration.setValue(m_simulation_acceleration_onStop);
        m_simulation_acceleration_onStop = -101; // reset value
    }
    setEnabled(true);

    emit simulationFinished();
}
