#ifndef ITEMBASICFORM_H
#define ITEMBASICFORM_H

#include "ItemForm.h"

#include <QDoubleSpinBox>
#include <QLabel>
#include <QPlainTextEdit>

class ItemBasicForm : public ItemForm
{
    Q_OBJECT

protected:
    QLabel m_valueLabel; QPlainTextEdit m_value;
    QLabel m_opacityLabel; QDoubleSpinBox m_opacity;
    QLabel m_posLabel; QDoubleSpinBox m_posX, m_posY;
    QLabel m_editorLabel; QDoubleSpinBox m_editorX, m_editorY;

public:
    explicit ItemBasicForm(QWidget *parent = 0);

    void setValueEditorPosEnabled(bool enabled);
    void retranslate() override;

protected:
    void clearFields();

    void registerItems() override;
    void unregisterItems() override;

signals:

private slots:
    void onItemValueChanged();
    void onItemOpacityChanged();
    void onItemPosChanged();
    void onItemValueEditorPosChanged();

    void onThisValueChanged();
    void onThisOpacityChanged();
    void onThisPosChanged();
    void onThisValueEditorPosChanged();
};

#endif // ITEMBASICFORM_H
