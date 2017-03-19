#ifndef FILEMENU_H
#define FILEMENU_H

#include "widget/Menu.h"

class FileMenu : public Menu
{
    Q_OBJECT

protected:
    QAction *m_workspace;

    QAction *m_quit;

public:
    explicit FileMenu(QWidget *parent = 0);

    void setShortcuts();
    void connectSignalsToSlots();
    void retranslate();

signals:
    void workSpaceActionTriggered(bool);

public slots:
};

#endif // FILEMENU_H
