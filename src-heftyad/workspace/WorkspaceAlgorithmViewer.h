#ifndef WORKSPACEALGORITHMVIEWER_H
#define WORKSPACEALGORITHMVIEWER_H

#include "widget/fileviewer/ModelViewer.h"

#include <QStandardItemModel>

class WorkspaceAlgorithmViewer : public ModelViewer
{
    Q_OBJECT

protected:
    QStandardItemModel m_viewerModel;
        QStandardItem *m_sources;
        QStandardItem *m_releases;

public:
    explicit WorkspaceAlgorithmViewer(QWidget *parent = 0);

    void retranslate();

signals:

public slots:
};

#endif // WORKSPACEALGORITHMVIEWER_H
