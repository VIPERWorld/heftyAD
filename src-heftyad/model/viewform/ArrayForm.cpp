#include "ArrayForm.h"
#include "ArrayModel.h"
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

    // connect signals to slots

    connect(m_view, &ArrayView::modelChanged, [this]() {
        auto *array = static_cast<ArrayModel*>(m_view->model());
        if(array) {
            const QSignalBlocker blocker(m_arraySpacing); Q_UNUSED(blocker)
            m_arraySpacing.setValue(array->layoutSpacing());
        }
    });
    connect(&m_arraySpacing, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ArrayForm::onArraySpacingChanged);
}

void ArrayForm::retranslate()
{
    ViewForm::retranslate();

    m_arraySection->setBarTitle(trUtf8("Tableau"));
    m_arraySpacingLabel.setText(trUtf8("Espacement"));
    m_arraySpacing.setToolTip(trUtf8("Espacement entre les items")+"\n"+
                              trUtf8("(pris en compte pendant la normalization de leur position par exemple)"));
}

#include <QDebug>
void ArrayForm::onSceneSelectionChanged()
{
    QList<ViewItem*> items;
    for(QGraphicsItem *item : m_view->sceneSelectedItems()) {
        items.append(static_cast<ViewItem*>(item));
    }

    for(auto *f : forms()) {
        f->setItems(items);
    }

    qDebug() << m_view->sceneSelectedItems();
}

void ArrayForm::onArraySpacingChanged()
{
    auto *array = static_cast<ArrayModel*>(m_view->model());
    array->setLayoutSpacing(m_arraySpacing.value());
}
