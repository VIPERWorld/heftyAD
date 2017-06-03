#include "SimulationAlgorithmFileViewer.h"

SimulationAlgorithmFileViewer::SimulationAlgorithmFileViewer(QWidget *parent)
    : BasicFileSystemViewer(parent)
{
    setRootPathCustomizerVisibility(true);

    connect(this, &BasicFileSystemViewer::selectedEntryChanged, this, &SimulationAlgorithmFileViewer::onSelectedEntryChanged);
}

QString SimulationAlgorithmFileViewer::selectedAlgorithm() const {return m_selectedAlgorithm;}

void SimulationAlgorithmFileViewer::retranslate()
{
    m_rootPathCustomizerLineEdit.setPlaceholderText(trUtf8("Répertoire racine des plugins"));
    m_rootPathCustomizerLineEdit.setToolTip(trUtf8("Répertoire à partir duquel les entrées sont lues"));

    m_rootPathCustomizerButton.setText(trUtf8("Changer"));
}

void SimulationAlgorithmFileViewer::onSelectedEntryChanged(const QString &entryPath)
{
    auto *fsmodel = static_cast<QFileSystemModel*>(m_model);
    if(fsmodel->isDir(fsmodel->index(entryPath))) {
        m_selectedAlgorithm = "";
    }
    else {
        m_selectedAlgorithm = entryPath;
    }

    emit selectedAlgorithmChanged(m_selectedAlgorithm);
}
