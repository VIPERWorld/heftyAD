#ifndef ALGORITHMWORK_H
#define ALGORITHMWORK_H

#include "AlgorithmWorkContent.h"
#include "Work.h"

class AlgorithmWork : public Work
{
    Q_OBJECT

protected:
    AlgorithmWorkContent m_content;

public:
    explicit AlgorithmWork(QWidget *parent = 0);

    bool isDirty() const override;
    void setDirty(bool dirty) override;

    bool saveTo(const QString &filePath) override;
    bool loadFrom(const QString &filePath) override;

    void retranslate() override;

signals:

public slots:
};

#endif // ALGORITHMWORK_H
