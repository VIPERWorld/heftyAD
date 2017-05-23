#include "Model.h"
#include "ModelWork.h"
#include "View.h"

#include <QFileDialog>

ModelWork::ModelWork(View *view, QWidget *parent)
    : Work(parent),
      m_view(view)
{
    m_exportFeatureEnabled = true;

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
    bool saved = false;
    if(m_view) {
        Model *model = m_view->model();
        if(model) {
            saved = model->saveTo(filePath);
        }
    }

    return saved;
}

bool ModelWork::loadFrom(const QString &filePath)
{
    bool loaded = false;
    if(m_view) {
        Model *model = m_view->model();
        if(model) {
            model->empty();
            loaded = model->loadFrom(filePath);
            m_view->undoStack()->clear();
        }
    }

    return loaded;
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

void ModelWork::execExportDialog()
{
    if(m_view) {
        QString filePath = QFileDialog::getSaveFileName(this, trUtf8("Exporter comme png"), "", "*.png");
        if(!filePath.isEmpty()) {
            if(!filePath.endsWith(".png")) {
                filePath += ".png";
            }

            m_view->saveImageTo(filePath, "PNG");
        }
    }
}
