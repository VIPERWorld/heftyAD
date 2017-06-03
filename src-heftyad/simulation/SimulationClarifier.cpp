#include "SimulationClarifier.h"

SimulationClarifier::SimulationClarifier(QObject *parent)
    : QObject(parent)
{
}

void SimulationClarifier::addText(const QString &text, int kind) {emit needText(text, kind);}
void SimulationClarifier::addInfo(const QString &text) {addText(text, InformativeText);}
void SimulationClarifier::addWarning(const QString &text) {addText(text, WarningText);}
void SimulationClarifier::addError(const QString &text) {addText(text, ErrorText);}

void SimulationClarifier::removeTexts() {emit needTextRemoval();}

void SimulationClarifier::prepareNewSection() const
{
    emit needNewSection();
}

void SimulationClarifier::addShadowMessage(int msgType, const QString &msg, int alinea) const
{
    emit needShadowMessage(msgType, msg, alinea);
}

void SimulationClarifier::addShadowMessageNormal(const QString &msg, int alinea) const  {addShadowMessage(NormalText,      msg, alinea);}
void SimulationClarifier::addShadowMessageInfo(const QString &msg, int alinea) const    {addShadowMessage(InformativeText, msg, alinea);}
void SimulationClarifier::addShadowMessageWarning(const QString &msg, int alinea) const {addShadowMessage(WarningText,     msg, alinea);}
void SimulationClarifier::addShadowMessageError(const QString &msg, int alinea) const   {addShadowMessage(ErrorText,       msg, alinea);}

void SimulationClarifier::addMessage(const QString &msg, int alinea) const
{
    emit needMessage(msg, alinea);
}
