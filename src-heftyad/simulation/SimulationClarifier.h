#ifndef SIMULATIONCLARIFIER_H
#define SIMULATIONCLARIFIER_H

#include <QObject>

class SimulationClarifier : public QObject
{
    Q_OBJECT

public:
    typedef enum {
        NormalText,
        InformativeText,
        WarningText,
        ErrorText
    } TextKind;

public:
    explicit SimulationClarifier(QObject *parent = 0);

    void addText(const QString &text, int kind = NormalText);
    void addInfo(const QString &text);
    void addWarning(const QString &text);
    void addError(const QString &text);

    void removeTexts();

public:
    // older version

    void prepareNewSection() const;
    void addShadowMessage(int msgType, const QString &msg = QString(), int alinea = 0) const;
    void addShadowMessageNormal(const QString &msg = QString(), int alinea = 0) const;
    void addShadowMessageInfo(const QString &msg = QString(), int alinea = 0) const;
    void addShadowMessageWarning(const QString &msg = QString(), int alinea = 0) const;
    void addShadowMessageError(const QString &msg = QString(), int alinea = 0) const;
    void addMessage(const QString &msg = QString(), int alinea = 0) const;

    // new version

signals:
    void needText(const QString &text, int kind);
    void needTextRemoval();

    // older version

signals:
    void needNewSection(void) const;
    void needShadowMessage(int msgType, const QString &msg = QString(), int alinea = 0) const;
    void needMessage(const QString &info = QString(), int alinea = 0) const;

public slots:
};

#endif // SIMULATIONCLARIFIER_H
