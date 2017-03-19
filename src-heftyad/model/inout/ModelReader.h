#ifndef MODELREADER_H
#define MODELREADER_H

#include <QObject>

class Model;

class QIODevice;

class ModelReader : public QObject
{
    Q_OBJECT

private:
    Model *m_model;

public:
    explicit ModelReader(Model *model);

    Model* model() const;

    bool read(const QString &filePath);
    virtual bool read(QIODevice &in) = 0;

signals:

public slots:
};

#endif // MODELREADER_H
