#include "GridGroupBox.h"

#include <QAbstractScrollArea>

GridGroupBox::GridGroupBox(QWidget *parent)
    : QGroupBox(parent)
{
    m_disableChildrenWhenUnchecked = false;

    setLayout(&m_layout);
    m_layout.setMargin(0);
    m_layout.setSpacing(0);

    m_layout.addWidget(&m_gridWidget);

    connect(this, SIGNAL(toggled(bool)), SLOT(onChecked(bool)));
}

bool GridGroupBox::disableChildrenWhenUncheked() const {return isCheckable() && m_disableChildrenWhenUnchecked;}
void GridGroupBox::setDisableChildrenWhenUncheked(bool disable) {m_disableChildrenWhenUnchecked = disable;}

GridWidget* GridGroupBox::gridWidget() const {return const_cast<GridWidget*>(&m_gridWidget);}

void GridGroupBox::onChecked(bool checked)
{
    if(checked || m_disableChildrenWhenUnchecked) {
        return;
    }

    // Enable all children

    QObjectList &childrenObjects = const_cast<QObjectList&>(children());
    foreach(QObject *object, childrenObjects) {
        if(auto *widget = qobject_cast<QWidget*>(object)) {
            widget->setEnabled(true);
        }
        else if(auto *scrollArea = qobject_cast<QAbstractScrollArea*>(object)) {
            scrollArea->setEnabled(true);
        }
    }
}
