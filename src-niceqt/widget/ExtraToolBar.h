#ifndef EXTRATOOLBAR_H
#define EXTRATOOLBAR_H

#include "GridWidget.h"

class QToolButton;

class ExtraToolBar : public GridWidget
{
    Q_OBJECT

protected:
    Qt::Orientation m_orientation;
    qreal m_iconSize;

    QList<QToolButton*> m_toolButtons;

public:
    explicit ExtraToolBar(Qt::Orientation orientation, QWidget *parent = 0);
    ~ExtraToolBar();

    QList<QToolButton*> toolButtons(void) const;

signals:

public slots:
    void setToolBarActions(const QList<QAction*> &actions);
    void clearToolBarActions(void);
};

#endif // EXTRATOOLBAR_H
