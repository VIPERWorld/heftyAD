#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QGridLayout>
#include <QMap>
#include <QWidget>

class GridWidget : public QWidget
{
    Q_OBJECT

private:
    QGridLayout m_layout;

    QMap<QWidget*, QWidget*> m_table; // table matching each added item to his initial parent

public:
    explicit GridWidget(QWidget *parent = 0);

    QGridLayout* gridLayout() const;
    QWidgetList widgets() const;

    void addWidget(QWidget *widget, int row, int column);
    void addWidget(QWidget *widget, int row, int column, int rowSpan, int columnSpan);

    void removeWidget(QWidget *widget);
    void removeWidgets();
    void removeWidgetsBut(const QWidgetList &except);

private:
    void removeWidget_p(QWidget *widget);

signals:
    void widgetAdded(QWidget *widget);
    void widgetRemoved(QWidget *widget);

public slots:
};

#endif // GRIDWIDGET_H
