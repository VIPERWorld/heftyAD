#ifndef ALGORITHMWORKCONTENT_H
#define ALGORITHMWORKCONTENT_H

#include "AlgorithmWorkFileViewer.h"

#include "widget/GridWidget.h"

#include <QMap>
#include <QSplitter>
#include <QStackedWidget>

class SciCodeEditor;

/**
 * This class is based from the tutorial
 *     http://qtsimplify.blogspot.fr/2013/08/code-editor-in-making.html
 */
class AlgorithmWorkContent : public GridWidget
{
    Q_OBJECT

    QMap<QStandardItem*, SciCodeEditor*> m_map; // correspondance between an item and its editor

    QSplitter m_splitter;
        AlgorithmWorkFileViewer m_files;
        QStackedWidget m_editors;

public:
    explicit AlgorithmWorkContent(QWidget *parent = 0);

    QStandardItem* addFile(const QString &fileName, int category);

    void retranslate();

private:
    /**
     * Should be called once.
     */
    void initProject();
    QStandardItem* addGlobalFile(const QString &fileName);
    QStandardItem* addSourceFile(const QString &fileName);

signals:

public slots:

protected slots:
    void onFileItemDoubleClicked(const QModelIndex &index);
};

#endif // ALGORITHMWORKCONTENT_H
