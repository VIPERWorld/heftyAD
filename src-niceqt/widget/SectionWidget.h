#ifndef SECTIONWIDGET_H
#define SECTIONWIDGET_H

#include "GridWidget.h"
#include "SectionWidgetBar.h"

class SectionWidget : public GridWidget
{
    Q_OBJECT

private:
    SectionWidgetBar m_bar;

    bool m_unrolled;

public:
    explicit SectionWidget(QWidget *parent = 0);

    bool isUnrolled() const;
    bool isOpened() const;
    bool isClosedz() const;

    SectionWidgetBar* bar() const;

    /*
     * Shortcut functions to access/change the state of the bar widget.
     */
    QString barTitle() const;
    void setBarTitle(const QString &title);
    QString barStyleSheet(void) const;
    void setBarStyleSheet(const QString &styleSheet);

    void unroll();
    void open();
    void closez();

private:
    void adjustBar();
    void onWidgetAdded(QWidget *widget);

signals:
    void unrolled();
};

#endif // SECTIONWIDGET_H
