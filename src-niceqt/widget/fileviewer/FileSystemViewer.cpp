#include "FileSystemViewer.h"

FileSystemViewer::FileSystemViewer(QWidget *parent)
    : ModelViewer(parent)
{
    setModel(&m_fsmodel);
}
