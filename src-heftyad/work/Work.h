#ifndef WORK_H
#define WORK_H

#include "widget/GridWidget.h"

namespace ncpp {
class UndoStack;
}

class Work : public GridWidget
{
    Q_OBJECT

private:
    QString m_filePath;

public:
    explicit Work(QWidget *parent = 0);

    QString filePath(void) const;
    void setFilePath(const QString &filePath);

    QString fileName() const;
    /**
     * Returns the file name, without its extension (if any).
     * Examples: "file.xml"     --> "file"
     *           "dir/file.xml" --> "file"
     *           ""             --> ""
     */
    QString shortName() const;

    bool isSaved() const;
    virtual bool isDirty() const = 0;
    virtual void setDirty(bool dirty) = 0;
    virtual ncpp::UndoStack* undoStack() const;

    virtual bool saveTo(const QString &filePath) = 0;
    virtual bool loadFrom(const QString &filePath) = 0;

    virtual QWidget* editionForm() const;
    virtual bool isEditionEnabled() const;

    virtual QList<QAction*> toolBarActions() const;
    virtual void retranslate();

signals:
    void dirtyChanged();
    void editionFormRequired(bool required);

public slots:
    bool save();
    bool load();
};

#endif // WORK_H
