#ifndef VIEW_H
#define VIEW_H

#include "ViewForm.h"

#include "graphics/BasicGraphicsView.h"

#include "others/ActionGroup.h"

#include <memory> // unique ptr

#include <QMenu>

class Model;

class View : public BasicGraphicsView
{
    Q_OBJECT

protected:
    std::unique_ptr<ViewForm> m_editionForm;

private:
    Model *m_model;

    QAction *m_sep1; // we use such short name to avoid collision within subclasses
    QAction *m_sep2;
    QAction *m_fullEdition;

public:
    explicit View(QWidget *parent = 0);

    ViewForm* editionForm() const;
    bool isFullEditionEnabled() const;

    Model* model() const;
    void setModel(Model *model);
    View* withModel(Model *model);
    virtual bool acceptModel(Model *model) = 0;

    QRectF modelCoverage() const;

    void setModelBackgroundBrush(const QBrush &brush);

    QList<QAction*> toolBarActions() const;
    virtual void retranslate();

protected:
    virtual QList<QAction*> modelRelatedActions() const;
    virtual QList<QAction*> selectionRelatedActions() const;

    void contextMenuEvent(QContextMenuEvent *event);

//    void paintEvent(QPaintEvent *event);

private:
    QList<QAction*> defaultToolBarActions() const;

signals:
    void modelChanged();

    void fullEditionEnabled(bool enabled);

protected slots:
    virtual void onModelChanged();

private slots:
    void onModelDestroyed();

    void onModelBackgroundBrushChanged();
};

#endif // VIEW_H
