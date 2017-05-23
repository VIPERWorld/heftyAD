#include "MainData.h"
#include "MainGuiData.h"
#include "ShadowMessageBox.h"
#include "Work.h"
#include "WorkspaceWorkContainer.h"

#include "utility/Utility.h"

#include <QEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMouseEvent>
#include <QTabBar>

WorkspaceWorkContainer::WorkspaceWorkContainer(QWidget *parent)
    : TabWidget(parent)
{
    //setTabVisility(TabWidget::HideOnMouseLeave);

    setTabsClosable(true);
    setMovable(true);

    tabBar()->setAcceptDrops(true);
    tabBar()->setChangeCurrentOnDrag(true);
    tabBar()->installEventFilter(this); // see eventFilter() in this class

    connect(this, &WorkspaceWorkContainer::currentChanged, this, &WorkspaceWorkContainer::onCurrentTabChanged);
    connect(this, &WorkspaceWorkContainer::tabCloseRequested, this, &WorkspaceWorkContainer::onTabCloseRequested);
}


WorkspaceWorkContainer::~WorkspaceWorkContainer()
{
    while(!m_works.isEmpty()) {
        removeWork(m_works.first());
    }
}

QString WorkspaceWorkContainer::tabTitleFor(Work *work)
{
    QString title;

    if(work) {
        title = work->fileName();
        if(title.isEmpty()) {
            int index = 1;

            QMap<Work*, int> &map(m_works_titleIndexes); // just to use a short name
            if(map.contains(work)) {
                index = map.value(work);
            }
            else {
                QList<int> values = map.values();
                std::sort(values.begin(), values.end()); // sort in ascending order
                for(int val : values) {
                    if(val != index) { // then that index'll be used since no work currently uses it
                        break;
                    }
                    index ++;
                }

                map.insert(work, index);
            }

            title = "work " + QString::number(index);
        }
    }

    return title;
}

QString WorkspaceWorkContainer::tabTooltipFor(Work *work)
{
    QString tooltip;

    if(work) {
        tooltip = work->filePath();
        if(tooltip.isEmpty()) {
            tooltip = tabTitleFor(work);
        }
    }

    return tooltip;
}

Work* WorkspaceWorkContainer::currentWork() const {return dynamic_cast<Work*>(currentWidget());}
void WorkspaceWorkContainer::setCurrentWork(Work *work) {setCurrentWidget(static_cast<QWidget*>(work));}

void WorkspaceWorkContainer::addWork(Work *work)
{
    if(!work) {
        return;
    }

    // configure work

    work->retranslate();
    connect(work, &Work::dirtyChanged,        this, &WorkspaceWorkContainer::onCurrentWorkDirtyChanged);
    connect(work, &Work::editionFormRequired, this, &WorkspaceWorkContainer::onCurrentWorkEditionRequired);

    // add work

    m_works.append(work);
    addTab(work, "");
    setCurrentWidget(work);
    onCurrentWorkDirtyChanged();
}

void WorkspaceWorkContainer::removeWork(Work *work)
{
    if(!work) {
        return;
    }

    disconnect(work, &Work::dirtyChanged,        this, &WorkspaceWorkContainer::onCurrentWorkDirtyChanged);
    disconnect(work, &Work::editionFormRequired, this, &WorkspaceWorkContainer::onCurrentWorkEditionRequired);

    m_works.removeOne(work);
    m_works_titleIndexes.remove(work);
    removeTab(indexOf(work));

    delete work;
}

Work* WorkspaceWorkContainer::getWork(const QString &filePath) const
{
    for(Work *work : m_works) {
        const QString &path = work->filePath();
        if(!path.isEmpty() && path==filePath) { // A work with the given filepath is found
            return work;
        }
    }

    return nullptr;
}

bool WorkspaceWorkContainer::containsWork(const QString &filePath) const {return getWork(filePath) != nullptr;}

bool WorkspaceWorkContainer::hasDirtyWork() const
{
    for(Work *work : m_works) {
        if(work->isDirty()) {
            return true;
        }
    }

    return false;
}

bool WorkspaceWorkContainer::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == tabBar() && event->type() == QEvent::MouseButtonPress) {
        auto *mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->buttons() & Qt::MiddleButton) {
            onTabCloseRequested(tabBar()->tabAt(mouseEvent->pos()));
            return true;
        }
    }

    return QTabWidget::eventFilter(watched, event);
}

void WorkspaceWorkContainer::onCurrentWorkDirtyChanged()
{
    Work *work = currentWork();
    const QString &title = tabTitleFor(work);
    const QString &tip   = tabTooltipFor(work);

    setTabText(currentIndex(), work->isDirty() ? title+" [*]" : title);
    setTabToolTip(currentIndex(), tip); // since saving a work also triggers this slot
}

void WorkspaceWorkContainer::onCurrentWorkEditionRequired(bool required)
{
    emit editionFormAvailable(required ? currentWork()->editionForm() : nullptr);
}

void WorkspaceWorkContainer::retranslate()
{
    for(Work* work : m_works) {
        work->retranslate();
    }
}

void WorkspaceWorkContainer::onTabBarContextMenuRequested(const QPoint &pos)
{
    // Build menu

    QMenu menu;
        QAction *save = menu.addAction(QIcon(""), trUtf8("Enregistrer"));
        QAction *saveAs = menu.addAction(QIcon(""), trUtf8("Enregistrer sous")+"...");
        QAction *saveACopy = menu.addAction(QIcon(""), trUtf8("Enregistrer une copie")+"...");
        menu.addSeparator();
        QAction *reload = menu.addAction(QIcon(""), trUtf8("Recharger"));
        QAction *exportAs = menu.addAction(QIcon(""), trUtf8("Exporter sous")+"...");
        menu.addSeparator();
        QAction *close = menu.addAction(QIcon(""), trUtf8("Fermer"));

    // Customize menu

    Work *currWork = currentWork();
    save->setEnabled(currWork->isDirty());
    //
    reload->setEnabled(!currWork->filePath().isEmpty());
    exportAs->setVisible(currWork->isExportFeatureEnabled());

    // Execute menu

    QAction *selectedAction = Utility::execMenuAt(&menu, pos, tabBar());
    if(selectedAction == save)      {saveWork(currWork);                  return;}
    if(selectedAction == saveAs)    {saveWorkAs(currWork);                return;}
    if(selectedAction == saveACopy) {saveWorkCopy(currWork);              return;}
    //
    if(selectedAction == reload)    {reloadWork(currWork);                return;}
    if(selectedAction == exportAs)  {currWork->startExportFeature();      return;}
    //
    if(selectedAction == close)     {onTabCloseRequested(currentIndex()); return;}
}

void WorkspaceWorkContainer::onCurrentTabChanged(int index)
{
    if(index >= 0) {
        Work *curr = currentWork();
        MainData::setCurrentUndoStack(curr->undoStack());
        MainGuiData::setCurrentWork(curr);

        emit toolBarActionsChanged(curr->toolBarActions());
        emit editionFormAvailable(curr->isEditionEnabled() ? curr->editionForm() : nullptr);
    }
    else {
        MainData::setCurrentUndoStack(nullptr);
        MainGuiData::setCurrentWork(nullptr);

        emit toolBarActionsChanged(QList<QAction*>()); // to make sure tool bar will be hidden
        emit editionFormAvailable(nullptr);
    }
}

void WorkspaceWorkContainer::onTabCloseRequested(int index)
{
    auto *work = dynamic_cast<Work*>(widget(index));
    if(work) {
        closeWork(work);
    }
}

void WorkspaceWorkContainer::saveWork(Work *work)
{
    if(!work) {
        return;
    }

    if(work->filePath().isEmpty()) {
        saveWorkAs(work);
        return;
    }

    work->save();
}

void WorkspaceWorkContainer::saveWorkAs(Work *work)
{
    if(!work) {
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, trUtf8("Enregistrer sous"), "", "*.xml");
    if(!filePath.isEmpty()) {
        if(!filePath.endsWith(".xml")) {
            filePath += ".xml";
        }

        if(!work->isDirty()) {
            work->setDirty(true); // to make sure the dirtyChanged signal'll be emitted on saved
        }
        work->setFilePath(filePath);
        work->save();
        m_works_titleIndexes.remove(work);
    }
}

void WorkspaceWorkContainer::saveWorkCopy(Work *work)
{
    if(!work) {
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, trUtf8("Enregistrer une copie"), "", "*.xml");
    if(!filePath.isEmpty()) {
        if(!filePath.endsWith(".xml")) {
            filePath += ".xml";
        }
        work->saveTo(filePath);
    }
}

void WorkspaceWorkContainer::saveAllWorks()
{
    // May change this later to show file dialog only once
    for(Work *work : m_works) {
        saveWork(work);
    }
}

void WorkspaceWorkContainer::reloadWork(Work *work)
{
    if(!work) {
        return;
    }

    if(work->isDirty()) {
        int res = ShadowMessageBox(this)
        .withIcon(QMessageBox::Warning)
        .withWindowTitle(trUtf8("Attention"))
        .withTextFormat(Qt::RichText)
        .withText(trUtf8("Des modifications <B>non enregistrées</B> sont en cours.<br/>"
                         "Elles seront <B>perdues</B> si vous continuez.<br/>"
                         "Souhaitez-vous recharger %1 ?").arg(tabTitleFor(work))
                  )
        .withStandardButtons(QMessageBox::Yes|QMessageBox::No)
        .withDefaultButton(QMessageBox::Yes)
        .exec();

        switch(res) {
        case QMessageBox::Yes:    break;
        default:                  return;
        }
    }

    if(!work->load()) {
        ShadowMessageBox(this)
        .withIcon(QMessageBox::Critical)
        .withWindowTitle(trUtf8("Attention"))
        .withTextFormat(Qt::RichText)
        .withText(trUtf8("%1 n'a pu être rechargée.").arg(tabTitleFor(work)))
        .withInformativeText(trUtf8("<I>Le fichier de travail n'existe probablement plus</I>."))
        .withDetailedText(work->filePath())
        .withStandardButtons(QMessageBox::Ok)
        .withDefaultButton(QMessageBox::Ok)
        .exec();
    }
}

void WorkspaceWorkContainer::closeWork(Work *work)
{
    if(work->isDirty()) {
        setCurrentWork(work);

        int res = ShadowMessageBox(this)
        .withIcon(QMessageBox::Warning)
        .withWindowTitle(trUtf8("Attention"))
        .withTextFormat(Qt::RichText)
        .withText(trUtf8("Des modifications <B>non enregistrées</B> sont en cours.<br/>"
                         "Souhaitez-vous enregistrer %1 ?").arg(tabTitleFor(work))
                  )
        .withStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel)
        //.withButtonTexts(QStringList() << trUtf8("Oui") << trUtf8("Non") << trUtf8("Annuler"))
        .withDefaultButton(QMessageBox::Yes)
        .exec();

        switch(res) {
        case QMessageBox::Yes:    saveWork(work); break;
        case QMessageBox::No:     break;
        case QMessageBox::Cancel: return;
        }
    }

    removeWork(work);
}

void WorkspaceWorkContainer::closeAllWorks()
{
    // May change this later to show file dialog only once
    while(!m_works.isEmpty()) {
        closeWork(m_works.first());
    }
}
