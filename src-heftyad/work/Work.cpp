#include "Work.h"

Work::Work(QWidget *parent)
    : GridWidget(parent)
{
}

QString Work::filePath(void) const {return m_filePath;}
void Work::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
    m_filePath.replace("\\", "/"); // replace any (Windows-like) "\" by "/"
}

QString Work::fileName() const {return m_filePath.split("/").last();}
QString Work::shortName() const
{
    const QString &fname(fileName());
    return !fname.contains(".") ? fname : fname.split(".")[0];
}

bool Work::isSaved() const {return !isDirty() && !m_filePath.isEmpty();}

ncpp::UndoStack* Work::undoStack() const {return nullptr;}

QWidget* Work::editionForm() const {return nullptr;}
bool Work::isEditionEnabled() const {return false;}

QList<QAction*> Work::toolBarActions() const {return QList<QAction*>();}
void Work::retranslate() {}

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
