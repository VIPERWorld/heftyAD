#include "ShadowMessageBox.h"
#include "WorkHelper.h"
#include "WorkspaceBasicTab.h"
#include "WorkspaceWorkEditor.h"

#include "widget/ExtraToolBar.h"

#include <QAction>
#include <QFileDialog>

WorkspaceBasicTab::WorkspaceBasicTab(QWidget *parent)
    : WorkspaceEmptyTab(parent)
{
    m_workToolBar = new ExtraToolBar(Qt::Vertical);
    m_workEditor  = new WorkspaceWorkEditor;

    m_splitter.addWidget(m_workToolBar);
    m_splitter.addWidget(&m_workContainer_groupBox);
    m_splitter.addWidget(m_workEditor);

    m_splitter.setCollapsible(0, false); // The m_workToolBar widget can't be collapsed
    m_splitter.setCollapsible(1, false);
    m_splitter.setCollapsible(2, false);

    m_splitter.setStretchFactor(0, 1);
    m_splitter.setStretchFactor(1, 100);
    m_splitter.setStretchFactor(2, 1);

    m_workContainer_groupBox.gridWidget()->addWidget(&m_workContainer, 0, 0);

    m_workToolBar->setVisible(false); // The toolbar is invisible at the beginning.
    m_workEditor->setVisible(false);

    connect(&m_workContainer, &WorkspaceWorkContainer::toolBarActionsChanged, this, &WorkspaceBasicTab::onToolBarActionsChanged);
    connect(&m_workContainer, &WorkspaceWorkContainer::editionFormAvailable,  this, &WorkspaceBasicTab::onEditionFormAvailable);
}

WorkspaceBasicTab::~WorkspaceBasicTab()
{
}

Work *WorkspaceBasicTab::currentWork() {return m_workContainer.currentWork();}

void WorkspaceBasicTab::openNewWork(const QString &workType)
{
    Work *work = WorkHelper::workInstanceByType(workType);
    if(work) {
        m_workContainer.addWork(work);
    }
}

void WorkspaceBasicTab::openExistingWorks(const QString &workFamily, const QStringList &filePaths)
{
    if(filePaths.isEmpty()) {
        return; // not mandatory
    }

    QStringList worksAlreadyLoaded;
    QStringList worksFailToLoad;
    for(const QString &path : filePaths) {
        Work *work = m_workContainer.getWork(path);
        if(work) { // No need to create a new work
            worksAlreadyLoaded.append(path);
            m_workContainer.setCurrentWork(work);
        }
        else {
            work = WorkHelper::workInstanceByFilePath(path, workFamily);
            if(work) {
                m_workContainer.addWork(work);
            }
            else {
                worksFailToLoad.append(path);
            }
        }
    }

    if(worksAlreadyLoaded.isEmpty() && worksFailToLoad.isEmpty()) { // Nothing goes wrong
        return;
    }

    QString text;
    if(!worksAlreadyLoaded.isEmpty()) {
        QString msg = "<B>"+trUtf8("Travaux déjà ouverts")+"</B>";
        for(const QString &path : worksAlreadyLoaded) {
            msg += "<br/>"+path;
        }
        text += msg;
    }
    if(!worksFailToLoad.isEmpty()) {
        if(!text.isEmpty()) {
            text += "<br/><br/>";
        }
        QString msg = "<B>"+trUtf8("Travaux dont l'ouverture a échoué")+"</B>";
        for(const QString &path : worksFailToLoad) {
            msg += "<br/>"+path;
        }
        text += msg;
    }

    ShadowMessageBox(this)
    .withIcon(worksFailToLoad.isEmpty() ? QMessageBox::Warning : QMessageBox::Critical)
    .withWindowTitle(trUtf8("A votre attention je vous prie"))
    .withTextFormat(Qt::RichText)
    .withText(text)
    //.withInformativeText("<I>"+trUtf8("Vérifiez par exemple que la balise xml principale porte le bon nom.")+"</I>")
    .withStandardButtons(QMessageBox::Ok)
    .withDefaultButton(QMessageBox::Ok)
            .exec();
}

QStringList WorkspaceBasicTab::getExistingWorkFilePaths()
{
    const QString &filter = "*"+m_workContainer.workFilePathExtension();
    const QStringList &filePaths = QFileDialog::getOpenFileNames(this, trUtf8("Sélectionner les travaux à ouvrir"), "", filter);
    return filePaths;
}

void WorkspaceBasicTab::retranslate()
{
    m_workToolBar->setToolTip(trUtf8("Barre d'outils"));
    m_workContainer_groupBox.setTitle(trUtf8("Travaux en cours"));
    m_workContainer.retranslate();
    m_workEditor->retranslate();
}

void WorkspaceBasicTab::onToolBarActionsChanged(const QList<QAction*> &actions)
{
    m_workToolBar->setToolBarActions(actions);
}

void WorkspaceBasicTab::onEditionFormAvailable(QWidget *form)
{
    m_workEditor->setEditor(form);
    m_workEditor->setVisible(form != nullptr);
}
