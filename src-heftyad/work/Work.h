#ifndef WORK_H
#define WORK_H

#include "widget/GridWidget.h"

namespace ncpp {
class UndoStack;
}

class Work : public GridWidget
{
    Q_OBJECT

public:
    typedef enum {
        Compile,
        Export
    } ExtraFeature;

private:
    QString m_filePath;

public:
    explicit Work(QWidget *parent = 0);

    QString filePath() const;
    /**
     * Sets the file path.
     * This function does not load this work. It doesn't save it either.
     * So it's up to the programmer to ensure the constant consistency of this work' state.
     */
    void setFilePath(const QString &filePath);

    /**
     * Returns the file name from the file path.
     * Examples: ""                   --> ""
     *           "fileName"           --> "fileName"
     *           "dir/fileName"       --> "fileName"
     *           "dir1/dir2/fileName" --> "fileName"
     */
    QString fileName() const;
    /**
     * Returns the fileName() without its extension (if any).
     * Examples: ""                   --> ""
     *           "file"               --> "file"
     *           "file.xml"           --> "file"
     *           "file.extension.xml" --> "file.extension" not "file"
     */
    QString shortName() const;

    virtual bool isDirty() const = 0;
    virtual void setDirty(bool dirty) = 0;
    virtual ncpp::UndoStack* undoStack() const;

    virtual bool saveTo(const QString &filePath) = 0;
    virtual bool loadFrom(const QString &filePath) = 0;

    virtual QWidget* editionForm() const;
    virtual bool isEditionEnabled() const;

    virtual QList<QAction*> toolBarActions() const;
    virtual void retranslate();

    virtual bool allowsExtraFeature(int feature);
    void startExtraFeature(int feature);

protected:
    virtual void postSave();
    virtual void execExtraFeature(int feature);

signals:
    void dirtyChanged();
    void editionFormRequired(bool required);

public slots:
    bool save();
    bool load();
};

#endif // WORK_H
