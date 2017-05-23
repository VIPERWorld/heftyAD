#include "Work.h"

Work::Work(QWidget *parent)
    : GridWidget(parent),
      m_exportFeatureEnabled(false)
{
}

bool Work::isExportFeatureEnabled() const {return m_exportFeatureEnabled;}
void Work::setExportFeatureEnabled(bool enabled) {m_exportFeatureEnabled = enabled;}
void Work::startExportFeature() {if(m_exportFeatureEnabled) execExportDialog();}

QString Work::filePath() const {return m_filePath;}
void Work::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
    m_filePath.replace("\\", "/"); // replace any Windows-like file separator
}

QString Work::fileName() const {return !m_filePath.contains("/") ? m_filePath : m_filePath.split("/").last();}
QString Work::shortName() const
{
    const QString &fname = fileName();
    QStringList list = fname.split(".");
    if(list.size() >= 2) {
        list.removeLast();
    }

    return list.join(".");
}

ncpp::UndoStack* Work::undoStack() const {return nullptr;}

QWidget* Work::editionForm() const {return nullptr;}
bool Work::isEditionEnabled() const {return false;}

QList<QAction*> Work::toolBarActions() const {return QList<QAction*>();}
void Work::retranslate() {}

void Work::execExportDialog() {}

bool Work::save()
{
    const bool &retVal(saveTo(m_filePath));
    if(retVal) {
        setDirty(false);
    }
    return retVal;
}

bool Work::load()
{
    const bool &retVal(loadFrom(m_filePath));
    if(retVal) {
        setDirty(false);
    }
    return retVal;
}
