#include "ArrayModel.h"
#include "ArrayModelItem.h"
#include "FakeAlgorithm.h"
#include "ModelPropertyAnimation.h"
#include "SimulationHighlighter.h"
#include "SimulationLocker.h"

#include <QDebug>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

FakeAlgorithm::FakeAlgorithm()
    : Algorithm()
{
    qDebug() << "creates" << __func__ << "in" << thread();
    qDebug() << "---";
}

bool FakeAlgorithm::requiresAModel() const {return true;}
bool FakeAlgorithm::hasAValidModel() const {return dynamic_cast<ArrayModel*>(m_model);}

void FakeAlgorithm::preExecute()
{
    auto &array(*static_cast<ArrayModel*>(m_model));
    array.restoreState();
    array.saveState();

    qDebug() << __func__;

    m_locker->lockFor(2000);

    suspendOrStopIfRequired(); // must be called (somewhere) for algorithm to suspend/stop

    array.addNewItem();
    array.addNewItem();
    array.centerHorizontally(); m_locker->lockFor(1000);
    array.centerVertically();   m_locker->lockFor(1000);
    array.layout(true);         m_locker->lockFor(1000);

    auto *animation = new QParallelAnimationGroup;
    animation->addAnimation(new QPropertyAnimation);
    animation->addAnimation(new QPropertyAnimation);
    ModelPropertyAnimation::configure((QPropertyAnimation*)animation->animationAt(0), &array.at(0), "opacity", 0., 1., 7500, -1);
    ModelPropertyAnimation::configure((QPropertyAnimation*)animation->animationAt(1), &array.at(1), "brushColor", QColor(Qt::gray), QColor(Qt::white), 7000);
    ModelPropertyAnimation().trigger(animation, false, ModelPropertyAnimation::ThisThread);

    // Actally, starting local HighlightingData in asynchronous way leads to crash (investigation later).
    // So HighlightingData object should be declared as algorithm attributes.
    data.configureData(false);
    data.textPen = QPen(Qt::black);
    data.rectBrush = QBrush(Qt::green);
    const QRectF &r(array.coverage());
    data.configureTextData(__func__, QRectF(r.topLeft()-QPointF(0, 50+5), r.topRight()-QPointF(0, 5)), Qt::AlignCenter);
    data.setTimerSingleShot(false);
    m_highlighter->start(&data, 2500, false);

    m_locker->lockFor(1000);
    data.rect.translate(0, -50-5);

    array.swap(0, array.size()-1, true);
}

void FakeAlgorithm::execute()
{
    qDebug() << __func__;

    m_locker->lockFor(1000);
    data.text = __func__;
}

void FakeAlgorithm::postExecute()
{
    qDebug() << __func__;

    m_locker->lockFor(1000);
    data.text = __func__;
}
