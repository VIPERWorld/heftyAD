#ifndef FILESYSTEMVIEWER_H
#define FILESYSTEMVIEWER_H

#include "ModelViewer.h"

#include <QFileSystemModel>

/**
 * The FileSystemViewer class represents a file viewer designed for file system.
 * It provides a simple way of viewing a QFileSystemModel (provided by default).
 * A view should later be set for the files to be shown.
 *
 * /!\ There is no need to change this viewer's model,
 *     since a default file system model is already provided.
 */

class FileSystemViewer : public ModelViewer
{
    Q_OBJECT

private:
    QFileSystemModel m_fsmodel; // set as this viewer's model

public:
    explicit FileSystemViewer(QWidget *parent = 0);

signals:

public slots:
};

#endif // FILESYSTEMVIEWER_H
