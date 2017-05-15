#include "ThreadWorker.h"

#include <QThread>

ThreadWorker::ThreadWorker(QObject *parent)
    : QObject(parent),
      m_data(nullptr)
{
}

void ThreadWorker::setAction(ThreadWorker::ActionFunctor action) {m_action = action;}

ThreadWorker::Data* ThreadWorker::data() const {return m_data;}
void ThreadWorker::setData(Data *data) {m_data = data;}

void ThreadWorker::start()
{
    m_action();
    emit finished(m_data);
}

void ThreadWorker::startInNewThread(ThreadWorker *worker, bool deleteWorkerLater)
{
    auto* thread = new QThread;
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &::ThreadWorker::start);
    connect(worker, &ThreadWorker::finished, thread, &QThread::quit);
    if(deleteWorkerLater) {
        connect(worker, &ThreadWorker::finished, worker, &ThreadWorker::deleteLater);
    }
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}
