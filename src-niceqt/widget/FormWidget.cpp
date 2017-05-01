#include "FormWidget.h"

#include <QBoxLayout>
#include <QLabel>

FormWidget::FormWidget(QWidget *parent)
    : QWidget(parent)
{
    setLayout(&m_layout);
}

FormWidget::~FormWidget()
{
    while(!m_newedLayouts.isEmpty()) {
        delete m_newedLayouts.takeFirst();
    }
}

QFormLayout* FormWidget::formLayout() const {return const_cast<QFormLayout*>(&m_layout);}

void FormWidget::addRow(QWidget *label, QWidget *field) {m_layout.addRow(label, field);}
void FormWidget::addRow(QWidget *label, QLayout *field) {m_layout.addRow(label, field);}
void FormWidget::addRow(const QString &labelText, QWidget *field) {m_layout.addRow(labelText, field);}
void FormWidget::addRow(const QString &labelText, QLayout *field) {m_layout.addRow(labelText, field);}
void FormWidget::addRow(QWidget *widget) {m_layout.addRow(widget);}
void FormWidget::addRow(QLayout *layout) {m_layout.addRow(layout);}

void FormWidget::addRow(QWidget *label, const QWidgetList &fields, Qt::Orientation orient, int spacing, int margin)
{
    QLayout *layout = newLayout(fields, orient, spacing, margin);
    addRow(label, layout);
}

void FormWidget::addRow(const QString &labelText, const QWidgetList &fields, Qt::Orientation orient, int spacing, int margin)
{
    QLayout *layout = newLayout(fields, orient, spacing, margin);
    addRow(labelText, layout);
}

void FormWidget::addRow(const QWidgetList &widgets, Qt::Orientation orient, int spacing, int margin)
{
    QLayout *layout = newLayout(widgets, orient, spacing, margin);
    addRow(layout);
}

QLayout* FormWidget::newLayout(const QWidgetList &widgets, Qt::Orientation orient, int spacing, int margin)
{
    int dir = -1; // just to avoid the "variable used uninitialized" warning
    switch(orient) {
    case Qt::Horizontal: dir = QBoxLayout::LeftToRight; break;
    case Qt::Vertical:   dir = QBoxLayout::TopToBottom; break;
    }

    QLayout *layout = new QBoxLayout((QBoxLayout::Direction)dir);
    m_newedLayouts << layout;
    for(QWidget *w : widgets) {
        layout->addWidget(w);
    }

    if(spacing >= 0) {
        layout->setSpacing(spacing);
    }
    if(margin >= 0) {
        layout->setMargin(margin);
    }

    return layout;
}

void FormWidget::clear()
{
    QLayoutItem *item;
    while(m_layout.count()!=0 && (item = m_layout.takeAt(0))) {
        if(auto *label = qobject_cast<QLabel*>(item->widget())) {
            label->setText(""); // should be done otherwise texts are superimposed
        }
        /*
         * since when the item is destroyed its layout is too,
         * we make sure the layout won't be deleted twice.
         */
        m_newedLayouts.removeOne(item->layout());
        delete item;
    }
}
