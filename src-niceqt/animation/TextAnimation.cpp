#include "TextAnimation.h"

TextAnimation::TextAnimation(QObject *parent)
    : QObject(parent)
{
    m_isRunning = false;

    m_frequence = 30; // 20, 25, 30, 50 and 70 are preferred values.

    m_index = 0;
    m_advanceByCharacter = true;

    m_timer.setSingleShot(true);

    registerDefaultBreakPoints();

    connect(&m_timer, &QTimer::timeout,         this, &TextAnimation::advance);
    connect(this,     &TextAnimation::started,  this, &TextAnimation::onStarted);
    connect(this,     &TextAnimation::finished, this, &TextAnimation::onFinished);
}

TextAnimation::~TextAnimation()
{
}

bool TextAnimation::isRunning() const {return m_isRunning;}

bool TextAnimation::advanceByCharacter() const {return m_advanceByCharacter;}
void TextAnimation::setAdvanceByCharacter(bool value) {m_advanceByCharacter = value;}

QString TextAnimation::text() const {return m_text;}
void TextAnimation::setText(const QString &text, bool startAnimation)
{
    if(!m_isRunning) {
        m_text = text;
        m_index = 0;
        if(startAnimation) {
            start();
        }
    }
    else {
        m_waintingTexts.append(text);
    }
}

int TextAnimation::frequence() const {return m_frequence;}
void TextAnimation::setFrequence(int milliseconds) {m_frequence = milliseconds;}

QMap<QString, int> TextAnimation::breakPoints() const {return m_breakPoints;}
void TextAnimation::setBreakPoints(const QMap<QString, int> &map)
{
    m_breakPoints.clear();
    addBreakPoints(map);
}

void TextAnimation::addBreakPoint(const QString &str, int duration)
{
    if(!str.isEmpty() && duration>0) {
        m_breakPoints[str] = duration;
    }
}

void TextAnimation::addBreakPoints(const QMap<QString, int> &map)
{
    QMapIterator<QString, int> it(map);
    while(it.hasNext()) {
        it.next();
        addBreakPoint(it.key(), it.value());
    }
}

void TextAnimation::removeBreakPoint(const QString &str)
{
    m_breakPoints.remove(str);
}

void TextAnimation::removeBreakPoints(const QMap<QString, int> &map)
{
    QMapIterator<QString, int> it(map);
    while(it.hasNext()) {
        it.next();
        removeBreakPoint(it.key());
    }
}

int TextAnimation::extraBreakFor(const QString &str)
{
    int ms = 0; // number of milliseconds
    if(m_breakPoints.contains(str)) {
        ms = m_breakPoints[str];
    }

    return ms;
}

int TextAnimation::nextIndex(QString &nextBreakPoint) const
{
    int index = -1;
    for(const QString &ponct : m_breakPoints.keys()) {
        int tmp = m_text.indexOf(ponct, m_index);
        if(tmp != -1) {
            tmp += ponct.length();
            if(index==-1 || tmp<index) {
                index = tmp;
                nextBreakPoint = ponct;
            }
        }
    }

    if(index == -1) {
        index = m_index + 1;
    }

    return index;
}

void TextAnimation::advanceIndex(QString &nextBreakPoint)
{
    m_index = nextIndex(nextBreakPoint);
}

void TextAnimation::registerDefaultBreakPoints()
{
    QStringList ponctuations1 = QStringList() << "." << "!" << "?";
    for(const auto &ponct : ponctuations1) {
        m_breakPoints[ponct] = 1000;
    }

    QStringList ponctuations2 = QStringList() << "," << ";" << ":";
    for(const auto &ponct : ponctuations2) {
        m_breakPoints[ponct] = 500;
    }
}

void TextAnimation::start()
{
    if(!m_isRunning) {
        m_isRunning = true;
        emit started();
        advance();
    }
}

void TextAnimation::stop()
{
    if(m_isRunning) {
        m_isRunning = false;
        m_timer.stop();
        emit finished();
    }
}

void TextAnimation::stopAll()
{
    m_waintingTexts.clear();
    stop();
}

void TextAnimation::advance()
{
    if(m_index == m_text.size()) {
        stop();
        return;
    }

    QString nextBreakPoint;

    const int old = m_index;
    advanceIndex(nextBreakPoint);
    int n = m_index - old;
    if(m_advanceByCharacter && n != nextBreakPoint.length()) {
        n = 1;
        m_index = old+1;
        nextBreakPoint = ""; // ignore break point
    }

    emit nextSubStr(m_text.mid(old, n));
    emit nextTotalStr(m_text.mid(0, m_index+1));

    m_timer.start(m_frequence + extraBreakFor(nextBreakPoint));
}

void TextAnimation::onStarted() {}

void TextAnimation::onFinished()
{
    if(!m_waintingTexts.isEmpty()) {
        QString str = m_waintingTexts.first();
        m_waintingTexts.removeFirst();
        setText(str);
        start();
    }
}
