#ifndef BASICMAINWINDOW_H
#define BASICMAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class BasicMainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    QStackedWidget m_centralWidget;

public:
    explicit BasicMainWindow(QWidget *parent = 0);
    ~BasicMainWindow();

protected:
    /**
     * If the concerned event is of type QEvent::LanguageChange,
     * meaning that the application translation has changed,
     * the virtual retranslate() function will be called.
     *
     * Otherwise the event is broadcasted to the upper class QMainWindow.
     *
     * Moreover since the retranslate() function is virtual, it can be overridden in subclasses.
     * Therefore there is no need to call QMainWindow::changeEvent(QEvent *event) when the application translation changes.
     */
    virtual void changeEvent(QEvent *event);

signals:

public slots:
    int addWidget(QWidget *widget);
    void removeWidget(QWidget *widget);
    void setCurrentWidget(QWidget *widget);

    /**
     * Performs translation: default implementation merely does nothing.
     *
     * class MainWindow : public BasicMainWindow
     * {
     * protected:
     *      // attributes (widgets, ...)
     *
     * public slots: // The retranslate function may not be a slot.
     *      void retranslate() override
     *      {
     *          // Let's assume there are the widget and menu attributes in this class.
     *
     *          widget.retranslate(); // A function for translation should be defined for widget.
     *          menu.setTitle(tr("Menu title"));
     *      }
     * };
     */
    virtual void retranslate();
};

#endif // BASICMAINWINDOW_H
