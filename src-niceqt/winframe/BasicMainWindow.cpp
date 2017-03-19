#include "BasicMainWindow.h"

#include <QEvent>

BasicMainWindow::BasicMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(&m_centralWidget);
}

BasicMainWindow::~BasicMainWindow()
{
}

void BasicMainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        retranslate();
        return;
    }

    QMainWindow::changeEvent(event);
}

int BasicMainWindow::addWidget(QWidget *widget) {return m_centralWidget.addWidget(widget);}
void BasicMainWindow::removeWidget(QWidget *widget) {m_centralWidget.removeWidget(widget);}
void BasicMainWindow::setCurrentWidget(QWidget *widget) {m_centralWidget.setCurrentWidget(widget);}

void BasicMainWindow::retranslate() {}
