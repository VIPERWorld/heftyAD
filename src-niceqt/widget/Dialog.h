#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGridLayout>

class Dialog : public QDialog
{
    Q_OBJECT

protected:
    QGridLayout m_layout;

    QPoint m_fixedPos;
    bool m_hasFixedPositionFeature;

public:
    explicit Dialog(QWidget *parent = 0);

    void setHasFixedPositionFeature(bool value);
    void setFixedPos(QPoint pos);
    void setFixedPos(int xPos, int yPos);

protected:
    void moveEvent(QMoveEvent *event);

signals:

public slots:
};

#endif // DIALOG_H
