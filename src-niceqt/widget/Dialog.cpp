#include "Dialog.h"

#include <QMoveEvent>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent),
      m_hasFixedPositionFeature(false)
{
    m_layout.setMargin(0);
    m_layout.setSpacing(0);
    setLayout(&m_layout);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void Dialog::setHasFixedPositionFeature(bool value) {m_hasFixedPositionFeature = value;}

void Dialog::setFixedPos(QPoint pos)
{
    if(!m_hasFixedPositionFeature) {
        return;
    }

    //setWindowFlags(Qt::FramelessWindowHint);
    m_fixedPos = pos;
    move(m_fixedPos);
}

void Dialog::setFixedPos(int xPos, int yPos) {setFixedPos(QPoint(xPos, yPos));}

void Dialog::moveEvent(QMoveEvent *event)
{
    QDialog::moveEvent(event);

    if(!m_hasFixedPositionFeature) {
        return;
    }

    if(event->pos()!=m_fixedPos) {
        move(m_fixedPos);
    }
}
