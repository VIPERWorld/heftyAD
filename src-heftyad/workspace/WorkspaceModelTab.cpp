#include "WorkspaceModelTab.h"

WorkspaceModelTab::WorkspaceModelTab(QWidget *parent)
    : WorkspaceBasicTab(parent)
{
    m_workContainer.setWorkFilePathExtension(".xml");

    // Fulfill new-model menu

    m_newArray = m_newModel.addAction(QIcon(""), "");
    m_newMatrix = m_newModel.addAction(QIcon(""), "");
    m_newModel.addSeparator();

    m_newAutomaton = m_newModel.addAction(QIcon(""), "");
    m_newGraph = m_newModel.addAction(QIcon(""), "");
    m_newModel.addSeparator();

    m_newMaze = m_newModel.addAction(QIcon(""), "");

    // Set load model action

    m_loadModels = new QAction(QIcon(""), "", nullptr);

    connectSignalsToSlots();

    // Momentarily disable some actions

    m_newMatrix->setEnabled(false);
    m_newAutomaton->setEnabled(false);
    m_newMaze->setEnabled(false);
}

WorkspaceModelTab::~WorkspaceModelTab()
{
    delete m_loadModels;
}

QList<QAction*> WorkspaceModelTab::contextMenuActions() const
{
    return QList<QAction*>() << m_newModel.menuAction() << m_loadModels;
}

void WorkspaceModelTab::retranslate()
{
    WorkspaceBasicTab::retranslate();

    m_newModel.setTitle(trUtf8("Nouveau modèle"));
        m_newArray->setText(trUtf8("Tableau"));
        m_newMatrix->setText(trUtf8("Matrice"));
        // ---
        m_newAutomaton->setText(trUtf8("Automate"));
        m_newGraph->setText(trUtf8("Graphe"));
        // ---
        m_newMaze->setText(trUtf8("Labyrinthe"));
    m_loadModels->setText(trUtf8("Modèles existants")+"...");
}

void WorkspaceModelTab::connectSignalsToSlots()
{
    connect(m_newArray, &QAction::triggered, [this](){openNewWork("array");});

    connect(m_newGraph, &QAction::triggered, [this](){openNewWork("graph");});

    connect(m_loadModels, &QAction::triggered, this, [this](){openExistingWorks("model", getExistingWorkFilePaths());});
}
