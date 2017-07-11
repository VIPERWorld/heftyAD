#include "WorkspaceAlgorithmTab.h"

#include <QAction>

WorkspaceAlgorithmTab::WorkspaceAlgorithmTab(QWidget *parent)
    : WorkspaceBasicTab(parent)
{
    m_workContainer.setWorkFilePathExtension(".js");

    m_newAlgorithm = new QAction(QIcon(""), "", nullptr);
    m_loadAlgorithms = new QAction(QIcon(""), "", nullptr);

    connectSignalsToSlots();
}

WorkspaceAlgorithmTab::~WorkspaceAlgorithmTab()
{
    delete m_newAlgorithm;
    delete m_loadAlgorithms;
}

QList<QAction*> WorkspaceAlgorithmTab::contextMenuActions() const
{
    return QList<QAction*>() << m_newAlgorithm << m_loadAlgorithms;
}

void WorkspaceAlgorithmTab::retranslate()
{
    WorkspaceBasicTab::retranslate();

    m_newAlgorithm->setText(trUtf8("Nouvel algorithme"));
    m_loadAlgorithms->setText(trUtf8("Algorithmes existants")+"...");
}

void WorkspaceAlgorithmTab::connectSignalsToSlots()
{
    connect(m_newAlgorithm,   &QAction::triggered, [this](){openNewWork("algorithm");});
    connect(m_loadAlgorithms, &QAction::triggered, [this](){openExistingWorks("algorithm", getExistingWorkFilePaths());});
}
