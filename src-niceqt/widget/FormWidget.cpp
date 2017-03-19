#include "FormWidget.h"

#include <QLabel>

FormWidget::FormWidget(QWidget *parent)
    : QWidget(parent)
{
    setLayout(&m_layout);
}

QFormLayout* FormWidget::formLayout() const {return const_cast<QFormLayout*>(&m_layout);}

void FormWidget::addRow(const QString &labelText, QWidget *field) {m_layout.addRow(labelText, field);}
void FormWidget::addRow(const QString &labelText, QLayout *field) {m_layout.addRow(labelText, field);}

void FormWidget::clear()
{
    QLayoutItem *item;
    while(m_layout.count()!=0 && (item = m_layout.takeAt(0))) {
        if(auto *label = qobject_cast<QLabel*>(item->widget())) {
            label->setText(""); // should be done otherwise texts are superimposed
        }
        delete item;
    }
}
