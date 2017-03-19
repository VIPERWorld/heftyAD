#include "WorkspaceContextMenu.h"

WorkspaceContextMenu::WorkspaceContextMenu(QWidget *parent)
    : Menu(parent)
{
    createDefaultActions();
    addDefaultActions();

    connectSignalsToSlots();
}

WorkspaceContextMenu::~WorkspaceContextMenu()
{
    delete m_tabPosGroup;
}

void WorkspaceContextMenu::setShortcuts()
{
}

void WorkspaceContextMenu::connectSignalsToSlots()
{
    connect(&m_tabPos, &QMenu::triggered, this, &WorkspaceContextMenu::onTabPosTriggered);
}

void WorkspaceContextMenu::retranslate()
{
    m_saveAllWorks->setText(trUtf8("Enregistrer Tout"));
    m_closeAllWorks->setText(trUtf8("Fermer Tout"));

    m_tabPos.setTitle(trUtf8("Position des onglets"));
        m_tabPosNorth->setText(trUtf8("Nord"));
        m_tabPosSouth->setText(trUtf8("Sud"));
        m_tabPosWest->setText(trUtf8("Ouest"));
        m_tabPosEast->setText(trUtf8("Est"));
}

void WorkspaceContextMenu::addDefaultActions()
{
    addAction(m_saveAllWorks);
    addAction(m_closeAllWorks);

    addSeparator();

    addMenu(&m_tabPos);
        m_tabPos.addAction(m_tabPosNorth);
        m_tabPos.addAction(m_tabPosSouth);
        m_tabPos.addAction(m_tabPosWest);
        m_tabPos.addAction(m_tabPosEast);
}

QAction* WorkspaceContextMenu::saveAll() const {return m_saveAllWorks;}
QAction* WorkspaceContextMenu::closeAll() const {return m_closeAllWorks;}

void WorkspaceContextMenu::createDefaultActions()
{
    // Create actions

    m_saveAllWorks = addAction(QIcon(""), "");
    m_closeAllWorks = addAction(QIcon(""), "");

    m_tabPosNorth = m_tabPos.addAction(QIcon(""), "");
    m_tabPosSouth = m_tabPos.addAction(QIcon(""), "");
    m_tabPosWest = m_tabPos.addAction(QIcon(""), "");
    m_tabPosEast = m_tabPos.addAction(QIcon(""), "");

    // Group actions

    m_tabPosGroup = new QActionGroup(&m_tabPos);
        m_tabPosGroup->addAction(m_tabPosNorth); m_tabPosNorth->setCheckable(true);
        m_tabPosGroup->addAction(m_tabPosSouth); m_tabPosSouth->setCheckable(true);
        m_tabPosGroup->addAction(m_tabPosWest); m_tabPosWest->setCheckable(true);
        m_tabPosGroup->addAction(m_tabPosEast); m_tabPosEast->setCheckable(true);
}

void WorkspaceContextMenu::onTabPosTriggered(QAction *action)
{
    const int pos = m_tabPos.actions().indexOf(action);
    if(pos != -1) {
        emit tabPosChanged(pos);
    }
}
