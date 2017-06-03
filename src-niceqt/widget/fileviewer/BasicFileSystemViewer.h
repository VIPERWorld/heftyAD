#ifndef BASICFILESYSTEMVIEWER_H
#define BASICFILESYSTEMVIEWER_H

#include "FileSystemViewer.h"

#include "widget/PushButton.h"

#include <QLineEdit>

/**
 * The BasicFileSystemViewer class represents a simple but handy file system viewer.
 * It's provided only for convenience.
 *
 * /!\ You shouldn't change this viewer' model, since a default file system model is already set.
 *     If you do change this viewer' model, your application will crash:
 *         especially when the model you set can't be statically-casted to QFileSystemModel.
 *     In addition, any model you set won't be initialized the same way the default-model would have been.
 */
class BasicFileSystemViewer : public FileSystemViewer
{
    Q_OBJECT

protected:
    QString m_selectedEntry;

    GridWidget m_rootPathCustomizer;
        QLineEdit m_rootPathCustomizerLineEdit;
        PushButton m_rootPathCustomizerButton;

public:
    explicit BasicFileSystemViewer(QWidget *parent = 0);

    QString selectedEntry() const;

signals:
    void selectedEntryChanged(const QString &entryPath);

public slots:
    QString entryRootPath() const;
    void setEntryRootPath(const QString &dirPath);

    QStringList entryFilters() const;
    void setEntryFilters(const QStringList &filters);

    void setRootPathCustomizerVisibility(bool visible);

protected slots:
    void onViewChanged(QAbstractItemView *old, QAbstractItemView *current);
    void onViewItemPressed(const QModelIndex &index);
    void onRootPathCustomizerButtonPressed();
};

#endif // BASICFILESYSTEMVIEWER_H
