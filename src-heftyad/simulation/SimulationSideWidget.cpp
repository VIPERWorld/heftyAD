#include "Resource.h"
#include "SimulationSideWidget.h"

SimulationSideWidget::SimulationSideWidget(QWidget *parent)
    : GridWidget(parent)
{
    m_controller.setLocker(&m_locker);
    m_controller.setClarifier(&m_clarifier);
    m_simpleConsole.setClarifier(&m_clarifier);
    m_shadowConsole.setClarifier(&m_clarifier);

    m_configWidget.setSimulationLocker(&m_locker);
    m_configWidget.setSimulationClarifier(&m_clarifier);
    m_configWidget.setSimulationHighlighter(&m_highlighter);

    // Add widgets

    m_others.addTab(&m_configWidget, "");
    m_others.addTab(&m_consoles, "");

    m_consoles.addTab(&m_simpleConsole, "");
    m_consoles.addTab(&m_shadowConsole, "");
    m_consoles.setCurrentIndex(1);

    gridLayout()->setSpacing(5);
    addWidget(&m_controller, 0, 0);
    addWidget(&m_others, 1, 0);

    // Set attributes

    //m_others.setTabPosition(QTabWidget::West);
    m_others.setTabIcon(0, Resource::instance().windowIcon());
    m_others.setTabIcon(1, Resource::instance().windowIcon());

    m_consoles.setTabIcon(0, Resource::instance().windowIcon());
    m_consoles.setTabIcon(1, Resource::instance().shadowIcon("shadow.png"));

    connectSignalsToSlots();
}

SimulationConfigWidget* SimulationSideWidget::configWidget() const {return const_cast<SimulationConfigWidget*>(&m_configWidget);}

QSize SimulationSideWidget::sizeHint() const {return QSize(150, GridWidget::sizeHint().height());}

void SimulationSideWidget::connectSignalsToSlots()
{
    connect(&m_configWidget, &SimulationConfigWidget::algorithmChanged, [this]() {
        m_controller.setAlgorithm(m_configWidget.algorithm());
    });
    connect(&m_configWidget, &SimulationConfigWidget::modelChanged, [this]() {
        m_controller.setSimulationView(m_configWidget.modelView());
        emit configWidgetModelChanged();
    });

    connect(&m_controller, &SimulationController::simulationStarted,  [this]() {m_configWidget.setEnabled(false);});
    connect(&m_controller, &SimulationController::simulationFinished, [this]() {m_configWidget.setEnabled(true); });
    connect(&m_controller, &SimulationController::simulationFullScreenEnabled,
            this, &SimulationSideWidget::simulationFullScreenEnabled);
}

void SimulationSideWidget::retranslate()
{
    m_controller.retranslate();
    m_configWidget.retranslate();

    m_others.setTabText(0, trUtf8("Votre simulation"));
    m_others.setTabText(1, trUtf8("Consoles de log"));

    m_consoles.setTabText(0, trUtf8("Console Basique"));
    m_consoles.setTabText(1, trUtf8("Console Shadow"));
}
