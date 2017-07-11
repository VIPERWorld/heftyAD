#ifndef RECENTWORKWIDGET_H
#define RECENTWORKWIDGET_H

#include "GridWidget.h"
#include "PushButton.h"

#include <QListWidget>

class RecentWorkWidget : public GridWidget
{
    Q_OBJECT

protected:
    QMap<QString, QListWidgetItem*> m_table;

    QListWidget m_listWidget;
    PushButton m_openSelectedItems;
    PushButton m_removeSelectedItems;

public:
    explicit RecentWorkWidget(QWidget *parent = 0);
    ~RecentWorkWidget();

    void setButtonTexts(const QString &textForOpen, const QString &textForRemove);

    void addRecentFile(const QString &filePath);

signals:
    void openFilesRequested(const QStringList &filePaths);

private slots:
    void onItemDoubleClicked(QListWidgetItem *item);
    void onItemSelectionChanged();
    void onOpenSelectedItemsButtonClicked();
    void onRemoveSelectedItemsButtonClicked();
};

#endif // RECENTWORKWIDGET_H
