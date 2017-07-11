#include "RecentWorkWidget.h"

RecentWorkWidget::RecentWorkWidget(QWidget *parent)
    : GridWidget(parent)
{
    gridLayout()->setMargin(5);
    addWidget(&m_listWidget, 0, 0, 1, 2);
    addWidget(&m_openSelectedItems, 1, 0);
    addWidget(&m_removeSelectedItems, 1, 1);

    m_listWidget.setSelectionMode(QListWidget::ExtendedSelection);
    m_openSelectedItems.setEnabled(false);
    m_removeSelectedItems.setEnabled(false);
    setButtonTexts("Open", "Remove");

    connect(&m_listWidget, &QListWidget::itemDoubleClicked, this, &RecentWorkWidget::onItemDoubleClicked);
    connect(&m_listWidget, &QListWidget::itemSelectionChanged, this, &RecentWorkWidget::onItemSelectionChanged);
    connect(&m_openSelectedItems,   &PushButton::clicked, this, &RecentWorkWidget::onOpenSelectedItemsButtonClicked);
    connect(&m_removeSelectedItems, &PushButton::clicked, this, &RecentWorkWidget::onRemoveSelectedItemsButtonClicked);
}

RecentWorkWidget::~RecentWorkWidget()
{
}

void RecentWorkWidget::setButtonTexts(const QString &textForOpen, const QString &textForRemove)
{
    m_openSelectedItems.setText(textForOpen);
    m_removeSelectedItems.setText(textForRemove);
}

void RecentWorkWidget::addRecentFile(const QString &filePath)
{
    QString path = filePath;
    path.replace("\\", "/");

    QListWidgetItem *item = nullptr;
    if(!m_table.contains(path)) {
        item = new QListWidgetItem;
        item->setText(path.split("/").last());
//        item->setTextAlignment(Qt::AlignCenter);
        item->setToolTip(path);
        m_table[path] = item;
    }
    else {
        item = m_table[path];
        const int row = m_listWidget.row(item);
        m_listWidget.takeItem(row);
    }

    m_listWidget.insertItem(0, item);
}

void RecentWorkWidget::onItemDoubleClicked(QListWidgetItem *item)
{
    emit openFilesRequested(QStringList() << item->toolTip());
}

void RecentWorkWidget::onItemSelectionChanged()
{
    const bool enable = !m_listWidget.selectedItems().isEmpty();
    m_openSelectedItems.setEnabled(enable);
    m_removeSelectedItems.setEnabled(enable);
}

void RecentWorkWidget::onOpenSelectedItemsButtonClicked()
{
    QList<QListWidgetItem*> selectedItems = m_listWidget.selectedItems();
    QStringList filePaths;
    for(QListWidgetItem *item : selectedItems) {
        filePaths << item->toolTip();
    }

    emit openFilesRequested(filePaths);
}

void RecentWorkWidget::onRemoveSelectedItemsButtonClicked()
{
    QList<QListWidgetItem*> selectedItems = m_listWidget.selectedItems();
    while(!selectedItems.isEmpty()) {
        auto *item = selectedItems.takeFirst();
        m_table.remove(item->toolTip());
        delete item;
    }
}

