#ifndef WORKGRAPHWIDGET_H
#define WORKGRAPHWIDGET_H

#include "GraphView.h"

#include "others/ActionGroup.h"

class WorkGraphView : public GraphView
{
    Q_OBJECT

protected:
    // model related actions

    ActionGroup m_actionGroup1;
        QAction *m_addNodes;
        QAction *m_addEdges;
        QAction *m_addEdgesSuccessively;
        QAction *m_addEdgeFromSourceOnMousePressed;
        QAction *m_textEdition;
        QAction *m_removeItems;

    QAction *m_separator;

    ActionGroup m_actionGroup2;
        QAction *m_showEdgePoints;

    // selection related actions

    QAction *m_selectVertices;
    QAction *m_selectEdges;

public:
    explicit WorkGraphView(QWidget *parent = 0);
    ~WorkGraphView();

    void retranslate() override;

protected:
    QList<QAction*> modelMainRelatedActions() const override;
    QList<QAction*> modelItemSelectionRelatedActions() const override;

private:
    void createModelRelatedActions();
    void retranslateModelRelatedActions();

    void createSelectionRelatedActions();
    void retranslateSelectionRelatedActions();

signals:    

public slots:
};

#endif // WORKGRAPHWIDGET_H
