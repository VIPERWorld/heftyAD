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
    void removeSelection() override;

    void cutSelection() override;

    void normalizeItemPositions();
    void shuffleItems();
    void sortItemsBySceneRectX();
    void sortItemsByValue();

    void setOnMousePressedAddItem(bool value);
    void setVisibleItemIndexesInArray(bool visible);

protected slots:
    void onModelChanged() override;

private slots:
    void onItemAddedToModel(ArrayModelItem *item);
    void onItemRemovedFromModel(ArrayModelItem *item, bool viewDeletionRequested);
    void onModelCleared();
};

#endif // ARRAYVIEW_H
