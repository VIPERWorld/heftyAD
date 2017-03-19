#ifndef ALGORITHMWORKFILEVIEWER_H
#define ALGORITHMWORKFILEVIEWER_H

#include "widget/fileviewer/ModelViewer.h"

#include <QStandardItemModel>

class AlgorithmWorkFileViewer : public ModelViewer
{
    Q_OBJECT

public:
    typedef enum {
        Global,
        Sources
    } FileItemCategory;

protected:
    QStandardItemModel m_fileModel;
        QStandardItem *m_global;
        QStandardItem *m_sources;

public:
    AlgorithmWorkFileViewer(QWidget *parent = 0);

    QStandardItemModel* fileModel() const;

    void addFileItem(QStandardItem *item, int category);

    void retranslate();

signals:
    void fileItemDoubleClicked(const QModelIndex &index);

public slots:
};

#endif // ALGORITHMWORKFILEVIEWER_H
