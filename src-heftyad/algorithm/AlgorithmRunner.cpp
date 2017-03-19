#include "Algorithm.h"
#include "AlgorithmRunner.h"

AlgorithmRunner::AlgorithmRunner(Algorithm *algorithm)
    : SimplyControllable()
{
    m_isRunning = false;

    m_algorithm = nullptr; // Needs to be set to null first (if not, setAlgorithm below will have an undefined behaviour).
    setAlgorithm(algorithm);
}

AlgorithmRunner::~AlgorithmRunner()
{
}

Algorithm* AlgorithmRunner::algorithm() const {return m_algorithm;}
void AlgorithmRunner::setAlgorithm(Algorithm *algorithm)
{
    if(isRunning() || m_algorithm==algorithm) {
        return;
    }

    if(m_algorithm) {
        disconnect(m_algorithm, &Algorithm::resumed,   this, &AlgorithmRunner::resumed);
        disconnect(m_algorithm, &Algorithm::suspended, this, &AlgorithmRunner::suspended);
    }

    m_algorithm = algorithm;
    if(m_algorithm) {
        connect(m_algorithm, &Algorithm::resumed,   this, &AlgorithmRunner::resumed);
        connect(m_algorithm, &Algorithm::suspended, this, &AlgorithmRunner::suspended);
    }
}

bool AlgorithmRunner::isSuspended() const {return m_algorithm && m_algorithm->isSuspended();}
bool AlgorithmRunner::isRunning() const {return m_isRunning;}

bool AlgorithmRunner::canRun() const
{
    if(isRunning()) {
        emit runFailsWith(AlreadyRunning);
        return false;
    }

    if(m_algorithm == nullptr) {
        emit runFailsWith(AlgorithmIsNull);
        return false;
    }

    if(!m_algorithm->hasAValidModel()) {
        emit runFailsWith(AlgorithmHasInvalidModel);
        return false;
    }

    return true;
}

void AlgorithmRunner::run()
{
    if(canRun()) {
        m_isRunning = true;

        try {
            m_algorithm->run();
        }
        catch(const SimplyControllable::StopException &) { // The algorithm has stopped
            emit stopped();
        }
        catch(const std::exception &e) {
            emit runFailsWith(StdException, e.what());
        }
        catch(...) {
            emit runFailsWith(UnknownException);
        }

        m_isRunning = false;
    }

    emit finished();
}

void AlgorithmRunner::suspend()
{
    if(m_algorithm) {
        m_algorithm->suspend();
    }
}

void AlgorithmRunner::resume()
{
    if(m_algorithm) {
        m_algorithm->resume();
    }
}

void AlgorithmRunner::stop()
{
    if(m_algorithm) {
        m_algorithm->stop();
    }
}
