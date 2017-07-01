#include "Model.h"
#include "ModelReader.h"
#include "ModelWriter.h"

#include <QJSEngine>

Model::Model(QObject *parent)
    : QObject(parent),
      m_reader(nullptr),
      m_writer(nullptr),
      m_bgBrush(Qt::NoBrush),
      m_animationAcceleration(0),
      m_jsEngine(nullptr)
{
}

QBrush Model::backgroundBrush() const {return m_bgBrush;}
void Model::setBackgroundBrush(const QBrush &brush, bool notify)
{
    if(m_bgBrush != brush) {
        m_bgBrush = brush;
        if(notify) {
            emit backgroundBrushChanged();
        }
    }
}

void Model::sendAttributeChanges()
{
    emit backgroundBrushChanged();
}

int Model::animationAcceleration() const {return m_animationAcceleration;}
void Model::setAnimationAcceleration(int acc)
{
    if(acc<-100 || acc>100 || acc==m_animationAcceleration) {
        return;
    }

    m_animationAcceleration = acc;
}

void Model::setJSEngine(QJSEngine *engine)
{
    /*
     * We didn't provide any "engineChanged" signal since
     *     it would have been exposed to the the JS script environment
     *     and we really do not need such a signal.
     */

    if(m_jsEngine != engine) {
        m_jsEngine = engine;
        m_jsItems.clear();
    }
}

void Model::saveState(int version)    { Q_UNUSED(version)               }
bool Model::restoreState(int version) { Q_UNUSED(version) return false; }
void Model::discardStates() {}

QString Model::toString() const {return QString();}
QRectF Model::coverage() const {return QRectF();}

void Model::empty() {}
void Model::shuffle() {}
void Model::layout() {}

QJSValue Model::getJSProxyOf(QObject *object)
{
    if(!m_jsEngine) {
        return QJSValue();
    }

    if(!m_jsItems.contains(object)) {
        m_jsItems[object] = m_jsEngine->newQObject(object);
    }

    return m_jsItems[object];
}

bool Model::saveTo(const QString &filePath, ModelWriter &writer)
{
    return writer.write(filePath);
}
bool Model::saveTo(const QString &filePath) {return !m_writer ? false : saveTo(filePath, *m_writer);}

bool Model::loadFrom(const QString &filePath, ModelReader &reader)
{
    return reader.read(filePath);
}
bool Model::loadFrom(const QString &filePath) {return !m_reader ? false : loadFrom(filePath, *m_reader);}
