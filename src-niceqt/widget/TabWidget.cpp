#include "TabWidget.h"

#include <QTabBar>

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    m_tabVisibility = AlwaysVisible;

    // Make sure that the current widget will also change on right mouse clicked.
    connect(this, &TabWidget::tabBarClicked, this, &TabWidget::setCurrentIndex);

    tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tabBar(), &QTabBar::customContextMenuRequested, this, &TabWidget::onTabBarContextMenuRequested);
}

TabWidget::TabVisibility TabWidget::tabVisibility() const {return m_tabVisibility;}
void TabWidget::setTabVisility(TabWidget::TabVisibility visibility)
{
    m_tabVisibility = visibility;
    setMouseTracking(m_tabVisibility != AlwaysVisible);
}

void TabWidget::enterEvent(QEvent *event)
{
    Q_UNUSED(event);

    switch(m_tabVisibility) {
        case AlwaysVisible:
            return;

        case HideOnMouseLeave:
            tabBar()->setVisible(true);
            return;

        default:
            return;
    }
}

void TabWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    switch(m_tabVisibility) {
        case AlwaysVisible:
            return;

        default:
            tabBar()->setVisible(false);
            return;
    }
}

void TabWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    //qDebug() << event->pos() << rect().top();
    /*int eventPosY = event->pos().y();
    int rectTop = rect().top();
    int tabBarHeight = tabBar()->height();

    bool cond1 = eventPosY >= rectTop;
    bool cond2 = eventPosY <= rectTop+tabBarHeight;
    tabBar()->setVisible(cond1 && cond2);*/
}

void TabWidget::onTabBarContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
}
