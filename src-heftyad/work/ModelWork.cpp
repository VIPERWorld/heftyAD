#include "Model.h"
#include "ModelWork.h"
#include "View.h"

ModelWork::ModelWork(View *view, QWidget *parent)
    : Work(parent),
      m_view(view)
{
    if(m_view != nullptr) {
        addWidget(m_view, 0, 0);

        connect(m_view, &View::dirtyChanged,       this, &ModelWork::dirtyChanged);
        connect(m_view, &View::fullEditionEnabled, this, &ModelWork::editionFormRequired);
    }
}

ModelWork::~ModelWork()
{
    delete m_view->model();
    delete m_view;
}

bool ModelWork::isDirty() const {return m_view && m_view->isDirty();}
void ModelWork::setDirty(bool dirty) {if(m_view) m_view->setDirty(dirty);}
ncpp::UndoStack *ModelWork::undoStack() const {return m_view ? m_view->undoStack() : nullptr;}

bool ModelWork::saveTo(const QString &filePath)
{
    if(m_view) {
        Model *model = m_view->model();
        if(model) {
            return model->saveTo(filePath);
        }
    }
    return false;
}

bool ModelWork::loadFrom(const QString &filePath)
{
    if(m_view) {
        Model *model = m_view->model();
        if(model) {
            return model->loadFrom(filePath);
        }
    }
    return false;
}

QWidget* ModelWork::editionForm() const {return m_view ? (QWidget*)m_view->editionForm() : nullptr;}
bool ModelWork::isEditionEnabled() const {return m_view ? m_view->isFullEditionEnabled() : false;}

QList<QAction *> ModelWork::toolBarActions() const
{
    return m_view ? m_view->toolBarActions() : Work::toolBarActions();
}

void ModelWork::retranslate()
{
    if(m_view) {
        m_view->retranslate();
    }
}

