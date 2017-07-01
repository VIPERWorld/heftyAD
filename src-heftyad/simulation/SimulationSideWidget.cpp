#include "Algorithm.h"
#include "Resource.h"
#include "SimulationSideWidget.h"

SimulationSideWidget::SimulationSideWidget(QWidget *parent)
    : GridWidget(parent)
{
    // Add widgets

    gridLayout()->setSpacing(5);
    addWidget(&m_controller, 0, 0);
    addWidget(&m_others, 1, 0);

    m_others.addTab(&m_configWidget, "");
    m_others.addTab(&m_consoles, "");

    // Set attributes

    //m_others.setTabPosition(QTabWidget::West);
    m_others.setTabIcon(0, Resource::windowIcon());
    m_others.setTabIcon(1, Resource::windowIcon());

    connectSignalsToSlots();
}

SimulationConfigWidget* SimulationSideWidget::configWidget() const {return const_cast<SimulationConfigWidget*>(&m_configWidget);}

QSize SimulationSideWidget::sizeHint() const {return QSize(150, GridWidget::sizeHint().height());}

void SimulationSideWidget::connectSignalsToSlots()
{
    connect(&m_configWidget, &SimulationConfigWidget::algorithmChanged, [this]() {
        Algorithm *algo = m_configWidget.algorithm();
        SimulationLocker *locker           = algo ? algo->locker()      : nullptr;
        SimulationClarifier *clarifier     = algo ? algo->clarifier()   : nullptr;

        m_controller.setAlgorithm(algo);
        m_controller.setLocker(locker);
        m_controller.setClarifier(clarifier);

        m_consoles.setClarifier(clarifier);
    });
    connect(&m_configWidget, &SimulationConfigWidget::modelChanged, [this]() {
        m_controller.setSimulationView(m_configWidget.modelView());
        emit configWidgetModelChanged();
    });

    connect(&m_controller, &SimulationController::simulationStarted,             [this]() { m_configWidget.setEnabled(false);       });
    connect(&m_controller, &SimulationController::simulationFinished,            [this]() { m_configWidget.setEnabled(true);        });
    connect(&m_controller, &SimulationController::simulationFailedWithException, [this]() { m_others.setCurrentWidget(&m_consoles); });
    connect(&m_controller, &SimulationController::simulationFullScreenEnabled, this, &SimulationSideWidget::simulationFullScreenEnabled);
}

void SimulationSideWidget::retranslate()
{
    m_controller.retranslate();

    m_others.setTabText(0, trUtf8("Votre simulation"));
    m_others.setTabText(1, trUtf8("Consoles de log"));
        m_configWidget.retranslate();
        m_consoles.retranslate();
}
