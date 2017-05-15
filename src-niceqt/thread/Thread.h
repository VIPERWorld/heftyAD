#ifndef THREAD_H
#define THREAD_H

#include <QThread>

/**
 * To be completed!
 * This class is unused, eventually.
 */

class Thread : public QThread
{
    Q_OBJECT

public:
    explicit Thread(QObject *parent = 0);
    ~Thread();

signals:

public slots:
};

#endif // THREAD_H
