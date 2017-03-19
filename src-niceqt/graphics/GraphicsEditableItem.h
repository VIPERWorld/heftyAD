#ifndef GRAPHICSEDITABLEITEM_H
#define GRAPHICSEDITABLEITEM_H

#include "GraphicsItem.h"
#include "GraphicsTextItem.h"

/* This class represents graphics items which display a text
 * which can be edited.
 */

class GraphicsEditableItem : public GraphicsItem
{
    Q_OBJECT

protected:
    QString m_text;
    GraphicsTextItem m_textEditor;
    bool m_textEditorFixed; // should be set in sub clases

public:
    explicit GraphicsEditableItem(QGraphicsItem *parent = 0);

    QString text() const;
    void setText(const QString &text);

    void setEditable(bool editable);

    /**
     * Since this item origin (0, 0) can change (for instance
     * it can be the item left corner or its center), you should change
     * its position in subclasses paint method, if you want it to be always correctly drawn.
     *
     * Note: If your item is centered around its origin (0, 0),
     * the id editor will always be drawn at the right position.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

signals:
    void textChanged(QString text);
};

#endif // GRAPHICSEDITABLEITEM_H
