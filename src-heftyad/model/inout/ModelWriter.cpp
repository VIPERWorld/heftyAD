#include "ModelWriter.h"

#include <QFile>

ModelWriter::ModelWriter(Model *model)
    : QObject(),
      m_model(model)
{
}

Model* ModelWriter::model() const {return m_model;}

bool ModelWriter::write(const QString &filePath)
{
    QFile out(filePath);
    if(!out.open(QIODevice::WriteOnly)) {
        return false;
    }

    return write(out);
}
