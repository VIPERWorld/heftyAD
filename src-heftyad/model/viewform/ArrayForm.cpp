#include "ArrayForm.h"
#include "ArrayView.h"
#include "ViewItem.h"

#include "widget/SectionWidget.h"

ArrayForm::ArrayForm(ArrayView *view, QWidget *parent)
    : ViewForm(view, parent),
      m_view(view)
{
    // add widgets

    m_arraySection = m_modelSections.addSection();
    m_arraySection->addWidget(&m_arrayForm, 1, 0);
    m_arrayForm.addRow(&m_arraySpacingLabel, &m_arraySpacing);

    // customize widgets

    m_arraySpacing.setContextMenuPolicy(Qt::NoContextMenu);
    const int M = 1e3;
    m_arraySpacing.setRange(-M, M);

    // customize base class widgets

    m_basicForm.setValueEditorPosEnabled(false);
    m_curveAttrs->setVisible(false);
}

void ArrayForm::retranslate()
{
    ViewForm::retranslate();

    m_arraySection->setBarTitle(trUtf8("Tableau"));
    m_arraySpacingLabel.setText(trUtf8("Espacement"));
    m_arraySpacing.setToolTip(trUtf8("Espacement entre les items lorsque positionnés automatiquement"));
}

#include <QDebug>
void ArrayForm::onSceneSelectionChanged()
{
    QList<ViewItem*> items;
    for(QGraphicsItem *item : m_view->sceneSelectedItems()) {
        items.append(static_cast<ViewItem*>(item));
    }

    m_basicForm.setItems(items);

    qDebug() << m_view->sceneSelectedItems();
}
