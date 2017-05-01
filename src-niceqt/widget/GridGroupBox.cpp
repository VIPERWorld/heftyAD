#include "GridGroupBox.h"

GridGroupBox::GridGroupBox(QWidget *parent)
    : QGroupBox(parent)
{
    setLayout(&m_layout);
    m_layout.setMargin(0);
    m_layout.setSpacing(0);

    m_layout.addWidget(&m_gridWidget);
}

GridWidget* GridGroupBox::gridWidget() const {return const_cast<GridWidget*>(&m_gridWidget);}
