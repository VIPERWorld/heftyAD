#ifndef ALGORITHMWORK_H
#define ALGORITHMWORK_H

#include "SciCodeEditor.h"
#include "Work.h"

#include <QTextEdit>
#include <QSplitter>

class AlgorithmWork : public Work
{
    Q_OBJECT

protected:
    QSplitter m_splitter;
        SciCodeEditor m_editor;
        QTextEdit m_console;
public:
    explicit AlgorithmWork(QWidget *parent = 0);

    bool allowsExtraFeature(int feature) override;

    bool isDirty() const override;
    void setDirty(bool dirty) override;

    bool saveTo(const QString &filePath) override;
    bool loadFrom(const QString &filePath) override;

    void retranslate() override;

protected:
    void setEditorInitialText();
    void execExtraFeature(int feature) override;

signals:

public slots:
};

#endif // ALGORITHMWORK_H
