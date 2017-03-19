#include "FileMenu.h"

FileMenu::FileMenu(QWidget *parent)
    : Menu(parent)
{
    m_workspace = addAction(QIcon(""), "");
        m_workspace->setCheckable(true);

    addSeparator();
    m_quit = addAction(QIcon(""), "");

    setShortcuts();
    connectSignalsToSlots();
}

void FileMenu::setShortcuts()
{
    m_workspace->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_N);
    m_quit->setShortcut(Qt::CTRL + Qt::Key_Q);
}

void FileMenu::connectSignalsToSlots()
{
    connect(m_workspace, static_cast<void (QAction::*)(bool)>(&QAction::triggered),
            this, [this](bool checked){emit workSpaceActionTriggered(checked);});
}

void FileMenu::retranslate()
{
    setTitle("&"+trUtf8("Fichier"));

    m_workspace->setText(trUtf8("Espace de travail"));
    m_quit->setText(trUtf8("Quitter"));
}
