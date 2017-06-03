#include "AlgorithmPluginInterface.h"
#include "Resource.h"
#include "SimulationAlgorithmPluginPicker.h"

#include <QPluginLoader>
#include <QTreeView>

SimulationAlgorithmPluginPicker::SimulationAlgorithmPluginPicker(QWidget *parent)
    : Dialog(parent)
{
    setWindowIcon(Resource::instance().windowIcon()); // actually done because no icon is shown, even when this widget has a parent
    setMinimumSize(500, 350);

    m_algorithmViewer.setView(new QTreeView); // the view will be deleted when the algorithm chooser will be (due to parent/child relation)
    ModelViewer::configure((QTreeView*)m_algorithmViewer.view());
    m_algorithmViewer.setEntryRootPath(Resource::instance().userAppPluginDir());
    //m_algorithmViewer.setEntriesFilters(QStringList() << "*.fileExtension"); // done when the current language changed

    m_choose.setEnabled(false);

    m_layout.setMargin(5);
    m_layout.setSpacing(5);
    m_algorithmViewer.layout()->setSpacing(m_layout.spacing());
    m_layout.addWidget(&m_languages, 0, 0);
    m_layout.addWidget(&m_filter, 0, 1, 1, 3);
    m_layout.addWidget(&m_algorithmViewer, 1, 0, 1, 4);
    m_layout.addWidget(&m_choose, 2, 3);

    addLanguages();
    onCurrentLanguageChanged(); // to make sure everything is OK for the first time
    connectSignalsToSlots();
}

QString SimulationAlgorithmPluginPicker::selectedPluginPath() const {return m_algorithmViewer.selectedAlgorithm();}

AlgorithmPluginInterface* SimulationAlgorithmPluginPicker::selectedPlugin() const
{
    AlgorithmPluginInterface *interface = nullptr;

    const QString &path(selectedPluginPath());
    switch(currentLanguage()) {
    case Cpp:
        if(!path.isEmpty()) {
            QPluginLoader loader(path);
            QObject *plugin = loader.instance();
            if(plugin) {
                auto *pluginCasted = qobject_cast<AlgorithmPluginInterface*>(plugin);
                if(pluginCasted) {
                    interface = pluginCasted;
                }
                /*
                 * If the instruction below was not commented, any algorithm instance,
                 * EVEN THOSE CREATED BEFORE the plugin loader is unloaded,
                 * would be no more valid.
                 *
                 * So we'll let Qt unloads plugins for us (see QPluginLoader::unload doc).
                 */
                //loader.unload();
            }
        }
        break;

    case JavaScript: // later: test qobject_cast<JavaScriptPluginInterface*>(...)
        break;
    }

    return interface;
}

int SimulationAlgorithmPluginPicker::currentLanguage() const {return m_languages.currentIndex();}

void SimulationAlgorithmPluginPicker::retranslate()
{
    setWindowTitle(trUtf8("Choisir un algorithme"));

    m_algorithmViewer.retranslate();
    m_choose.setText(trUtf8("Choisir"));
}

QString SimulationAlgorithmPluginPicker::textFor(int language)
{
    switch(language) {
    case Cpp:           return "C++";
    case JavaScript:    return "JavaScript";
    }

    return "";
}

void SimulationAlgorithmPluginPicker::connectSignalsToSlots()
{
    // we should use static_cast here: see doc
    connect(&m_languages, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SimulationAlgorithmPluginPicker::onCurrentLanguageChanged);

    connect(&m_algorithmViewer, &SimulationAlgorithmFileViewer::selectedAlgorithmChanged,
            this, &SimulationAlgorithmPluginPicker::onSelectedAlgorithmChanged);

    connect(&m_choose, &PushButton::pressed, this, &SimulationAlgorithmPluginPicker::accept);
}

void SimulationAlgorithmPluginPicker::setEntryFilters(const QStringList &filters)
{
    m_algorithmViewer.setEntryFilters(filters);
    m_filter.setText(filters.join(", "));
}

void SimulationAlgorithmPluginPicker::addLanguages()
{
    int lang = 0;
    QString text = textFor(lang);
    while(!text.isEmpty()) {
        m_languages.addItem(text);
        lang++;
        text = textFor(lang);
    }
}

void SimulationAlgorithmPluginPicker::onCurrentLanguageChanged()
{
    const int lang = m_languages.currentIndex();
    switch(lang) {
    case Cpp:           setEntryFilters(QStringList() << "*.dll" << "*.dylib" << "*.lib" << "*.so"); break;
    case JavaScript:    setEntryFilters(QStringList() << "*.xml");                                   break;
    }

    m_choose.setEnabled(false);
}

void SimulationAlgorithmPluginPicker::onSelectedAlgorithmChanged(QString filePath)
{
    Q_UNUSED(filePath)

    m_choose.setEnabled(false); // not really mandatory but we want the bouton disabled until selectedPlugin() returns
    m_choose.setEnabled(selectedPlugin() != nullptr);
}
