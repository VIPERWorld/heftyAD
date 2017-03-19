#ifndef SECTIONEDWIDGETELT_H
#define SECTIONEDWIDGETELT_H

#include "GridWidget.h"
#include "SectionedWidgetEltBar.h"

class SectionedWidgetElt : public GridWidget
{
    Q_OBJECT

private:
    SectionedWidgetEltBar m_bar;

    bool m_unrolled;

public:
    explicit SectionedWidgetElt(QWidget *parent = 0);

    bool isUnrolled() const;
    SectionedWidgetEltBar* bar() const;

    /*
     * Shortcut functions to access/change the state of the bar widget.
     */
    QString barTitle() const;
    void setBarTitle(const QString &title);
    QString barStyleSheet(void) const;
    void setBarStyleSheet(const QString &styleSheet);

    void unroll();

private:
    void adjustBar();
    void onWidgetAdded(QWidget *widget);

signals:
    void unrolled();
};

#endif // SECTIONEDWIDGETELT_H
