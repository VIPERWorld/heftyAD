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
    setUpTabBarContextMenuActions();

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

QString WorkspaceWorkContainer::workFilePathExtension() const {return m_workFilePathExtension;}
void WorkspaceWorkContainer::setWorkFilePathExtension(const QString &filter) {m_workFilePathExtension = filter;}

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
    disableUselessToolBarActions();

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

void WorkspaceWorkContainer::setUpTabBarContextMenuActions()
{
    // create actions

    m_save      = new QAction(QIcon(""), "", this);
    m_saveAs    = new QAction(QIcon(""), "", this);
    m_saveACopy = new QAction(QIcon(""), "", this);
    m_saveAll   = new QAction(QIcon(""), "", this);

    m_reload    = new QAction(QIcon(""), "", this);
    m_compile   = new QAction(QIcon(""), "", this);
    m_exportAs  = new QAction(QIcon(""), "", this);

    m_close     = new QAction(QIcon(""), "", this);
    m_closeAll  = new QAction(QIcon(""), "", this);

    // set shortcuts

    m_save->setShortcut(QKeySequence("Ctrl+S")); addAction(m_save);
    m_saveAs->setShortcut(QKeySequence("Ctrl+Alt+S")); addAction(m_saveAs);
    m_saveAll->setShortcut(QKeySequence("Ctrl+Shift+S")); addAction(m_saveAll);

    m_reload->setShortcut(QKeySequence::Refresh); addAction(m_reload);
    m_compile->setShortcut(QKeySequence("Ctrl+B")); addAction(m_compile);
    m_exportAs->setShortcut(QKeySequence("Ctrl+E")); addAction(m_exportAs);

    m_close->setShortcuts(QList<QKeySequence>() << QKeySequence("Ctrl+W") << QKeySequence("Ctrl+F4")); addAction(m_close);
    m_closeAll->setShortcuts(QList<QKeySequence>() << QKeySequence("Ctrl+Shift+W") << QKeySequence("Ctrl+ShiftF4")); addAction(m_closeAll);

    // connect signals to slots

    connect(m_save,      &QAction::triggered, [this]() { saveWork(currentWork());                         });
    connect(m_saveAs,    &QAction::triggered, [this]() { saveWorkAs(currentWork());                       });
    connect(m_saveACopy, &QAction::triggered, [this]() { saveWorkCopy(currentWork());                     });
    connect(m_saveAll,   &QAction::triggered, [this]() { saveAllWorks();                                  });

    connect(m_reload,    &QAction::triggered, [this]() { reloadWork(currentWork());                       });
    connect(m_compile,   &QAction::triggered, [this]() { currentWork()->startExtraFeature(Work::Compile); });
    connect(m_exportAs,  &QAction::triggered, [this]() { currentWork()->startExtraFeature(Work::Export);  });

    connect(m_close,     &QAction::triggered, [this]() { closeWork(currentWork());                        });
    connect(m_closeAll,  &QAction::triggered, [this]() { closeAllWorks();                                 });
}

void WorkspaceWorkContainer::retranslateTabBarContextMenuActions()
{
    m_save->setText(trUtf8("Enregistrer"));
    m_saveAs->setText(trUtf8("Enregistrer sous")+"...");
    m_saveACopy->setText(trUtf8("Enregistrer une copie")+"...");
    m_saveAll->setText(trUtf8("Enregistrer Tout"));

    m_reload->setText(trUtf8("Recharger"));
    m_compile->setText(trUtf8("Compiler"));
    m_exportAs->setText(trUtf8("Exporter sous")+"...");

    m_close->setText(trUtf8("Fermer"));
    m_closeAll->setText(trUtf8("Fermer Tout"));
}

void WorkspaceWorkContainer::disableUselessToolBarActions()
{
    Work *work = currentWork();
    if(work) {
        m_save->setEnabled(work->isDirty());
        m_saveAll->setEnabled(hasDirtyWork());

        m_reload->setEnabled(!work->filePath().isEmpty());
        m_compile->setVisible(work->allowsExtraFeature(Work::Compile));
        m_exportAs->setVisible(work->allowsExtraFeature(Work::Export));
    }
}

void WorkspaceWorkContainer::retranslate()
{
    retranslateTabBarContextMenuActions();
    for(Work* work : m_works) {
        work->retranslate();
    }
}

void WorkspaceWorkContainer::onTabBarContextMenuRequested(const QPoint &pos)
{
    // Build menu

    QMenu menu;
    QList<QAction*> list;
    list << m_save << m_saveAs << m_saveACopy << m_saveAll
         << menu.addSeparator()
         << m_reload << m_compile << m_exportAs
         << menu.addSeparator()
         << m_close << m_closeAll;
    menu.addActions(list);

    // Execute menu

    Utility::execMenuAt(&menu, pos, tabBar());
}

void WorkspaceWorkContainer::onCurrentTabChanged(int index)
{
    disableUselessToolBarActions();

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

    QString filePath = QFileDialog::getSaveFileName(this, trUtf8("Enregistrer sous"), "", "*"+m_workFilePathExtension);
    if(!filePath.isEmpty()) {
        if(!filePath.endsWith(m_workFilePathExtension)) {
            filePath += m_workFilePathExtension;
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

    QString filePath = QFileDialog::getSaveFileName(this, trUtf8("Enregistrer une copie"), "", "*"+m_workFilePathExtension);
    if(!filePath.isEmpty()) {
        if(!filePath.endsWith(m_workFilePathExtension)) {
            filePath += m_workFilePathExtension;
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
        case QMessageBox::Yes: saveWork(work); break;
        case QMessageBox::No:                  break;
        case QMessageBox::Cancel:              return;
        }
    }

    removeWork(work);
}

void WorkspaceWorkContainer::closeAllWorks()
{
    /**
     * We use such an algorithm since the close operation may be canceled (so the work is not removed).
     * Moreover we first copy the inner work list since the list'll be altered while being iterated through.
     */
    QList<Work*> works = m_works;
    for(Work *work : works) { // May change this later to show file dialog only once
        closeWork(work);
    }
}

