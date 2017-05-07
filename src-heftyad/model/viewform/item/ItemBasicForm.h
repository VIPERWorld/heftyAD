#ifndef ITEMBASICFORM_H
#define ITEMBASICFORM_H

#include "ItemForm.h"

#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>

class ItemBasicForm : public ItemForm
{
    Q_OBJECT

protected:
    QLabel m_valueLabel; QLineEdit m_value;
    QLabel m_opacityLabel; QDoubleSpinBox m_opacity;
    QLabel m_posLabel; QDoubleSpinBox m_posX, m_posY;
    QLabel m_editorLabel; QDoubleSpinBox m_editorX, m_editorY;

public:
    explicit ItemBasicForm(QWidget *parent = 0);

    void retranslate() override;

signals:

public slots:
};

#endif // ITEMBASICFORM_H
