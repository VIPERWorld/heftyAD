#include "LanguagePicker.h"
#include "SettingMenu.h"
#include "Resource.h"

SettingMenu::SettingMenu(QWidget *parent)
    : Menu(parent)
{
    const QString &iconDir(Resource::languageDir());
    const QString &iconFileType("png");
    const QString &qmFileDir(Resource::userAppTranslationDir());
    const QString &qmFileBaseName("heftyAD");
    const QString &translationSource("fr");
    const QString &saveLocation(Resource::userAppTranslationDir()+"/lang.txt");

    m_langPicker = new LanguagePicker(iconDir, iconFileType, qmFileDir, qmFileBaseName, translationSource, saveLocation);
    m_langPicker->loadIcons();
    m_langPicker->setCurrentLanguage(m_langPicker->languageLastlySaved());

    addAction(m_langPicker);
}

SettingMenu::~SettingMenu()
{
    delete m_langPicker;
}

void SettingMenu::setShortcuts()
{
}

void SettingMenu::connectSignalsToSlots()
{
}

void SettingMenu::retranslate()
{
    setTitle("&"+trUtf8("Paramètres"));
}
