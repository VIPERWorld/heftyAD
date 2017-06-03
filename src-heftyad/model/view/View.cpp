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

    setUpContextMenuDefaultActions();
    m_ignoreContextMenu = false;

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
    QList<QAction*> retVal = modelMainRelatedActions();
    if(!retVal.isEmpty()) { // then add separators
        retVal << m_sep1 << m_sep2;
    }
    retVal << defaultToolBarActions();

    return retVal;
}

void View::retranslate()
{
    m_fullEdition->setText(trUtf8("Edition Complète"));
    retranslateContextMenuDefaultActions();
    if(m_editionForm) {
        editionForm()->retranslate();
    }
}

void View::setIgnoreContextMenu(bool ignore) {m_ignoreContextMenu = ignore;}

QList<QAction*> View::modelMainRelatedActions() const {return QList<QAction*>();}
QList<QAction*> View::modelItemSelectionRelatedActions() const {return QList<QAction*>();}

void View::contextMenuEvent(QContextMenuEvent *event)
{
    if(m_ignoreContextMenu) {
        return;
    }

    BasicGraphicsView::contextMenuEvent(event);

    // temporary data

    QMenu toolBar(trUtf8("Modèle"));
    toolBar.addActions(modelMainRelatedActions());

    // Now build context menu

    QMenu menu(this);
    menu.addMenu(&toolBar);
    menu.addActions(defaultToolBarActions());
    menu.addSeparator();
    //
    menu.addAction(m_cut); // maybe: pass the following actions to Edit Menu later (so that we do not create different actions)
    menu.addAction(m_copy);
    menu.addAction(m_copyToSystemClipboard);
    menu.addAction(m_paste);
    menu.addSeparator();
    //
    menu.addAction(m_duplicate);
    menu.addSeparator();
    //
    menu.addActions(modelItemSelectionRelatedActions());
    menu.addAction(m_selectAll);

    Utility::execMenuAt(&menu, event->pos(), this);
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

void View::setUpContextMenuDefaultActions()
{
    // create actions

    m_cut                   = new QAction(QIcon(""), "", this);
    m_copy                  = new QAction(QIcon(""), "", this);
    m_copyToSystemClipboard = new QAction(QIcon(""), "", this);
    m_paste                 = new QAction(QIcon(""), "", this);

    m_duplicate             = new QAction(QIcon(""), "", this);

    m_selectAll             = new QAction(QIcon(""), "", this);

    // set shortcuts

    m_cut->setShortcut(QKeySequence::Cut); addAction(m_cut);
    m_copy->setShortcut(QKeySequence::Copy); addAction(m_copy);
    m_copyToSystemClipboard->setShortcut(QKeySequence("Ctrl+Shift+C")); addAction(m_copyToSystemClipboard);
    m_paste->setShortcut(QKeySequence::Paste); addAction(m_paste);

    m_duplicate->setShortcut(QKeySequence("Ctrl+D")); addAction(m_duplicate);

    m_selectAll->setShortcut(QKeySequence::SelectAll); addAction(m_selectAll);

    // connect signals to slots

    connect(m_cut,                   &QAction::triggered, [this]() { cutSelection();             });
    connect(m_copy,                  &QAction::triggered, [this]() { copySelection();            });
    connect(m_copyToSystemClipboard, &QAction::triggered, [this]() { copySelectionToClipboard(); });
    connect(m_paste,                 &QAction::triggered, [this]() { pasteCopiedSelection();     });

    connect(m_duplicate,             &QAction::triggered, [this]() { duplicateSelection();       });

    connect(m_selectAll,             &QAction::triggered, [this]() { selectAll();                });

    connect(scene(), &QGraphicsScene::selectionChanged, this, &View::disableUselessContextMenuDefaultActions);
    disableUselessContextMenuDefaultActions();
    m_paste->setEnabled(false);

    // momentarily

    m_cut->setVisible(false);
    m_copy->setVisible(false);
    m_copyToSystemClipboard->setVisible(false);
    m_paste->setVisible(false);

    m_duplicate->setVisible(false);
}

void View::retranslateContextMenuDefaultActions()
{
    m_cut->setText(trUtf8("Couper"));
    m_copy->setText(trUtf8("Copier"));
    m_copyToSystemClipboard->setText(trUtf8("Copier dans le Press-papiers System"));
    m_paste->setText(trUtf8("Coller"));

    m_duplicate->setText(trUtf8("Dupliquer"));

    m_selectAll->setText(trUtf8("Tout Sélectionner"));
}

void View::disableUselessContextMenuDefaultActions()
{
    const QList<QGraphicsItem*> &list = scene()->selectedItems();
    const bool empty = list.isEmpty();

    m_cut->setEnabled(!empty);
    m_copy->setEnabled(!empty);

    m_duplicate->setEnabled(!empty);
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
