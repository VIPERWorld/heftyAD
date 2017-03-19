#ifndef TREEDWIDGET_H
#define TREEDWIDGET_H

#include <QTreeWidget>

/**
 * /!\ For the time being I'm not sure why but this class is BUGGY.
 *     Indeed there are several crashs lurking around while the code seems clean.
 *     Further investigations are required to spotlight those nasty bugs.
 *     Anyway, the SectionedWidget class can be used as an alternative.
 */
class TreedWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit TreedWidget(QWidget *parent = 0);

    QTreeWidgetItem* addSection(int index, const QString &text = QString());
    QTreeWidgetItem* addSection(const QString &text = QString());

    QTreeWidgetItem* removeSection(int index);
    QTreeWidgetItem* removeFirstSection();
    QTreeWidgetItem* removeLastSection();
    void removeSections();

    /**
     * Adds a sub-section.
     *
     * When adding a new sub-section, make sure no item has the given widget as item-widget.
     * Indeed this function will register widget as item-widget for the returned sub section item: setItemWidget(someItem, 0, widget).
     * But the returned sub section item may not be the only one item such that itemWidget(item, 0) returns true.
     */
    QTreeWidgetItem* addSubSection(int sectionIndex, QWidget *widget);
    QTreeWidgetItem* addSubSection(QWidget *widget);

    QTreeWidgetItem* removeSubSection(int sectionIndex, int subSectionIndex);
    void removeSubSections(int sectionIndex);

private:
    void removeSubSection(QTreeWidgetItem *section, QTreeWidgetItem *subSection);

    void removeSubSections_p(int sectionIndex);

signals:

public slots:
};

#endif // TREEDWIDGET_H
