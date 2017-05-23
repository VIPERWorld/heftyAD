#ifndef WORKSPACEEMPTYTAB_H
#define WORKSPACEEMPTYTAB_H

#include "widget/GridWidget.h"

#include <QSplitter>

class WorkspaceEmptyTab : public GridWidget
{
    Q_OBJECT

public:
    typedef enum {
        SaveAll  = 1,
        CloseAll = 2,
        None     = 4
    } WorkspaceTabAction;

protected:
    WorkspaceTabAction m_allowedActions;

    QSplitter m_splitter;

public:
    explicit WorkspaceEmptyTab(QWidget *parent = 0);

    WorkspaceTabAction allowedActions() const;
    bool allowAction(WorkspaceTabAction action) const;
    bool allowCloseAll() const;
    bool allowSaveAll() const;

    virtual QList<QAction*> contextMenuActions() const;
    virtual void retranslate();

    virtual bool hasDirtyWork() const;
    virtual bool hasOpenedWork() const;

    virtual void saveAllWorks();
    virtual void closeAllWorks();

signals:

public slots:
};

#endif // WORKSPACEEMPTYTAB_H
