#include "ArrayForm.h"
#include "ArrayView.h"

#include "widget/SectionedWidgetElt.h"

ArrayForm::ArrayForm(ArrayView *view, QWidget *parent)
    : ViewForm(view, parent),
      m_view(view)
{
    m_curveAttrs->setVisible(false);

    m_arraySpacing.setContextMenuPolicy(Qt::NoContextMenu);

    m_arraySection = m_modelSections.addSection();
    m_arraySection->addWidget(&m_arrayForm, 1, 0);
}

void ArrayForm::retranslate()
{
    ViewForm::retranslate();

    m_arraySection->setBarTitle(trUtf8("Tableau"));

    m_arrayForm.clear();
    m_arraySpacing.setToolTip(trUtf8("Espacement de positionnement"));
    m_arrayForm.addRow(trUtf8("Espacement"), &m_arraySpacing);
}

#include <QDebug>
void ArrayForm::onSceneSelectionChanged()
{
    qDebug() << m_view->sceneSelectedItems();
}
