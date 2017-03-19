#ifndef SIMULATIONALGORITHMFILEVIEWER_H
#define SIMULATIONALGORITHMFILEVIEWER_H

#include "widget/fileviewer/BasicFileSystemViewer.h"

class SimulationAlgorithmFileViewer : public BasicFileSystemViewer
{
    Q_OBJECT

protected:
    QString m_selectedAlgorithm;

public:
    explicit SimulationAlgorithmFileViewer(QWidget *parent = 0);

    QString selectedAlgorithm() const;

signals:
    void selectedAlgorithmChanged(const QString &filePath);

public slots:
    void retranslate();

    void onSelectedEntryChanged(const QString &entryPath);
};

#endif // SIMULATIONALGORITHMFILEVIEWER_H
