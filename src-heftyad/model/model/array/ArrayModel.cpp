#include "ArrayModel.h"
#include "ArrayModelItem.h"
#include "ArrayModelXmlReader.h"
#include "ArrayModelXmlWriter.h"
#include "ModelPropertyAnimation.h"

#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

ArrayModel::ArrayModel(QObject *parent)
    : Model(parent)
{
    m_reader = new ArrayModelXmlReader(this);
    m_writer = new ArrayModelXmlWriter(this);

    m_layoutSpacing = 5; // -105 (for instance) could also be used.
}

ArrayModel::~ArrayModel()
{
    clear(false); // do note need to delete view since QGraphicsView handle that
    m_itemsToBeFreed.clear(); // "new"ed items (if any) will be destroyed

    delete m_reader;
    delete m_writer;
}

int ArrayModel::layoutSpacing() const {return m_layoutSpacing;}
void ArrayModel::setLayoutSpacing(int spacing) {m_layoutSpacing = spacing;}

bool ArrayModel::isValidIndex(int index) const {return m_array.isValidIndex(index);}

int ArrayModel::size() const {return m_array.size();}
bool ArrayModel::isEmpty() const {return m_array.isEmpty();}
QVector<ArrayModelItem*> ArrayModel::items() const
{
    return QVector<ArrayModelItem*>::fromStdVector(m_array.values());
}

bool ArrayModel::contains(const QString &value) const
{
    return m_array.contains([&value](ArrayModelItem *item) -> bool {
        return item->value() == value;
    });
}

bool ArrayModel::contains(const ArrayModelItem &item) const
{
    auto &item2(const_cast<ArrayModelItem&>(item));
    return m_array.contains(&item2);
}

bool ArrayModel::contains(std::function<bool (const ArrayModelItem&)> predicate) const
{
    return m_array.contains([&predicate](ArrayModelItem *item) {
        return predicate(*item);
    });
}

int ArrayModel::indexOf(const ArrayModelItem &item) const
{
    auto &item2(const_cast<ArrayModelItem&>(item));
    return m_array.indexOf(&item2);
}

ArrayModelItem& ArrayModel::at(int index) const {return *(m_array.at(index));}
ArrayModelItem& ArrayModel::first() const {return at(0);}
ArrayModelItem& ArrayModel::last() const {return at(size()-1);}
ArrayModelItem& ArrayModel::operator[](int index) const {return at(index);}
QJSValue ArrayModel::itemAt(int index)
{
    ArrayModelItem *item = nullptr;
    try {
        item = &at(index);
    }
    catch(...) {}

    return item ? getJSProxyOf(item) : QJSValue();
}

void ArrayModel::swap(int i, int j, bool civwl, bool useAnimation)
{
    /*
     * Normally, we don't need that kind of complicated algorithm used below.
     * But since items may have different dimensions (width, height, ...),
     * it's important to make sure their moves get some minimal sense.
     */

    // should be done before swapping elements from the inner array
    QPointF start = !isEmpty() ? first().sceneRect().topLeft() : QPointF();

    m_array.swap(i, j); // does nothing when i == j
    if(i==j) {
        return;
    }

    // we make sure start.y() has the lowest possible value
    for(ArrayModelItem *item : items()) {
        const QPointF &p = item->sceneRect().topLeft();
        if(p.y() < start.y()) {
            start.setY(p.y());
        }
    }

    const QPointF &pos_i = at(i).pos(); // positions before layouting this array
    const QPointF &pos_j = at(j).pos();
    layout(start, civwl);
    const QPointF &pos_i_new = at(i).pos(); // positions after layouting
    const QPointF &pos_j_new = at(j).pos();

    if(!useAnimation) {
        at(i).setPos(pos_i_new);
        at(j).setPos(pos_j_new);
    }
    else {
        const QPointF &point = pos_i-pos_i_new;
        const qreal &distance = std::sqrt(std::pow(point.x(), 2) + std::pow(point.y(), 2));
        //       60 --> 500 ms
        // distance --> duration = ?
        qreal duration = distance*500/60;
        duration -= duration*m_animationAcceleration/100; // just to take acceleration into account

        auto *animation = new QParallelAnimationGroup;
        animation->addAnimation(new QPropertyAnimation);
        animation->addAnimation(new QPropertyAnimation);
        ModelPropertyAnimation::configure((QPropertyAnimation*)animation->animationAt(0), &at(i), "pos", pos_i, pos_i_new, duration);
        ModelPropertyAnimation::configure((QPropertyAnimation*)animation->animationAt(1), &at(j), "pos", pos_j, pos_j_new, duration);
        ModelPropertyAnimation().trigger(animation);
    }
}

void ArrayModel::swapItems(int i, int j, bool civwl, bool useAnimation)
{
    try {
        swap(i, j, civwl, useAnimation);
    }
    catch(...) {}
}

bool ArrayModel::addItem(ArrayModelItem *item)
{
    const bool conditionOK = item && !contains(*item);
    if(conditionOK) {
        addItemDirectly(item);
    }

    return conditionOK;
}

bool ArrayModel::addItem(ArrayModelItem *item, int index)
{
    const bool conditionOK = item && !contains(*item);
    if(conditionOK) {
        addItemDirectly(item, index);
    }

    return conditionOK;
}

ArrayModelItem& ArrayModel::addNewItem()
{
    auto *item = new ArrayModelItem;
    registerItemToBeFreed(item);

    addItemDirectly(item);
    return *item;
}

ArrayModelItem& ArrayModel::addNewItem(int index)
{
    auto *item = new ArrayModelItem;
    try {
        addItemDirectly(item, index); // may throw an exception
    }
    catch(std::out_of_range &e) {
        delete item;
        throw e;
    }

    registerItemToBeFreed(item);
    return *item;
}

bool ArrayModel::removeItem(const ArrayModelItem &item, bool deleteView)
{
    auto *item2 = &const_cast<ArrayModelItem&>(item);
    const bool removed = m_array.removeFirst(item2);
    if(removed) {
        emit itemRemoved(item2, deleteView);
    }

    return removed;
}

bool ArrayModel::removeItemAt(int index, bool deleteView)
{
    ArrayModelItem *item = isValidIndex(index) ? m_array.at(index) : nullptr;

    const bool removed = m_array.removeAt(index);
    if(removed) {
        emit itemRemoved(item, deleteView);
    }

    return removed;
}

void ArrayModel::clear(bool deleteViews)
{
    if(isEmpty()) {
        return;
    }

    if(!deleteViews) {
        m_array.clear();
    }
    else {
        while(!m_array.isEmpty()) {
            removeItemAt(0, true);
        }
    }

    emit cleared();
}

void ArrayModel::sort(std::function<int (const ArrayModelItem &, const ArrayModelItem &)> comparator)
{
    /*
     * The algorithm used below is the selection sort algorithm.
     * Using std::sort isn't relevant since inner array contains pointers.
     */

    const int s = size();
    for(int i=0; i<s; i++) {
        int min = i;
        for(int j=i+1; j<s; j++) {
            if(comparator(at(j), at(min)) == -1) {
                min = j;
            }
        }

        // swap items at indexes i and min: we don't use the swap function since it currently layouts the array.
        if(i != min) {
            ArrayModelItem *tmp = m_array[i];
            m_array[i] = m_array[min];
            m_array[min] = tmp;
        }
    }
}

void ArrayModel::sortBySceneRectX()
{
    sort([](const ArrayModelItem &item1, const ArrayModelItem &item2) -> int {
        const QPointF &p1 = item1.sceneRect().topLeft();
        const QPointF &p2 = item2.sceneRect().topLeft();

        if(p1.x() < p2.x()) return -1;
        if(p1.x() > p2.x()) return 1;
        return 0;
    });
}

void ArrayModel::sortByValue()
{
    sort([](const ArrayModelItem &item1, const ArrayModelItem &item2) -> int {
        const QString &s1 = item1.value();
        const QString &s2 = item2.value();

        if(s1 < s2) return -1;
        if(s1 > s2) return 1;
        return 0;
    });
}

void ArrayModel::centerHorizontally()
{
    if(isEmpty()) {
        return;
    }

    qreal pixels = at(0).rect().width();
    const int &s(size());
    for(int i=1; i<s; i++) {
        pixels += m_layoutSpacing + at(i).rect().width();
    }

    /*
     * Now we make sure the top left corner of this array' first item is -pixels/2
     * and then layout the array.
     */
    ArrayModelItem &f(first());
    f.setX(-pixels/2, false); // (0, 0) is the center of any view watching this model.
    f.moveBy(f.rect().width()/2, 0);
    layout();
}

void ArrayModel::centerVertically()
{
    if(isEmpty()) {
        return;
    }

    qreal maxHeight = at(0).rect().height();
    const int &s(size());
    for(int i=1; i<s; i++) {
        const qreal &height(at(i).rect().height());
        if(height > maxHeight) {
            maxHeight = height;
        }
    }

    /*
     * Now we make sure the top left corner of this array' first item is -maxHeight/2
     * and then layout the array.
     */
    ArrayModelItem &f(first());
    f.setY(-maxHeight/2, false); // (0, 0) is the center of any view watching this model.
    f.moveBy(0, f.rect().height()/2);
    layout();
}

void ArrayModel::saveState(int version)
{
    State &state(m_states[version]);
    state.items = m_array.values();
    state.itemStates.clear();
    state.layoutSpacing = m_layoutSpacing;

    for(ArrayModelItem *item : state.items) {
        state.itemStates.append(ItemState());
        item->saveState(state.itemStates.last());
    }
}

bool ArrayModel::restoreState(int version)
{
    if(!m_states.contains(version)) {
        return false;
    }

    /*
     * We pass true as parameter since we want the views associated
     * to any item (even those added from an algorithm) to be removed.
     */
    clear(true);

    State &state(m_states[version]);

    /*// Make sure each backed up item (even those removed after calling saveState) has its associated view deleted

    int i = state.items.size()-1;
    while(i >= 0) {
        m_array.add(state.items.at(i));
        removeItemAt(0, true);
        i--;
    }*/

    // Restore items back

    int i = 0;
    for(ArrayModelItem *item : state.items) {
        item->restoreState(state.itemStates[i++]);
        addItem(item); // views will then be re-created
    }

    m_layoutSpacing = state.layoutSpacing;

    return true;
}

void ArrayModel::discardStates()
{
    m_states.clear();
}

QString ArrayModel::toString() const {return toString("[", "]");}

QString ArrayModel::toString(const QString &delStart, const QString &delEnd,
                             const QString &sep) const
{
    const std::string &delStart2(delStart.toStdString());
    const std::string &delEnd2(delEnd.toStdString());
    const std::string &sep2(sep.toStdString());

    // Can't use the toString function from the inner array since it contains pointers.
    // Using operator<< from ArrayModelItem is exactely what we want here.

    std::stringstream stream;
    const int &s(size());
    int c = 0;

    stream << delStart2;
    for(ArrayModelItem *item : items()) {
        stream << *item;
        if(++c != s) { // Make sure that it is not the last value
            stream << sep2;
        }
    }
    stream << delEnd2;

    return QString(stream.str().c_str());
}

QRectF ArrayModel::coverage() const
{
    if(isEmpty()) {
        return QRectF();
    }

    QPointF topLeft     = first().sceneBoundingRect().topLeft();
    QPointF bottomRight = first().sceneBoundingRect().bottomRight();

    const int s = size();
    for(int i=1; i<s; i++) {
        const QRectF &r(at(i).sceneBoundingRect());
        const QPointF &tl(r.topLeft());
        const QPointF &br(r.bottomRight());

        //qDebug() << topLeft << tl << i << bottomRight << br;

        if(tl.x() < topLeft.x()) {
            topLeft.setX(tl.x());
        }
        if(tl.y() < topLeft.y()) {
            topLeft.setY(tl.y());
        }
        if(br.x() > bottomRight.x()) {
            bottomRight.setX(br.x());
        }
        if(br.y() > bottomRight.y()) {
            bottomRight.setY(br.y());
        }
    }

    return QRectF(topLeft, bottomRight);
}

void ArrayModel::empty() {clear();}

#include <QGlobal.h>
#include <QTime>

int randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

/*
 * Algorithm inspired by
 *     https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#The_modern_algorithm
 *     https://fr.wikipedia.org/wiki/M%C3%A9lange_de_Fisher-Yates
 *
 * Random value generation from
 *     http://eastfist.com/qt_tutorials/how-to-generate-random-number-in-qt/
 */
void ArrayModel::shuffle()
{
    const QTime &time = QTime::currentTime();
    qsrand((uint)time.msec()); // to make sure each thread (running algorithm) gets its own random number sequence

    const int &n(size());
    for(int i=n-1; i>=1; i--) {
        const int &j(randInt(0, i));
        m_array.swap(j, i); // we do not use this.swap since it performs animation and uses a little bit complicated algorithm.
    }
}

void ArrayModel::layout()
{
    layout(false);
}

void ArrayModel::layout(bool centerItemsVertically)
{
    if(!isEmpty()) { // Since we use the 1st item below.
        layout(first().sceneRect().topLeft(), centerItemsVertically);
    }
}

void ArrayModel::layout(const QPointF &start, bool centerItemsVertically,
                        const QVector<int> &ignoredIndexes)
{
    if(isEmpty()) { // Not necessary but helps avoid useless computations
        return;
    }

    qreal maxHeight = -1; // maximal height (among items)
    const qreal &s(size());

    QPointF p = start; // since start can't be assigned a value
    for(int i = 0; i<s; i++) {
        ArrayModelItem *item = m_array[i];
        const QRectF &rect(item->rect());
        const qreal &itemWidth(rect.width());
        const qreal &itemHeight(rect.height());

        if(itemHeight > maxHeight) {
            maxHeight = itemHeight;
        }

        if(!ignoredIndexes.contains(i)) {
            item->setPos(p, false); // the item center will be at the given position
            item->moveBy(itemWidth/2, itemHeight/2);
        }

        if(m_layoutSpacing >= 0) { // align elements to the right
            p.setX(p.x() + itemWidth + m_layoutSpacing);
        }
        else if(i != s-1) { // align elements to the left
            p.setX(p.x() - m_array[i+1]->rect().width() + m_layoutSpacing);
        }
    }

    if(centerItemsVertically) {
        for(int i = 0; i<s; i++) {
            if(!ignoredIndexes.contains(i)) {
                ArrayModelItem *item = m_array[i];
                const QRectF &rect(item->rect());
                const qreal &itemHeight(rect.height());
                const qreal &k(maxHeight - itemHeight);

                item->setY(item->y() + k/2);
            }
        }
    }
}

void ArrayModel::registerItemToBeFreed(ArrayModelItem *item)
{
    if(item != nullptr) {
        std::unique_ptr<ArrayModelItem> uptr(item);
        m_itemsToBeFreed.push_back(std::move(uptr));
    }
}

void ArrayModel::addItemDirectly(ArrayModelItem *item)
{
    m_array.add(item);
    emit itemAdded(item);
}

void ArrayModel::addItemDirectly(ArrayModelItem *item, int index)
{
    m_array.add(item, index);
    emit itemAdded(item);
}
