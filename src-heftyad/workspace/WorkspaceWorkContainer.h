#ifndef WORKSPACEWORKCONTAINER_H
#define WORKSPACEWORKCONTAINER_H

#include "widget/TabWidget.h"

#include <QAction>
#include <QMap>

class Work;

class WorkspaceWorkContainer : public TabWidget
{
    Q_OBJECT

private:
    QList<Work*> m_works;
    QMap<Work*, int> m_works_titleIndexes;
    QString m_workFilePathExtension; // .xml or .js for instance

    // tab bar context menu

    QAction *m_save;
    QAction *m_saveAs;
    QAction *m_saveACopy;
    QAction *m_saveAll;
    //
    QAction *m_reload;
    QAction *m_compile;
    QAction *m_exportAs;
    //
    QAction *m_close;
    QAction *m_closeAll;

public:
    explicit WorkspaceWorkContainer(QWidget *parent = 0);
    ~WorkspaceWorkContainer();

    QString workFilePathExtension() const;
    void setWorkFilePathExtension(const QString &filter);

    QString tabTitleFor(Work *work);
    QString tabTooltipFor(Work *work);

    Work* currentWork() const;
    void setCurrentWork(Work *work);

    void addWork(Work *work);
    void removeWork(Work *work);

    /**
     * Returns the work with the given file pah.
     * Works with empty file path are ignored.
     * So getWork("") will always return nullptr even if there are works with empty file path (newly created ones).
     */
    Work* getWork(const QString &filePath) const;
    /**
     * Returns getWork(filePath) != nullptr.
     */
    bool containsWork(const QString &filePath) const;

    bool hasDirtyWork() const;

    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    void onCurrentWorkDirtyChanged();
    void onCurrentWorkEditionRequired(bool required);

private:
    void setUpTabBarContextMenuActions();
    void retranslateTabBarContextMenuActions();
    void disableUselessToolBarActions();

signals:
    void toolBarActionsChanged(const QList<QAction*> &actions);
    void editionFormAvailable(QWidget *form);

public slots:
    void retranslate();

    void onTabBarContextMenuRequested(const QPoint &pos) override;
    void onCurrentTabChanged(int index);
    void onTabCloseRequested(int index);

    void saveWork(Work *work);
    void saveWorkAs(Work *work);
    void saveWorkCopy(Work *work);
    void saveAllWorks();

    void reloadWork(Work *work);

    void closeWork(Work *work);
    void closeAllWorks();
};

#endif // WORKSPACEWORKCONTAINER_H
