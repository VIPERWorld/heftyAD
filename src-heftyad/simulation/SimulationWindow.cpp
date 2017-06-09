#include "Resource.h"
#include "SimulationWindow.h"

#include <QCloseEvent>
#include <QMdiArea>

SimulationWindow::SimulationWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMdiSubWindow(parent, flags),
      m_mdiAreaBeforeGoingFullScreen(nullptr)
{
    setWidget(&m_widget);

    connect(this, &SimulationWindow::windowStateChanged, this, &SimulationWindow::onWindowStateChanged);
    connect(&m_widget, &SimulationWidget::simulationFullScreenEnabled, this, &SimulationWindow::onSimulationFullScreenEnabled);
}

bool SimulationWindow::canBeClosed() const {return m_widget.sideWidget()->configWidget()->isEnabled();}

void SimulationWindow::onSimulationFullScreenEnabled(bool enabled)
{
    if(enabled) {
        m_mdiAreaBeforeGoingFullScreen = mdiArea();
        m_geometryBeforeGoingFullScreen = saveGeometry();

        m_mdiAreaBeforeGoingFullScreen->removeSubWindow(this);
        setWindowModality(Qt::ApplicationModal);
        showFullScreen();

        return;
    }

    if(m_mdiAreaBeforeGoingFullScreen) {
        m_mdiAreaBeforeGoingFullScreen->addSubWindow(this);
        restoreGeometry(m_geometryBeforeGoingFullScreen);
        show();
    }
}

void SimulationWindow::retranslate()
{
    m_widget.retranslate();
}

void SimulationWindow::closeEvent(QCloseEvent *closeEvent)
{
    if(!canBeClosed()) {
        closeEvent->ignore();
        return;
    }

    QMdiSubWindow::closeEvent(closeEvent);
}

void SimulationWindow::onWindowStateChanged(Qt::WindowStates oldState, Qt::WindowStates newState)
{
    Q_UNUSED(oldState)

    // Remove the sub window incon (if maximized)

    if(newState & Qt::WindowMaximized) {
        setWindowIcon(QIcon("?")); // We use "?" since if an empty string were given to QIcon(), the sub window would have shown the Qt icon.
        return;
    }

    // Restore window icon for the sub window

    setWindowIcon(Resource::windowIcon());
}
