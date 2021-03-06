#ifndef WORKSPACEALGORITHMTAB_H
#define WORKSPACEALGORITHMTAB_H

#include "WorkspaceBasicTab.h"

class WorkspaceAlgorithmTab : public WorkspaceBasicTab
{
    Q_OBJECT

protected:
    QAction *m_newAlgorithm;
    QAction *m_loadAlgorithms;

public:
    explicit WorkspaceAlgorithmTab(QWidget *parent = 0);
    ~WorkspaceAlgorithmTab();

    QList<QAction*> contextMenuActions() const override;
    void retranslate() override;

private:
    void connectSignalsToSlots();

signals:

public slots:
};

#endif // WORKSPACEALGORITHMTAB_H
