#ifndef GRIDGROUPBOX_H
#define GRIDGROUPBOX_H

#include "GridWidget.h"

#include <QGroupBox>

class GridGroupBox : public QGroupBox
{
    Q_OBJECT

protected:
    GridWidget m_gridWidget;

private:
    QGridLayout m_layout;

public:
    explicit GridGroupBox(QWidget *parent = 0);

    GridWidget* gridWidget() const;

signals:
};

#endif // GRIDGROUPBOX_H
