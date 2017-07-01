#ifndef ARRAYMODEL_H
#define ARRAYMODEL_H

#include "Model.h"

#include "datastructure/Array.hpp"

#include <QVector>

#include <memory> // for std::unique_ptr

class ArrayModelItem;

class ArrayModel : public Model
{
    friend class ArrayView; // Required to acess m_array (performing undo/redo in array view).
                            // --> Change this to make friend with the required functions only. It seems impossible
                            // (since we do not want to include ArrayView and forward declaration only is insufficient).

    Q_OBJECT

private:
    typedef QVariantList ItemState;
    typedef struct {
        std::vector<ArrayModelItem*> items;
        QList<ItemState> itemStates;
        int layoutSpacing;
    } State;

protected:
    ncpp::Array<ArrayModelItem*> m_array;
    std::vector<std::unique_ptr<ArrayModelItem>> m_itemsToBeFreed;

    int m_layoutSpacing;

private:
    QMap<int, State> m_states;

public:
    explicit ArrayModel(QObject *parent = 0);
    ~ArrayModel();

    Q_INVOKABLE int layoutSpacing() const;
    Q_INVOKABLE void setLayoutSpacing(int spacing);

    bool isValidIndex(int index) const;

    Q_INVOKABLE int size() const;
    Q_INVOKABLE bool isEmpty() const;
    QVector<ArrayModelItem*> items() const;

    bool contains(const QString &value) const;
    bool contains(const ArrayModelItem &item) const;
    bool contains(std::function<bool (const ArrayModelItem&)> predicate) const;
    int indexOf(const ArrayModelItem &item) const;

    ArrayModelItem& at(int index) const;
    ArrayModelItem& first() const;
    ArrayModelItem& last() const;
    ArrayModelItem& operator[](int index) const;
    Q_INVOKABLE QJSValue itemAt(int index);

    // The 3rd parameter means: Center Items Vertically When Layouting
    void swap(int i, int j, bool civwl = false, bool useAnimation = true);
    Q_INVOKABLE void swapItems(int i, int j, bool civwl = false, bool useAnimation = true);

    /*
     * Dev Notes
     * =========
     * We could have merged these two functions.
     * But a default value isn't given to index since we want the user to be notified with an exception when the given index is invalid.
     * In addition, those functions call two different internal functions.
     */
    bool addItem(ArrayModelItem *item);
    bool addItem(ArrayModelItem *item, int index); // /!\ See comments in dev notes above

    ArrayModelItem& addNewItem();
    ArrayModelItem& addNewItem(int index);

    bool removeItem(const ArrayModelItem &item, bool deleteView = true);
    bool removeItemAt(int index, bool deleteView = true);

    Q_INVOKABLE void clear(bool deleteViews = true);

    void sort(std::function<int (const ArrayModelItem&, const ArrayModelItem&)> comparator);
    void sortBySceneRectX();
    void sortByValue();

    Q_INVOKABLE void centerHorizontally();
    Q_INVOKABLE void centerVertically();

    void saveState(int version = 0) override;
    bool restoreState(int version = 0) override;
    void discardStates() override;

    Q_INVOKABLE QString toString() const override;
    Q_INVOKABLE QString toString(const QString &delStart, const QString &delEnd,
                     const QString &sep = ", ") const;

    QRectF coverage() const override;

    void empty() override;
    void shuffle() override;
    Q_INVOKABLE void layout() override;
    Q_INVOKABLE void layout(bool centerItemsVertically);
    /**
     * Layouts this array.
     *
     * Layouting this array results in its items being arranged and positioned linearly,
     * i.e. from array[0] to array[N-1], N being the number of elements in the array.
     *
     * The given start position will become the left corner of this array' first item.
     * The second argument tells whether items should be aligned vertically, since their heights may differ.
     * When ignoredIndexes is not empty, it indicates the indexes of items that shouldn't be moved when reordering.
     */
    void layout(const QPointF &start, bool centerItemsVertically = false,
                const QVector<int> &ignoredIndexes = QVector<int>());

protected:
    void registerItemToBeFreed(ArrayModelItem *item);

private:
    /**
     * Adds item directly,
     * regardless whether the array already contains the item or not.
     */
    void addItemDirectly(ArrayModelItem *item);
    void addItemDirectly(ArrayModelItem *item, int index);

signals:
    void itemAdded(ArrayModelItem *item);
    void itemRemoved(ArrayModelItem *item, bool viewDeletionRequested);
    void cleared();

public slots:
};

#endif // ARRAYMODEL_H
