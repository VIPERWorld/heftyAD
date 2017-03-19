#ifndef LANGUAGEPICKER_H
#define LANGUAGEPICKER_H

#include "others/BasicLanguagePicker.h"

class LanguagePicker : public BasicLanguagePicker
{
    Q_OBJECT

public:
    explicit LanguagePicker(const QString &iconDir, const QString &iconFileType,
                            const QString &qmFileDir, const QString &qmFileBaseName,
                            const QString &translationSource,
                            const QString &saveLocation,
                            QObject *parent = 0);

protected:
    void handleInstallationError(const QString &qmFile, const QString &qmDir) override;
    void handleInstallationError(const QString &title, const QString &text,
                                 const QString &informativeText, const QString &detailedText);

signals:

public slots:
};

#endif // LANGUAGEPICKER_H
