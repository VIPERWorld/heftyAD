#ifndef THREADWORKER_H
#define THREADWORKER_H

#include <QObject>
#include <QPointer>

#include <functional>

/**
 * "Understanding threading with Qt", here are my preferred articles:
 *     http://blog.qt.io/blog/2010/06/17/youre-doing-it-wrong/
 *     https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
 * Foremost please make sure you first read them.
 *
 * The ThreadWorker class provides a simple and light API to be used to run tasks in Qt threads.
 * A thread worker is actually meant to be started in a new thread, but you're not obliged to.
 * For instance a worker may require inputs and yield outputs:
 *     such data can be wrapped into the Data structure.
 */
class ThreadWorker : public QObject
{
    Q_OBJECT

public:
    typedef std::function<void ()> ActionFunctor;
    struct Data : public QObject {};

private:
    ActionFunctor m_action; // action to be executed when this worker is started
    QPointer<Data> m_data;  // data being manipulated by this worker

public:
    explicit ThreadWorker(QObject *parent = 0);

    /**
     * Sets the action to be executed when this worker is start()ed.
     */
    void setAction(ActionFunctor action);

    Data* data() const;
    /**
     * Sets the data being used by this worker.
     * It can be created and accessed from within the associated action functor.
     */
    void setData(Data *data);

    /**
     * Moves this worker to a new thread and starts it.
     * When deleteWorkerLater is true, this worker will be deleted later (when finished is emitted).
     */
    static void startInNewThread(ThreadWorker *worker, bool deleteWorkerLater = false);

signals:
    /**
     * This signal is emitted whenever start() finishes.
     */
    void finished(Data *data);

public slots:
    void start();
};

#endif // THREADWORKER_H
