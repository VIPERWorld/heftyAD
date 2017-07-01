#ifndef ALGORITHMWORKCONTENT_H
#define ALGORITHMWORKCONTENT_H

#include "AlgorithmWorkViewer.h"

#include "widget/GridWidget.h"

#include <QMap>
#include <QSplitter>
#include <QStackedWidget>

class SciCodeEditor;

class AlgorithmWorkContent : public GridWidget
{
    Q_OBJECT

private:
    QMap<QStandardItem*, SciCodeEditor*> m_map; // correspondance between an item and its editor

    QSplitter m_splitter;
        AlgorithmWorkViewer m_viewer;
        QStackedWidget m_editors;

public:
    explicit AlgorithmWorkContent(QWidget *parent = 0);

private:
    /**
     * Should be called once.
     */
    void fillViewer();

    QStandardItem* addItem(const QString &name, const QString &editorText = "");

signals:

public slots:

protected slots:
    void onViewItemPressed(const QModelIndex &index);
};

#endif // ALGORITHMWORKCONTENT_H
