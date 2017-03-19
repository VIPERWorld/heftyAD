#ifndef MODELWRITER_H
#define MODELWRITER_H

#include <QObject>

class Model;

class QIODevice;

class ModelWriter : public QObject
{
    Q_OBJECT

private:
    Model *m_model;

public:
    explicit ModelWriter(Model *model);

    Model* model() const;

    bool write(const QString &filePath);
    virtual bool write(QIODevice &out) = 0;

signals:

public slots:
};

#endif // MODELWRITER_H
