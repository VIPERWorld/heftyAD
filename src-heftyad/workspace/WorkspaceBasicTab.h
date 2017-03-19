#ifndef WORKSPACEBASICTAB_H
#define WORKSPACEBASICTAB_H

#include "WorkspaceEmptyTab.h"
#include "WorkspaceWorkContainer.h"

#include "widget/GridGroupBox.h"

class ExtraToolBar;
class WorkspaceWorkEditor;

class WorkspaceBasicTab : public WorkspaceEmptyTab
{
    Q_OBJECT

protected:
    GridGroupBox m_recentStudies;
    ExtraToolBar *m_workToolBar;
    GridGroupBox m_workContainer_groupBox;
        WorkspaceWorkContainer m_workContainer;
    /*
     * I new-ed this widget due that nasty bug lurking around.
     * Indeed, when the editor is visible (because a given work enables Full Edition),
     * closing the window leads to a crash: the work editor seems to be deleted while
     * the slot onEditionFormAvailable in this class is still being executed.
     *
     * But using a pointer fixes every thing, to my amazement. (-,-)
     *
     * Besides, it' stated in ~QSplitter() doc that "All children are deleted"
     * but adding non new-ed widgets to the splitter doesn't lead to a crash,
     * surprisingly.
     */
    WorkspaceWorkEditor *m_workEditor;

public:
    explicit WorkspaceBasicTab(QWidget *parent = 0);
    ~WorkspaceBasicTab();

    void retranslate() override;

    void openNewWork(const QString &workType);
    void openExistingWorks(const QString &workFamily);

    bool hasUnsavedWork() const override;
    bool hasOpenedWork() const override;

    void saveAllWorks() override;
    void closeAllWorks() override;

signals:

protected slots:
    void onToolBarActionsChanged(const QList<QAction*> &actions);
    void onEditionFormAvailable(QWidget *form);
};

#endif // WORKSPACEBASICTAB_H
