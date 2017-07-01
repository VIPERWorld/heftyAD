#ifndef ALGORITHMWORKVIEWER_H
#define ALGORITHMWORKVIEWER_H

#include "widget/fileviewer/ModelViewer.h"

#include <QStandardItemModel>

class AlgorithmWorkViewer : public ModelViewer
{
    Q_OBJECT

protected:
    QStandardItemModel m_itemModel;

public:
    AlgorithmWorkViewer(QWidget *parent = 0);

    QStandardItemModel* itemModel() const;
    void addItem(QStandardItem *item);

signals:
    void viewItemPressed(const QModelIndex &index);

public slots:
};

#endif // ALGORITHMWORKVIEWER_H
