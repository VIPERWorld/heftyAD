#ifndef WORKARRAYVIEW_H
#define WORKARRAYVIEW_H

#include "ArrayView.h"

#include "others/ActionGroup.h"

class WorkArrayView : public ArrayView
{
    Q_OBJECT

protected:
    // model related actions

    ActionGroup m_actionGroup1;
        QAction *m_addItems;
        QAction *m_textEdition;
        QAction *m_removeItems;

    QAction *m_separator1;

    ActionGroup m_actionGroup2;
        QAction *m_showItemIndexes;
        QAction *m_normalizeItemPositions;

    QAction *m_separator2;

    ActionGroup m_actionGroup3;
        QAction *m_shuffleItems;
        QAction *m_sortItemsByPos;
        QAction *m_sortItemsByValue;

public:
    WorkArrayView(QWidget *parent = 0);
    ~WorkArrayView();

    void retranslate() override;    

protected:
    QList<QAction*> modelMainRelatedActions() const override;

private:
    void createModelRelatedActions();
    void retranslateModelRelatedActions();
};

#endif // WORKARRAYVIEW_H
