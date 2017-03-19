#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include "widget/GridWidget.h"

class QAbstractItemModel;
class QAbstractItemView;
class QTreeView;

class ModelViewer : public GridWidget
{
    Q_OBJECT

protected:
    QAbstractItemModel *m_model;
    QAbstractItemView *m_view;

public:
    explicit ModelViewer(QWidget *parent = 0);

    QAbstractItemModel* model() const;
    void setModel(QAbstractItemModel *model);

    QAbstractItemView* view() const;
    /**
     * Sets the view.
     * This function implicitly takes ownership of the given view,
     * since it's added to it (parent/child relation).
     */
    void setView(QAbstractItemView *view);

    static void configure(QTreeView *view);

signals:
    void viewChanged(QAbstractItemView *old, QAbstractItemView *current);

public slots:
};

#endif // MODELVIEWER_H
