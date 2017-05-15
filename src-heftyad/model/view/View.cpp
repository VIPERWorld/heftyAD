#include "Model.h"
#include "Resource.h"
#include "View.h"

#include "utility/Utility.h"

#include <QContextMenuEvent>
#include <QMenu>

View::View(QWidget *parent)
    : BasicGraphicsView(parent),
      m_editionForm(nullptr),
      m_model(nullptr)
{
    m_sep1 = new QAction("", this); m_sep1->setSeparator(true);
    m_sep2 = new QAction("", this); m_sep2->setSeparator(true);

    m_fullEdition = new QAction("", this);
    m_fullEdition->setCheckable(true);
    m_fullEdition->setIcon(Resource::instance().toolBarIcon(".png"));
    connect(m_fullEdition, &QAction::toggled, this, &View::fullEditionEnabled);

    connect(this, &View::modelChanged, this, &View::onModelChanged);

    connect(this, &View::highlightingDataAccelerationChanged, [this]() {
        if(m_model) {
            m_model->setAnimationAcceleration(highlightingDataAcceleration());
        }
    });
}

ViewForm* View::editionForm() const {return m_editionForm.get();}
bool View::isFullEditionEnabled() const {return m_fullEdition->isChecked();}

Model* View::model() const {return m_model;}
void View::setModel(Model *model)
{
    if(m_model==model || !acceptModel(model)) {
        return;
    }

    if(m_model != nullptr) {
        disconnect(m_model, 0, this, 0);
    }

    m_model = model;
    if(m_model != nullptr) {
        connect(m_model, &Model::destroyed, this, &View::onModelDestroyed);

        connect(m_model, &Model::backgroundBrushChanged, this, &View::onModelBackgroundBrushChanged);
    }

    emit modelChanged();
}
View* View::withModel(Model *model)
{
    setModel(model);
    return this;
}

QRectF View::modelCoverage() const {return m_model ? m_model->coverage() : sceneRect();}

void View::setModelBackgroundBrush(const QBrush &brush)
{
    if(m_model) {
        m_model->setBackgroundBrush(brush);
    }
}

QList<QAction*> View::toolBarActions() const
{
    QList<QAction*> retVal = modelRelatedActions();
    if(!retVal.isEmpty()) { // then add separators
        retVal << m_sep1 << m_sep2;
    }
    retVal << defaultToolBarActions();

    return retVal;
}

void View::retranslate()
{
    m_fullEdition->setText(trUtf8("Edition Complète"));

    if(m_editionForm) {
        editionForm()->retranslate();
    }
}

QList<QAction*> View::modelRelatedActions() const {return QList<QAction*>();}
QList<QAction*> View::selectionRelatedActions() const {return QList<QAction*>();}

void View::contextMenuEvent(QContextMenuEvent *event)
{
    BasicGraphicsView::contextMenuEvent(event);

    // temporary data

    QMenu toolBar(trUtf8("Modèle"));
    toolBar.addActions(modelRelatedActions());

    QAction cut      (Resource::instance().toolBarIcon(".png"), trUtf8("Couper"), this);
    QAction copy     (Resource::instance().toolBarIcon(".png"), trUtf8("Copier"), this);
    QAction paste    (Resource::instance().toolBarIcon(".png"), trUtf8("Coller"), this);
    QAction selectAll(Resource::instance().toolBarIcon(".png"), trUtf8("Tout Sélectionner"), this);

    // Now build context menu

    QMenu menu(this);
    menu.addMenu(&toolBar);
    menu.addActions(defaultToolBarActions());
    menu.addSeparator();
    //
    menu.addAction(&cut); // maybe pass the following actions to Edit Menu (so that we do not create different actions)
    menu.addAction(&copy);
    menu.addAction(&paste);
    menu.addSeparator();
    //
    menu.addActions(selectionRelatedActions());
    menu.addAction(&selectAll);

    QAction *action = Utility::execMenuAt(&menu, event->pos(), this);
    if(action == &selectAll) {
        this->selectAll();
        return;
    }
}

//#include <QGraphicsItem>
//void View::paintEvent(QPaintEvent *event)
//{
//    const QList<QGraphicsItem*> &list = scene()->selectedItems();
//    if(list.size() == 1) {
//        QGraphicsItem *item = list.first();
//        const QRectF r(mapFromScene(item->sceneBoundingRect()).boundingRect().normalized());

//        QPainter painter(viewport());
//        painter.setPen(QPen(Qt::black, 1.5, Qt::DashLine));

//        // vertical lines
//        painter.drawLine(r.topLeft() -QPointF(0, 10000), r.bottomLeft() +QPointF(0, 10000));
//        painter.drawLine(r.topRight()-QPointF(0, 10000), r.bottomRight()+QPointF(0, 10000));
//        // horizontal lines
//        painter.drawLine(r.topLeft()   -QPointF(10000, 0), r.topRight()   +QPointF(10000, 0));
//        painter.drawLine(r.bottomLeft()-QPointF(10000, 0), r.bottomRight()+QPointF(10000, 0));
//    }

//    BasicGraphicsView::paintEvent(event);
//}

QList<QAction*> View::defaultToolBarActions() const
{
    return QList<QAction*>() << m_fullEdition;
}

void View::onModelChanged()
{
    if(m_model) {
        m_model->sendAttributeChanges(); // to make sure view registers the right values for model attributes.
    }
}

void View::onModelDestroyed()
{
    setModel(nullptr);
}

void View::onModelBackgroundBrushChanged()
{
    if(m_model) {setBackgroundBrush(m_model->backgroundBrush());}
}
