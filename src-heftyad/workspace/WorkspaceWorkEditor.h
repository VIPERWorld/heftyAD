#ifndef WORKSPACEWORKEDITOR_H
#define WORKSPACEWORKEDITOR_H

#include "widget/GridGroupBox.h"

class WorkspaceWorkEditor : public GridGroupBox
{
    Q_OBJECT

private:
    QWidget *m_editor;

public:
    explicit WorkspaceWorkEditor(QWidget *parent = 0);
    ~WorkspaceWorkEditor();

    void setEditor(QWidget *editor);

    void retranslate();

signals:

public slots:
};

#endif // WORKSPACEWORKEDITOR_H
