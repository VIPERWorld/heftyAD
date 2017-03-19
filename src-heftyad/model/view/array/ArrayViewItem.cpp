#include "ArrayModel.h"
#include "ArrayModelItem.h"
#include "ArrayViewItem.h"

#include <QPainter>

ArrayViewItem::ArrayViewItem(QGraphicsItem *parent)
    : ViewMultiShapeItem(parent),
      m_arrayContainer(nullptr),
      m_drawItemIndexInArray(false)
{
}

bool ArrayViewItem::acceptModelItem(ModelItem *modelItem)
{
    return modelItem==nullptr || dynamic_cast<ArrayModelItem*>(modelItem);
}

void ArrayViewItem::setArrayContainer(ArrayModel *container) {m_arrayContainer = container;}
void ArrayViewItem::setDrawItemIndexInArray(bool value) {m_drawItemIndexInArray = value;}

void ArrayViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    ViewMultiShapeItem::paint(painter, option, widget);
    if(m_drawItemIndexInArray) {
        paintModelIndex(painter);
    }
}

void ArrayViewItem::paintModelIndex(QPainter *painter)
{
    const QPointF &p = rect().topLeft();
    const qreal &pX  = p.x()+4.;
    const qreal &pY  = p.y()+4.;
    const auto &rect = QRectF(pX, pY, 20., 20.);

//    // Make drawing transparent
//    QBrush b = painter->brush();
//    QColor c = b.color();
//    c.setAlpha(0); // fully transparent
//    b.setColor(c);
//    painter->setBrush(b);

    // First draw the shape that will contain the index of the model item of this view item.

    auto *item = static_cast<ArrayModelItem*>(m_modelItem);
    if(item) {
        switch(item->shapeKind()) {
        case ArrayModelItem::Circle:
            painter->drawEllipse(rect);
            break;
        case ArrayModelItem::Rectangle:
            item->isRounded() ? painter->drawRoundedRect(rect, 3., 3.) : painter->drawRect(rect);
            break;
        }
    }
    else {
        painter->drawRect(rect);
    }

    // Now draw the index in the shape drawn above.

    if(item && m_arrayContainer) {
        QFont font = painter->font();
            const qreal &k = font.pointSizeF()-5;
            font.setPointSize(k<=0 ? font.pointSizeF() : k);
            painter->setFont(font);

        painter->drawText(rect, Qt::AlignCenter, QString::number(m_arrayContainer->indexOf(*item)));
    }
}
