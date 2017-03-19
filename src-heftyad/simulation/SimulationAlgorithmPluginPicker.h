#ifndef SIMULATIONALGORITHMPLUGINPICKER_H
#define SIMULATIONALGORITHMPLUGINPICKER_H

#include "SimulationAlgorithmFileViewer.h"

#include "widget/Dialog.h"
#include "widget/PushButton.h"

#include <QComboBox>
#include <QLabel>

class AlgorithmPluginInterface;

class SimulationAlgorithmPluginPicker : public Dialog
{
    Q_OBJECT

    typedef enum {
        Cpp,
        JavaScript
    } Language;

protected:
    QComboBox m_languages;
    QLabel m_filter;
    SimulationAlgorithmFileViewer m_algorithmViewer;
    PushButton m_choose;

public:
    explicit SimulationAlgorithmPluginPicker(QWidget *parent = 0);

    QString selectedPluginPath() const;
    AlgorithmPluginInterface* selectedPlugin() const;

    int currentLanguage() const;

    void retranslate();

    static QString textFor(int language);

protected:
    void connectSignalsToSlots();
    void setEntryFilters(const QStringList &filters);

private:
    void addLanguages();

signals:

protected slots:
    void onCurrentLanguageChanged();
    void onSelectedAlgorithmChanged(QString filePath);
};

#endif // SIMULATIONALGORITHMPLUGINPICKER_H
