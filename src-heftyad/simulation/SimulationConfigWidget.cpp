#include "Algorithm.h"
#include "AlgorithmPluginInterface.h"
#include "FakeAlgorithm.h"
#include "Model.h"
#include "SimulationAlgorithmPluginPicker.h"
#include "SimulationConfigWidget.h"
#include "SimulationHighlighter.h" // Should be included since m_view->setHighlighter(...)
                                   // needs to convert its parameter to subclass type.
#include "View.h"
#include "WorkHelper.h"

#include <QFileDialog>

SimulationConfigWidget::SimulationConfigWidget(QWidget *parent)
    : GridWidget(parent)
{
    m_diag = new SimulationAlgorithmPluginPicker(this);

    m_algorithm = nullptr;
    m_model = nullptr;
    m_view = nullptr;

    addWidget(&m_algorithmLineEdit, 0, 0);
    addWidget(&m_chooseAlgorithm,   0, 1);
    addWidget(&m_modelLineEdit,     1, 0);
    addWidget(&m_chooseModel,       1, 1);
    addWidget(&m_form,              2, 0, 1, 2);

    m_algorithmLineEdit.setReadOnly(true);
    m_algorithmLineEdit.setContextMenuPolicy(Qt::NoContextMenu);

    m_modelLineEdit.setReadOnly(true);
    m_modelLineEdit.setContextMenuPolicy(Qt::NoContextMenu);

    m_modelLineEdit.setVisible(false); // will be shown only if needed
    m_chooseModel.setVisible(false); // same as above

    connect(&m_chooseAlgorithm, &PushButton::pressed, this, &SimulationConfigWidget::onChooseAlgorithmButtonPressed);
    connect(&m_chooseModel,     &PushButton::pressed, this, &SimulationConfigWidget::onChooseModelButtonPressed);
}

SimulationConfigWidget::~SimulationConfigWidget()
{
    deleteAlgorithm();
    delete m_model;
    delete m_view;
}

Algorithm* SimulationConfigWidget::algorithm() const {return m_algorithm;}
Model* SimulationConfigWidget::model() const {return m_model;}
View* SimulationConfigWidget::modelView() const {return m_view;}

QString SimulationConfigWidget::algorithmPath() const {return m_algorithmPath;}
QString SimulationConfigWidget::modePath() const {return m_modelPath;}

void SimulationConfigWidget::retranslate()
{
    m_diag->retranslate();

    m_algorithmLineEdit.setPlaceholderText(trUtf8("Fichier décrivant l'algorithme"));
    m_algorithmLineEdit.setToolTip(m_algorithmLineEdit.placeholderText());

    m_modelLineEdit.setPlaceholderText(trUtf8("Modèle associé à l'algorithme"));
    m_modelLineEdit.setToolTip(m_modelLineEdit.placeholderText());

    m_chooseAlgorithm.setText(trUtf8("Choisir"));
    m_chooseModel.setText(trUtf8("Choisir"));
    m_form.retranslate();
}

void SimulationConfigWidget::deleteAlgorithm()
{
    /*
     * First make sure any highlighting data (that is an attribute of the current algorithm)
     * is first stopped (removed from the current view).
     * Otherwise view will try do draw stuffs on screen while the data is already deleted.
     *
     * Note: We could have just deleted the view prior to deleting algorithm
     */
    if(m_view) {
        m_view->stopHighlighting();
    }

    delete m_algorithm;
}

void SimulationConfigWidget::onChooseAlgorithmButtonPressed()
{
    if(m_diag->exec() == QDialog::Rejected) {
        return;
    }

    AlgorithmPluginInterface *plugin = m_diag->selectedPlugin();
    Algorithm *pluginAlgorithm = plugin->algorithmInstance();
//    Algorithm *pluginAlgorithm = new FakeAlgorithm;
    const QString &pluginPath(m_diag->selectedPluginPath());

    m_form.fillIn(plugin);

    m_algorithmPath = pluginPath;
    m_algorithmLineEdit.setText(m_algorithmPath.split("/").last());
    m_modelLineEdit.setVisible(pluginAlgorithm && pluginAlgorithm->requiresAModel());
    m_chooseModel.setVisible(m_modelLineEdit.isVisible());

    deleteAlgorithm();
    m_algorithm = pluginAlgorithm;
    if(!m_algorithm || !m_algorithm->requiresAModel()) {
        delete m_model;
        delete m_view;
        m_modelPath.clear();
        m_modelLineEdit.clear();
    }
    else {
        m_algorithm->setModel(m_model);
        if(m_view) {
            m_view->setHighlighter(m_algorithm->highlighter());
        }
    }
    emit algorithmChanged();
}

void SimulationConfigWidget::onChooseModelButtonPressed()
{
    QString modelDir = ""; // the common model dir (to be set later)

    QString lastDir = m_modelPath; // the directory of the previously selected model
    if(!lastDir.isEmpty()) {
        QStringList tmp = lastDir.split("/");
        tmp.removeLast(); // remove the file name
        lastDir = tmp.join("/");
    }

    const QString &dir = m_modelPath.isEmpty() ? modelDir : lastDir;
    const QString &filePath = QFileDialog::getOpenFileName(this, trUtf8("Choisir un modèle"), dir, "*.xml");
    if(filePath.isEmpty()) {
        return;
    }

    m_modelPath = filePath;
    m_modelLineEdit.setText(m_modelPath.split("/").last());

    delete m_model;
    delete m_view;
    m_view = WorkHelper::modelViewInstanceByFilePath(m_modelPath);
    if(m_view) {
        m_model = m_view->model();
        m_view->setHighlighter(m_algorithm->highlighter());
    }
    m_algorithm->setModel(m_model);
    emit modelChanged();
}
