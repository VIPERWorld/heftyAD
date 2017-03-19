#include "WorkspaceWorkEditor.h"

WorkspaceWorkEditor::WorkspaceWorkEditor(QWidget *parent)
    : GridGroupBox(parent),
      m_editor(nullptr)
{
}

WorkspaceWorkEditor::~WorkspaceWorkEditor()
{
    if(m_editor) {
        setEditor(nullptr); // to make sure gridWidget() won't remain the editor' parent: so it won't delete it.
    }
}

void WorkspaceWorkEditor::setEditor(QWidget *editor)
{
    if(m_editor) {
        gridWidget()->removeWidget(m_editor);
        m_editor = nullptr;
    }

    if(editor) {
        m_editor = editor;
        gridWidget()->addWidget(m_editor, 0, 0);
    }
}

void WorkspaceWorkEditor::retranslate()
{
    setTitle(trUtf8("Editeur"));
}
