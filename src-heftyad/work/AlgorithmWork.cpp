#include "AlgorithmWork.h"

AlgorithmWork::AlgorithmWork(QWidget *parent)
    : Work(parent)
{
    addWidget(&m_content, 0, 0);
}

bool AlgorithmWork::isDirty() const {return false;}
void AlgorithmWork::setDirty(bool dirty) {Q_UNUSED(dirty)}

bool AlgorithmWork::saveTo(const QString &filePath)
{
    Q_UNUSED(filePath)
    /*if(m_view) {
        Model *model = m_view->model();
        if(model) {
            return model->saveTo(filePath);
        }
    }*/
    return false;
}

bool AlgorithmWork::loadFrom(const QString &filePath)
{
    Q_UNUSED(filePath)
    /*if(m_view) {
        Model *model = m_view->model();
        if(model) {
            return model->loadFrom(filePath);
        }
    }*/
    return false;
}

void AlgorithmWork::retranslate()
{
    m_content.retranslate();
}
