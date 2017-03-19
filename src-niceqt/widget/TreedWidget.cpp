#include "TreedWidget.h"

TreedWidget::TreedWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setColumnCount(1);
    setHeaderHidden(true);
}

QTreeWidgetItem* TreedWidget::addSection(int index, const QString &text)
{
    auto *item = new QTreeWidgetItem;
    item->setText(0, text);
    insertTopLevelItem(index, item);
    return item;
}

QTreeWidgetItem* TreedWidget::addSection(const QString &text) {return new QTreeWidgetItem(this, QStringList(text));}

QTreeWidgetItem* TreedWidget::removeSection(int index) {return takeTopLevelItem(index);}
QTreeWidgetItem* TreedWidget::removeFirstSection()     {return removeSection(0);}
QTreeWidgetItem* TreedWidget::removeLastSection()      {return removeSection(topLevelItemCount()-1);}

void TreedWidget::removeSections()
{
    setUpdatesEnabled(false);

    while(topLevelItemCount() != 0) {
        removeSubSections_p(0); // We call the private function so that update is not enabled
        delete removeSection(0);
    }

    setUpdatesEnabled(true);
}

QTreeWidgetItem* TreedWidget::addSubSection(int sectionIndex, QWidget *widget)
{
    QTreeWidgetItem *child = nullptr; // the value to return

    QTreeWidgetItem *section = topLevelItem(sectionIndex);
    if(section && widget) {
        child = new QTreeWidgetItem(section);
        widget->setAutoFillBackground(true); // see QTreeWidget::setItemWidget doc (called below)
        setItemWidget(child, 0, widget);
    }

    return child;
}
QTreeWidgetItem* TreedWidget::addSubSection(QWidget *widget) {return addSubSection(topLevelItemCount()-1, widget);}

QTreeWidgetItem* TreedWidget::removeSubSection(int sectionIndex, int subSectionIndex)
{
    QTreeWidgetItem *child = nullptr; // the value to return

    QTreeWidgetItem *section = topLevelItem(sectionIndex);
    if(section) {
        const int c = section->childCount();
        for(int i=0; i<c; i++) {
            if(i == subSectionIndex) {
                child = section->child(i);
                break;
            }
        }

        if(child) {
            removeSubSection(section, child);
        }
    }

    return child;
}

void TreedWidget::removeSubSections(int sectionIndex)
{
    setUpdatesEnabled(false);
    removeSubSections_p(sectionIndex);
    setUpdatesEnabled(true);
}

void TreedWidget::removeSubSection(QTreeWidgetItem *section, QTreeWidgetItem *subSection)
{
    QWidget *widget = itemWidget(subSection, 0);
    if(widget) {
        widget->setParent(nullptr);
        widget->setAutoFillBackground(false);
    }

    setItemWidget(subSection, 0, nullptr);
    section->removeChild(subSection);
}

void TreedWidget::removeSubSections_p(int sectionIndex)
{
    QTreeWidgetItem *section = topLevelItem(sectionIndex);
    if(section) {
        int i = 0;
        while(section->childCount() != 0) {
            QTreeWidgetItem *child = section->child(i++);
            removeSubSection(section, child);
            delete child;
        }
    }
}
