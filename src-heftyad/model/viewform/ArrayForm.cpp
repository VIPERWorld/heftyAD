#include "ArrayForm.h"
#include "ArrayView.h"

ArrayForm::ArrayForm(ArrayView *view, QWidget *parent)
    : ViewForm(view, parent),
      m_view(view)
{
    m_arraySpacing.setContextMenuPolicy(Qt::NoContextMenu);
}

void ArrayForm::retranslate()
{
    ViewForm::retranslate();

//    m_modelTab.removeSections();
//    m_modelTab.addSection(trUtf8("arrière plan"));
//    m_modelTab.addSubSection(&m_modelBgBrush); m_modelBgBrush.addRow("kkk", new QLineEdit(this));

//    m_arrayForm.clear();
//    m_arraySpacing.setToolTip(trUtf8("Espacement de positionnement"));
//    m_arrayForm.addRow(trUtf8("Espacement"), &m_arraySpacing);
}

#include <QDebug>
void ArrayForm::onSceneSelectionChanged()
{
    qDebug() << m_view->sceneSelectedItems();
}
