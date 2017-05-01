#ifndef BASICITEMFORM_H
#define BASICITEMFORM_H

#include "ItemForm.h"

#include <QDoubleSpinBox>
#include <QLineEdit>

class BasicItemForm : public ItemForm
{
    Q_OBJECT

protected:
    QLineEdit m_value;
    QLineEdit m_opacity;
    QDoubleSpinBox m_posX, m_posY;

public:
    explicit BasicItemForm(QWidget *parent = 0);

    void retranslate() override;

signals:

public slots:
};

#endif // BASICITEMFORM_H
