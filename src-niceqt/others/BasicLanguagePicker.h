#ifndef BASICLANGUAGEPICKER_H
#define BASICLANGUAGEPICKER_H

#include <QAction>
#include <QMenu>
#include <QTranslator>

class BasicLanguagePicker : public QAction
{
    Q_OBJECT

private:
    QTranslator m_translator;
    QString m_currLang;
    QMenu m_menu;

    QString m_iconDir;
    QString m_iconFileType;
    QString m_qmFileDir;
    QString m_qmFileBaseName;
    QString m_translationSource;
    QString m_saveLocation;

public:
    /**
     * iconDir              is the directory where icons (one per language) are stored.
     *                      It should not end with a "/".
     *                      For instance "dir/subdir" is Ok but "dir/subdir/" is not.
     *                      Indeed when changing the current language, icon is set as follows:
     *                          setIcon(QIcon(m_iconDir + "/" + m_currLang + "." + m_iconFileType)
     *
     * iconFileType         is the extension of the icon files within iconDir.
     *                      It can be "png", "gif", "jpeg", ...
     *                      All icons should have the same filetype/extension.
     *                      For instance "fr.png" (for french), "es.png", "de.png", ...
     *
     * qmFileDir            is the directory where the qm files are stored.
     *                      Indeed *.qm files are needed for translation to take place.
     *                      Unlike iconDir, it may end with a "/".
     *
     * qmFileBaseName       is the prefix of the qm files used to perform the translations.
     *                      It will be "heftyAD" if your qm files are "heftyAD_fr.qm", "heftyAD_en.qm", ...
     *
     * translationSource    corresponds to the native language of your app.
     *                      Basically the translation source is the language from which all texts are translated.
     *                      It is the one for which there is no need to define a qt translation file.
     *                      It can be "fr", "es", "de", ... and should correspond to one of the icons stored in the icon dir.
     *
     * saveLocation         corresponds to a name of a file where the language choosed by the user will be saved and loaded from.
     *                      It should be readable by a QTextStream (a plain text file) if the default
     *                      implementation of saveCurrentLangage() or languageLastlySaved() are used.
     *                      If a valid filename isn't given, this language picker won't merely perform load/save.
     */
    BasicLanguagePicker(const QString &iconDir, const QString &iconFileType,
                        const QString &qmFileDir, const QString &qmFileBaseName,
                        const QString &translationSource,
                        const QString &saveLocation,
                        QObject *parent = 0);
    ~BasicLanguagePicker();

    QString currentLanguage() const;
    void setCurrentLanguage(QString lang);

    /**
     * Opens and reads the save-location file, then returns the language lastly saved through saveCurrentLangage().
     * If there is no such a language, or if the one read is invalid
     * (i.e. it doesn't match any of the icons in the registered icon directory), an empty string is returned.
     *
     * Note: The registered save location file should be readable by a QTexttream (a plain text file).
     *       For this method to work with other kind of file, it should be overloaded in subclasses.
     */
    virtual QString languageLastlySaved() const;

    QStringList iconFilePaths() const;

protected:
    bool installLangage(const QString &lang);
    virtual void saveCurrentLangage();

    virtual void handleInstallationError(const QString &qmFile, const QString &qmDir);

private:
    void deleteMenuActions();

signals:
    void languageChanged(QString str);

public slots:
    /**
     * Loads icons from the icon dir,
     * using the file type registered for icons.
     */
    void loadIcons();

protected slots:
    void onMenuTriggered(QAction *action);
    virtual void retranslate();
};

#endif // BASICLANGUAGEPICKER_H
