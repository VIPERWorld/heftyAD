#ifndef SETTINGMENU_H
#define SETTINGMENU_H

#include "widget/Menu.h"

class BasicLanguagePicker;

class SettingMenu : public Menu
{
    Q_OBJECT

protected:
    BasicLanguagePicker *m_langPicker;

public:
    explicit SettingMenu(QWidget *parent = 0);
    ~SettingMenu();

    void setShortcuts();
    void connectSignalsToSlots();
    void retranslate();

signals:

public slots:
};

#endif // SETTINGMENU_H
