#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    typedef enum {
        AlwaysVisible,      // Default value
        HideOnMouseLeave,
        MobilePhoneStyle    // /!\ This name isn't expressive! (It should be changed)
    } TabVisibility;

protected:
    TabVisibility m_tabVisibility;

public:
    explicit TabWidget(QWidget *parent = 0);

    TabVisibility tabVisibility() const;
    void setTabVisility(TabVisibility visibility);

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

signals:

public slots:
    virtual void onTabBarContextMenuRequested(const QPoint &pos);
};

#endif // TABWIDGET_H
