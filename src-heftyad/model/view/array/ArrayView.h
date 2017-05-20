#ifndef ARRAYVIEW_H
#define ARRAYVIEW_H

#include "View.h"

class ArrayModel;
class ArrayModelItem;

class ArrayView : public View
{
    Q_OBJECT

protected:
    ArrayModel *m_modelAsArray;

    bool m_onMousePressedAddItem;
    bool m_visibleItemIndexesInArray;

public:
    ArrayView(QWidget *parent = 0);
    ~ArrayView();

    bool acceptModel(Model *model) override;

protected:
    void mousePressEvent(QMouseEvent *event) override;

public slots:
    void removeSelectedItems() override;

    void normalizeItemPositions();
    void shuffleItems();
    void sortItemsBySceneRectX();
    void sortItemsByValue();

    void setOnMousePressedAddItem(bool value);
    void setVisibleItemIndexesInArray(bool visible);

private slots:
    void onModelChanged() override;

    void onItemAddedToModel(ArrayModelItem *item);
    void onItemRemovedFromModel(ArrayModelItem *item, bool viewDeletionRequested);
    void onModelCleared();
};

#endif // ARRAYVIEW_H
