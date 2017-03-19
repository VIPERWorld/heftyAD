#ifndef GRIDGROUPBOX_H
#define GRIDGROUPBOX_H

#include "GridWidget.h"

#include <QGroupBox>

class GridGroupBox : public QGroupBox
{
    Q_OBJECT

protected:
    GridWidget m_gridWidget;

    bool m_disableChildrenWhenUnchecked;

private:
    QGridLayout m_layout;

public:
    explicit GridGroupBox(QWidget *parent = 0);

    bool disableChildrenWhenUncheked() const;
    void setDisableChildrenWhenUncheked(bool disable);

    GridWidget* gridWidget() const;

signals:

protected slots:
    void onChecked(bool checked);
};

#endif // GRIDGROUPBOX_H
