#include "ItemBasicForm.h"

ItemBasicForm::ItemBasicForm(QWidget *parent)
    : ItemForm(parent)
{
    // add widgets

    addRow(&m_valueLabel,   &m_value);
    addRow(&m_opacityLabel, &m_opacity);
    addRow(&m_posLabel,     QWidgetList() << &m_posX << &m_posY, Qt::Horizontal);
    addRow(&m_editorLabel,  QWidgetList() << &m_editorX << &m_editorY, Qt::Horizontal);

    // customize widgets

    QWidgetList widgets = QWidgetList() << &m_value << &m_opacity << &m_posX << &m_posY << &m_editorX << &m_editorY;
    for(auto *w : widgets) {
        w->setContextMenuPolicy(Qt::NoContextMenu);
    }

    m_opacity.setRange(0, 1);
    m_opacity.setSingleStep(0.01);

    const int M = 1e6;
    QList<QDoubleSpinBox*> spins = QList<QDoubleSpinBox*>() << &m_posX << &m_posY << &m_editorX << &m_editorY;
    for(auto *s : spins) {
        s->setRange(-M, M);
    }

    const QString X = "X = ";
    const QString Y = "Y = ";
    m_posX.setPrefix(X);
    m_posY.setPrefix(Y);
    m_editorX.setPrefix(X);
    m_editorY.setPrefix(Y);
}

void ItemBasicForm::retranslate()
{
    m_valueLabel.setText(trUtf8("Valeur"));
    m_opacityLabel.setText(trUtf8("Opacité"));
    m_posLabel.setText(trUtf8("Position"));
    m_editorLabel.setText(trUtf8("Editeur"));
}
