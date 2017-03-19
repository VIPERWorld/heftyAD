#ifndef ALGORITHMRUNNER_H
#define ALGORITHMRUNNER_H

#include "SimplyControllable.h"
#include <QPointer>

class Algorithm;

class AlgorithmRunner : public SimplyControllable
{
    Q_OBJECT

public:
    typedef enum {
        AlreadyRunning, // runner is already running

        AlgorithmIsNull,
        AlgorithmHasInvalidModel,
        StdException,
        UnknownException
    } RunningErrorFlag;

protected:
    QPointer<Algorithm> m_algorithm;

    bool m_isRunning;

public:
    explicit AlgorithmRunner(Algorithm *algorithm = nullptr);
    ~AlgorithmRunner();

    Algorithm* algorithm() const;
    void setAlgorithm(Algorithm *algorithm);

    bool isSuspended() const override;
    bool isRunning() const;

    bool canRun() const;
    void run();

signals:
    void runFailsWith(int errorFlag, const QString &errorMessage = "") const;

    void stopped();
    void finished();

public slots:
    void suspend() override;
    void resume() override;

    void stop();
};

#endif // ALGORITHMRUNNER_H
