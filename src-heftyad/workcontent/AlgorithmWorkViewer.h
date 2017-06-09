#ifndef ALGORITHMWORKVIEWER_H
#define ALGORITHMWORKVIEWER_H

#include "widget/fileviewer/ModelViewer.h"

#include <QStandardItemModel>

class AlgorithmWorkViewer : public ModelViewer
{
    Q_OBJECT

public:
    typedef enum {
        MetaData,
        Sources
    } ItemCategory;

protected:
    QStandardItemModel m_itemModel;
        QStandardItem *m_meta;
        QStandardItem *m_sources;

public:
    AlgorithmWorkViewer(QWidget *parent = 0);

    QStandardItemModel* itemModel() const;

    void addItem(QStandardItem *item, int category);

    void retranslate();

signals:
    void viewItemPressed(const QModelIndex &index);

public slots:
};

#endif // ALGORITHMWORKVIEWER_H
