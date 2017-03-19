#include "GridWidget.h"

GridWidget::GridWidget(QWidget *parent)
    : QWidget(parent)
{
    setLayout(&m_layout);
    m_layout.setMargin(0);
    m_layout.setSpacing(0);
}

QGridLayout* GridWidget::gridLayout() const {return const_cast<QGridLayout*>(&m_layout);}
QWidgetList GridWidget::widgets() const {return m_table.keys();}

void GridWidget::addWidget(QWidget *widget, int row, int column)
{
    addWidget(widget, row, column, 1, 1);
}

void GridWidget::addWidget(QWidget *widget, int row, int column, int rowSpan, int columnSpan)
{
    m_table[widget] = widget->parentWidget();

    m_layout.addWidget(widget, row, column, rowSpan, columnSpan);
    widget->setVisible(true); // if the widget was previously removed, it should explicitely be shown

    emit widgetAdded(widget);
}

void GridWidget::removeWidget(QWidget *widget)
{
    if(m_table.contains(widget)) {
        removeWidget_p(widget);
    }
}

void GridWidget::removeWidgets()
{
    while(!m_table.isEmpty()) {
        removeWidget_p(m_table.firstKey());
    }
}

void GridWidget::removeWidgetsBut(const QWidgetList &except)
{
    while(!m_table.isEmpty()) {
        QWidget *first = m_table.firstKey();
        if(!except.contains(first)) {
            removeWidget_p(first);
        }
    }
}

void GridWidget::removeWidget_p(QWidget *widget)
{
    m_layout.removeWidget(widget);
    widget->setVisible(false);

    /*
     * If not done, the parent will remain this grid.
     * So it wil try to destry the widget, due to that Qt-ish parent/child relation.
     */
    widget->setParent(m_table[widget]);
    m_table.remove(widget);

    emit widgetRemoved(widget);
}
