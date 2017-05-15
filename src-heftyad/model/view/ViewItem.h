#ifndef VIEWITEM_H
#define VIEWITEM_H

#include "graphics/GraphicsEditableItem.h"

class ModelItem;

class ViewItem : public GraphicsEditableItem
{
    Q_OBJECT

protected:
    ModelItem *m_modelItem;

public:
    explicit ViewItem(QGraphicsItem *parent = nullptr);
    ~ViewItem();

    ModelItem* modelItem() const;
    void setModelItem(ModelItem *modelItem);
    virtual bool acceptModelItem(ModelItem *modelItem) = 0;

    QRectF rect() const; // not intended to be virtual since the model item already has the same function as virtual.
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    /**
     * Prepares the given painter so that it uses this item's attributes: brush, font, pen, etc.
     * The function can be called in sub-classes.
     */
    void preparePainting(QPainter *painter);

signals:
    void modelItemChanged();
    void editorHovered(bool state);

protected slots:
    virtual void onModelItemChanged();

private:
    void setModelItemValue();
    //
    void setModelItemEnabled();
    void setModelItemSelected();
    void setModelItemOpacity();
    void setModelItemVisibility();
    //
    void setModelItemPos();
    //
    void setModelItemValueEditorPos();

private slots:
    // when model attributes change

    void onModelItemDestroyed();

    void onModelItemValueChanged();

    void onModelItemEnabledChanged();
    void onModelItemSelectedChanged();
    void onModelItemOpacityChanged();
    void onModelItemVisibilityChanged();

    void onModelItemBrushChanged();
    void onModelItemPenChanged();
    void onModelItemFontChanged();

    void onModelItemPosChanged();

    void onModelItemValueEditorPosChanged();

    // when this class attributes change

    void onTextChanged();
    void onTextEditorPosChanged();
};

#endif // VIEWITEM_H
