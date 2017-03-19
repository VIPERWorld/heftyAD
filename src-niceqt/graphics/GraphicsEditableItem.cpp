#include "GraphicsEditableItem.h"

#include <QPainter>
#include <QTextDocument>

GraphicsEditableItem::GraphicsEditableItem(QGraphicsItem *parent)
    : GraphicsItem(parent)
{
    m_textEditorFixed = false;
    m_textEditor.setParentItem(this);
    setEditable(false); // among others: to make sure the right textInteractionFlags is set

    connect(m_textEditor.document(), &QTextDocument::contentsChanged, [this](){setText(m_textEditor.toPlainText());});
}

QString GraphicsEditableItem::text() const {return m_text;}
void GraphicsEditableItem::setText(const QString &text) {
    if(m_text != text) {
        m_text = text;
        emit textChanged(m_text);
    }
}

void GraphicsEditableItem::setEditable(bool editable)
{
    m_textEditor.setVisible(editable);
    if(m_textEditor.isVisible()) {
        m_textEditor.setTextInteractionFlags(Qt::TextEditorInteraction);
        m_textEditor.setPlainText(m_text);
    }
    else {
        m_textEditor.setTextInteractionFlags(Qt::NoTextInteraction);
        setText(m_textEditor.toPlainText());
    }

    update();
}

void GraphicsEditableItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if(!m_textEditor.isVisible()) {
        painter->drawText(boundingRect(), Qt::AlignCenter, m_text);
    }
    else {
        if(m_textEditorFixed) {
            // Center the text editor considering that its bounding rect is centered around its origin (0, 0).
            m_textEditor.setPos((0-m_textEditor.boundingRect().width() )/2,
                                (0-m_textEditor.boundingRect().height())/2);
        }
    }
}
