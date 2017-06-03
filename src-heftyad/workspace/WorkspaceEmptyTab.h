#ifndef WORKSPACEEMPTYTAB_H
#define WORKSPACEEMPTYTAB_H

#include "widget/GridWidget.h"

#include <QSplitter>

class WorkspaceEmptyTab : public GridWidget
{
    Q_OBJECT

protected:
    QSplitter m_splitter;

public:
    explicit WorkspaceEmptyTab(QWidget *parent = 0);

    virtual QList<QAction*> contextMenuActions() const;
    virtual void retranslate();

signals:

public slots:
};

#endif // WORKSPACEEMPTYTAB_H
