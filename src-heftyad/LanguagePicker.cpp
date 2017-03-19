#include "LanguagePicker.h"
#include "ShadowMessageBox.h"

LanguagePicker::LanguagePicker(const QString &iconDir, const QString &iconFileType,
                               const QString &qmFileDir, const QString &qmFileBaseName,
                               const QString &translationSource,
                               const QString &saveLocation,
                               QObject *parent)
    : BasicLanguagePicker(iconDir, iconFileType,
                          qmFileDir, qmFileBaseName,
                          translationSource,
                          saveLocation,
                          parent)
{
}

void LanguagePicker::handleInstallationError(const QString &qmFile, const QString &qmDir)
{
    const QString &title = trUtf8("Fichier de translation introuvable");

    const QString &text = trUtf8("Une erreur s'est produite lors du chargement d'un fichier de translation. "
                                 "Il est en effet nécessaire que le fichier %1 existe à l'emplacement %2.")
            .arg("<B style=\"color: blue\";>"+qmFile+"</B>")
            .arg("<B style=\"color: blue\";>"+qmDir +"</B>");

    const QString &informativeText = trUtf8("Merci de restaurer celui-ci ou de consulter notre site web.");

    const QString &detailedText = QString("%1\n%2").arg(qmFile).arg(qmDir);

    handleInstallationError(title, text, informativeText, detailedText);
}

void LanguagePicker::handleInstallationError(const QString &title, const QString &text,
                                                   const QString &informativeText, const QString &detailedText)
{
    ShadowMessageBox(nullptr)
    .withIcon(QMessageBox::Information)
    .withWindowTitle(title)
    .withTextFormat(Qt::RichText)
    .withText(text)
    .withInformativeText("<B>"+informativeText+"</B>")
    .withDetailedText(detailedText)
    .withStandardButtons(QMessageBox::Ok)
    .withDefaultButton(QMessageBox::Ok)
    .exec();
}
