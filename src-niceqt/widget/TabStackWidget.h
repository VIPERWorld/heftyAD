#ifndef TABSTACKWIDGET_H
#define TABSTACKWIDGET_H

#include <QComboBox>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

class QTabWidget;

/**
 * The TabStackWidget class behaves like a wrapper over a QTabWidget.
 * It has two visual modes: Tabbed and Stacked.
 * When Tabbed,  the tab widget is shown (with its widgets).
 * When Stacked, the widgets within the tab widget are moved to an inner stack widget
 *               and a combobox helps navigate through the widgets.
 *
 * This class doesn't listen to changes made to the QTabWidget.
 * So we highly recommend not to add/remove widgets to/from the tab widget when visual mode is Stacked,
 * since the tab widget is unmistakably empty in that mode.
 * In addition, other operations like:
 *     changing a tab text,
 *     ...
 * won't be taken into account.
 *
 * Finally due to ownership matters, make sure this widget is in Tabbed mode before it's destroyed.
 * This ensures the inner stack widget is empty and keeps ownership of 0 widgets.
 */
class TabStackWidget : public QWidget
{
    Q_OBJECT

public:
    typedef enum {
        Stacked,
        Tabbed
    } Mode;

private:
    Mode m_mode;

    QVBoxLayout m_layout;
        QComboBox m_comboBox;
        QStackedWidget m_stack;
        //
        QTabWidget *m_tab;

public:
    explicit TabStackWidget(QWidget *parent = 0);

    TabStackWidget::Mode mode() const;
    void setMode(TabStackWidget::Mode mode);

    void setTabWidget(QTabWidget *tab);

private:
    void moveContentToTab();
    void moveContentToStack();

    void removeWidget(QWidget *widget);
    void addWidget(QWidget *widget);

signals:
    void modeChanged();

private slots:
    void onModeChanged();
};

#endif // TABSTACKWIDGET_H
