#include "BasicFileSystemViewer.h"

#include <QAbstractItemView>
#include <QFileDialog>

BasicFileSystemViewer::BasicFileSystemViewer(QWidget *parent)
    : FileSystemViewer(parent)
{
    addWidget(&m_rootPathCustomizer, 1, 0);
    setRootPathCustomizerVisibility(false); // should be set after the root path widget customizer is added

    m_rootPathCustomizer.addWidget(&m_rootPathCustomizerLineEdit, 0, 0);
    m_rootPathCustomizer.addWidget(&m_rootPathCustomizerButton, 0, 1);

    m_rootPathCustomizerLineEdit.setReadOnly(true);
    m_rootPathCustomizerLineEdit.setContextMenuPolicy(Qt::NoContextMenu);
    m_rootPathCustomizerLineEdit.setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred));

    auto *fsmodel = static_cast<QFileSystemModel*>(m_model);
    fsmodel->setFilter(QDir::Files|QDir::NoDotAndDotDot|QDir::AllDirs);
    fsmodel->setNameFilterDisables(false);

    connect(this, &FileSystemViewer::viewChanged, this, &BasicFileSystemViewer::onViewChanged);
    connect(&m_rootPathCustomizerButton, &PushButton::pressed, this, &BasicFileSystemViewer::onRootPathCustomizerButtonPressed);
}

QString BasicFileSystemViewer::selectedEntry() const {return m_selectedEntry;}

QString BasicFileSystemViewer::entryRootPath() const {return static_cast<QFileSystemModel*>(m_model)->rootPath();}
void BasicFileSystemViewer::setEntryRootPath(const QString &dirPath)
{
    auto *fsmodel = static_cast<QFileSystemModel*>(m_model);
    fsmodel->setRootPath(dirPath);
    if(m_view) {
        m_view->setRootIndex(fsmodel->index(dirPath));
    }

    m_rootPathCustomizerLineEdit.setText(dirPath);
}

QStringList BasicFileSystemViewer::entryFilters() const {return static_cast<QFileSystemModel*>(m_model)->nameFilters();}
void BasicFileSystemViewer::setEntryFilters(const QStringList &filters)
{
    auto *fsmodel = static_cast<QFileSystemModel*>(m_model);
    fsmodel->setNameFilters(filters);
}

void BasicFileSystemViewer::setRootPathCustomizerVisibility(bool visible)
{
    m_rootPathCustomizer.setVisible(visible);
}

void BasicFileSystemViewer::onViewChanged(QAbstractItemView *old, QAbstractItemView *current)
{
    Q_UNUSED(current) // current is same as m_view

    if(old) {
        disconnect(old, &QAbstractItemView::pressed, this, &BasicFileSystemViewer::onViewItemPressed);
    }

    connect(m_view, &QAbstractItemView::pressed, this, &BasicFileSystemViewer::onViewItemPressed);
}

void BasicFileSystemViewer::onViewItemPressed(const QModelIndex &index)
{
    auto *fsmodel = static_cast<QFileSystemModel*>(m_model);
    if(m_selectedEntry != fsmodel->filePath(index)) {
        m_selectedEntry = fsmodel->filePath(index);
        emit selectedEntryChanged(m_selectedEntry);
    }
}

void BasicFileSystemViewer::onRootPathCustomizerButtonPressed()
{
    QFileDialog diag(this);
    diag.setDirectory(m_rootPathCustomizerLineEdit.text());
    diag.setFileMode(QFileDialog::Directory);
    diag.setOption(QFileDialog::ShowDirsOnly, true);

    if(diag.exec()) {
        setEntryRootPath(diag.selectedFiles().last());
    }
}
