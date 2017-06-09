#include "View.h"
#include "SimulationWidget.h"

SimulationWidget::SimulationWidget(QWidget *parent)
    : GridWidget(parent)
{
    m_view = nullptr;

    m_splitter.setOrientation(Qt::Horizontal);
    m_splitter.setChildrenCollapsible(false);
    addWidget(&m_splitter, 0, 0);

    m_splitter.addWidget(&m_sideWidget);
    m_splitter.setCollapsible(0, true);

    connect(&m_sideWidget, &SimulationSideWidget::simulationFullScreenEnabled, this, &SimulationWidget::simulationFullScreenEnabled);
    connect(&m_sideWidget, &SimulationSideWidget::configWidgetModelChanged, [this]() {
        setView(m_sideWidget.configWidget()->modelView());
    });
}

SimulationSideWidget* SimulationWidget::sideWidget() const {return const_cast<SimulationSideWidget*>(&m_sideWidget);}

void SimulationWidget::setView(View *view)
{
    // First remove the current view (if any)

    if(m_view) {
        m_view->setParent(nullptr);
        m_view->setInvolvedInASimulation(false);
    }

    // Then set the new view

    m_view = view;
    if(m_view) {
        m_splitter.addWidget(m_view);
        m_view->retranslate(); // not mandatory
        m_view->setInvolvedInASimulation(true);
    }
}

void SimulationWidget::retranslate()
{
    m_sideWidget.retranslate();
    if(m_view) {
        m_view->retranslate();
    }
}
