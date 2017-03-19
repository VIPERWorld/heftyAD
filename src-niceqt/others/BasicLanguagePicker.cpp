#include "BasicLanguagePicker.h"

#include <QAbstractButton>
#include <QApplication>
#include <QDirIterator>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

BasicLanguagePicker::BasicLanguagePicker(const QString &iconDir, const QString &iconFileType,
                                         const QString &qmFileDir, const QString &qmFileBaseName,
                                         const QString &translationSource,
                                         const QString &saveLocation,
                                         QObject *parent)
    : QAction(parent),
      m_iconDir(iconDir),
      m_iconFileType(iconFileType),
      m_qmFileDir(qmFileDir),
      m_qmFileBaseName(qmFileBaseName),
      m_translationSource(translationSource),
      m_saveLocation(saveLocation)
{
    setMenu(&m_menu);

    QObject::connect(&m_menu, SIGNAL(triggered(QAction*)), this, SLOT(onMenuTriggered(QAction *)));
}

BasicLanguagePicker::~BasicLanguagePicker()
{
    deleteMenuActions();
}

QString BasicLanguagePicker::currentLanguage() const {return m_currLang;}
void BasicLanguagePicker::setCurrentLanguage(QString lang)
{
    if(lang.isEmpty()) {
        lang = QLocale::system().name().section('_', 0, 0);
    }

    const bool languageInstalled = installLangage(lang);
    if(languageInstalled) {
        m_currLang = lang;
    }

    setIcon(QIcon(m_iconDir + "/" + m_currLang + "." + m_iconFileType));
    retranslate();
    saveCurrentLangage();

    emit languageChanged(m_currLang);
}

QString BasicLanguagePicker::languageLastlySaved() const
{
    QString language;

    // Read language from the save location

    QFile file(m_saveLocation);
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        in >> language;
    }

    // Now make sure the language read is valid.

    auto languageIsValid = false;
    for(QAction *action : m_menu.actions()) {
        if(action->text().compare(language) == 0) {
            languageIsValid = true;
            break;
        }
    }

    if(!languageIsValid) {
        language = "";
    }

    return language;
}

QStringList BasicLanguagePicker::iconFilePaths() const
{
    QStringList list;
    QDirIterator it(m_iconDir);
    while(it.hasNext()) {
        QString s = it.next();
        if(s.endsWith(".") || s.endsWith("..")) {
            continue;
        }
        list << s;
    }

    return list.filter("."+m_iconFileType);
}

void BasicLanguagePicker::saveCurrentLangage()
{
    QFile file(m_saveLocation);
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << m_currLang;
    }
}

void BasicLanguagePicker::handleInstallationError(const QString &qmFile, const QString &qmDir)
{
    QMessageBox msgBox(nullptr);
    msgBox.setIcon(QMessageBox::Information);

    msgBox.setWindowTitle("Unable to find the translation file");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(QString("An error occured while loading the translation file <B>%1</B> from <B>%2</B>.")
                   .arg(qmFile).arg(qmDir));
    msgBox.setInformativeText("Please restore the missing file or switch to the right directory.");

    msgBox.setStandardButtons(QMessageBox::Ok);
    foreach(auto button, msgBox.buttons()) {
        button->setCursor(Qt::PointingHandCursor);
    }
    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.exec();
}

void BasicLanguagePicker::deleteMenuActions()
{
    for(QAction *action : m_menu.actions()) {
        m_menu.removeAction(action);
        delete action;
        action = nullptr;
    }
}

bool BasicLanguagePicker::installLangage(const QString &lang)
{
    qApp->removeTranslator(&m_translator); // First remove the current translator

    const QString &qmDir (m_qmFileDir);
    const QString &qmFile(m_qmFileBaseName+"_"+lang+".qm");

    const bool failToLoadQmFile = !m_translator.load(qmFile, qmDir);
    const bool languageToInstallIDiffersFromTheSourceLanguage = lang.compare(m_translationSource)!=0;

    if(failToLoadQmFile && languageToInstallIDiffersFromTheSourceLanguage) {
        // Make sure error message is displayed in the current langage
        m_translator.load(m_qmFileBaseName+"_"+m_currLang+".qm", qmDir);
        qApp->installTranslator(&m_translator);

        handleInstallationError(qmFile, qmDir);
        return false;
    }

    qApp->installTranslator(&m_translator);
    return true;
}

void BasicLanguagePicker::loadIcons()
{
    deleteMenuActions();

    QStringListIterator it(iconFilePaths());
    while(it.hasNext()) {
        const QString &filePath = it.next();

        QString fileName = filePath;
        fileName.remove(0, 1+m_iconDir.length()); // For instance: change "dir/subdir/fr.png" to "fr.png"
        fileName.chop(1+m_iconFileType.length()); // Remove extension (.png for example)

        m_menu.addAction(new QAction(QIcon(filePath), fileName, nullptr));
    }
}

void BasicLanguagePicker::onMenuTriggered(QAction *action)
{
    if(m_currLang.compare(action->iconText()) != 0) {
        setCurrentLanguage(action->text());
    }
}

void BasicLanguagePicker::retranslate()
{
    m_menu.setTitle(trUtf8("Langues"));
}
