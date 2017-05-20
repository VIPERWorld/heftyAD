#include "ModelItem.h"

ModelItem::ModelItem(QObject *parent)
    : QObject(parent)
{
    m_value = "?";

    m_enabled = true;
    m_selected = false;
    m_opacity = 1.0;
    m_visible = true;

    m_brush = QBrush(Qt::white);
    m_pen   = QPen(Qt::black, 1.1);
    m_font  = QFont("Times New Roman", 15);
}

QString ModelItem::value() const {return m_value;}
void ModelItem::setValue(const QString &value, bool notify)
{
    if(m_value != value) {
        m_value = value;
        if(notify) {
            emit valueChanged();
        }
    }
}

bool ModelItem::isEnabled() const {return m_enabled;}
void ModelItem::setEnabled(bool enabled, bool notify)
{
    if(m_enabled != enabled) {
        m_enabled = enabled;
        if(notify) {
            emit enabledChanged();
        }
    }
}

bool ModelItem::isSelected() const {return m_selected;}
void ModelItem::setSelected(bool selected, bool notify)
{
    if(m_selected != selected) {
        m_selected = selected;
        if(notify) {
            emit selectedChanged();
        }
    }
}

qreal ModelItem::opacity() const {return m_opacity;}
void ModelItem::setOpacity(qreal opacity, bool notify)
{
    if(m_opacity != opacity) {
        m_opacity = opacity;
        if(notify) {
            emit opacityChanged();
        }
    }
}

bool ModelItem::isVisible() const {return m_visible;}
void ModelItem::setVisible(bool visible, bool notify)
{
    if(m_visible != visible) {
        m_visible = visible;
        if(notify) {
            emit visibilityChanged();
        }
    }
}

QBrush ModelItem::brush() const {return m_brush;}
QColor ModelItem::brushColor() const {return m_brush.color();}
QPixmap ModelItem::brushTexture() const {return m_brush.texture();}
void ModelItem::setBrush(const QBrush &brush, bool notify)
{
    m_brush = brush;
    if(notify) {
        emit brushChanged();
    }
}
void ModelItem::setBrush(const QGradient &gradient, bool notify) {setBrush(QBrush(gradient), notify);}
void ModelItem::setBrushColor(const QColor &color, bool notify)
{
    if(m_brush.color() != color) {
        m_brush.setColor(color);
        if(notify) {
            emit brushChanged();
        }
    }
}
void ModelItem::setBrushTexture(const QPixmap &pixmap, bool notify)
{
    m_brush.setTexture(pixmap);
    if(notify) {
        emit brushChanged();
    }
}

QPen ModelItem::pen() const {return m_pen;}
QColor ModelItem::penColor() const {return m_pen.color();}
Qt::PenStyle ModelItem::penStyle() const {return m_pen.style();}
qreal ModelItem::penWidth() const {return m_pen.widthF();}
void ModelItem::setPen(const QPen &pen, bool notify)
{
    m_pen = pen;
    if(notify) {
        emit penChanged();
    }
}
void ModelItem::setPenColor(const QColor &color, bool notify)
{
    if(m_pen.color() != color) {
        m_pen.setColor(color);
        if(notify) {
            emit penChanged();
        }
    }
}
void ModelItem::setPenStyle(int style, bool notify)
{
    bool validStyle = false;
    switch(style) {
    case Qt::NoPen: case Qt::SolidLine: case Qt::DashLine: case Qt::DotLine:
    case Qt::DashDotLine: case Qt::DashDotDotLine:
        validStyle = true;
        break;

    default:
        break;
    }

    if(validStyle && style!=m_pen.style()) {
        m_pen.setStyle((Qt::PenStyle)style);
        if(notify) {
            emit penChanged();
        }
    }
}
void ModelItem::setPenWidth(qreal width, bool notify)
{
    if(m_pen.widthF()!=width) {
        m_pen.setWidthF(width);
        if(notify) {
            emit penChanged();
        }
    }
}

QFont ModelItem::font() const{return m_font;}
QString ModelItem::fontFamily() const {return m_font.family();}
qreal ModelItem::fontPointSize() const {return m_font.pointSizeF();}
void ModelItem::setFont(const QFont &font, bool notify)
{
    m_font = font;
    if(notify) {
        emit fontChanged();
    }
}
void ModelItem::setFontFamily(const QString &family, bool notify)
{
    if(m_font.family() != family) {
        m_font.setFamily(family);
        if(notify) {
            emit fontChanged();
        }
    }
}
void ModelItem::setFontPointSize(qreal pointSize, bool notify)
{
    if(pointSize>=10. && m_font.pointSizeF()!=pointSize) {
        m_font.setPointSizeF(pointSize);
        if(notify) {
            emit fontChanged();
        }
    }
}

QPointF ModelItem::pos() const {return m_pos;}
void ModelItem::setPos(const QPointF &pos, bool notify)
{
    if(m_pos != pos) {
        m_pos = pos;
        if(notify) {
            emit posChanged();
        }
    }
}
void ModelItem::setPos(qreal x, qreal y, bool notify) {setPos(QPointF(x, y), notify);}
void ModelItem::moveBy(qreal dx, qreal dy, bool notify) {setPos(pos()+QPointF(dx, dy), notify);}

qreal ModelItem::x() const {return m_pos.x();}
void ModelItem::setX(qreal x, bool notify)
{
    if(m_pos.x() != x) {
        m_pos.setX(x);
        if(notify) {
            emit posChanged();
        }
    }
}

qreal ModelItem::y() const {return m_pos.y();}
void ModelItem::setY(qreal y, bool notify)
{
    if(m_pos.y() != y) {
        m_pos.setY(y);
        if(notify) {
            emit posChanged();
        }
    }
}

QPointF ModelItem::valueEditorPos() const {return m_valueEditorPos;}
void ModelItem::setValueEditorPos(const QPointF &pos, bool notify)
{
    if(m_valueEditorPos != pos) {
        m_valueEditorPos = pos;
        if(notify) {
            emit valueEditorPosChanged();
        }
    }
}
void ModelItem::setValueEditorPos(qreal x, qreal y, bool notify) {setValueEditorPos(QPointF(x, y), notify);}

void ModelItem::sendAttributeChanges()
{
    emit valueChanged();

    emit enabledChanged();
    emit selectedChanged();
    emit opacityChanged();
    emit visibilityChanged();

    emit brushChanged();
    emit penChanged();
    emit fontChanged();

    emit posChanged();

    emit valueEditorPosChanged();
}

void ModelItem::saveState(QVariantList &state) const
{
    state.clear(); // Since this class is the deepest one (in type hierarchy).

    state << m_value;
    state << m_enabled << m_selected << m_opacity << m_visible;
    state << m_brush << m_pen << m_font;
    state << m_pos;
    state << m_valueEditorPos;
}

int ModelItem::restoreState(const QVariantList &state)
{
    int i = 0;

    m_value     = state[i++].toString();

    m_enabled   = state[i++].toBool();
    m_selected  = state[i++].toBool();
    m_opacity   = state[i++].toReal();
    m_visible   = state[i++].toBool();

    m_brush     = state[i++].value<QBrush>();
    m_pen       = state[i++].value<QPen>();
    m_font      = state[i++].value<QFont>();

    m_pos       = state[i++].toPointF();

    m_valueEditorPos = state[i++].toPointF();

    return i;
}

qreal ModelItem::scenePenWidth() const
{
    qreal p = m_pen.widthF();
    if(isSelected()) {
        p += 1.5;
    }

    return p;
}

QRectF ModelItem::boundingRect() const
{
    const QRectF &r = rect();
    const qreal &rx = r.x();
    const qreal &ry = r.y();
    const qreal &rw = r.width();
    const qreal &rh = r.height();

    const qreal &adjust = extra()/2;
    return QRectF(rx-adjust, ry-adjust, rw+2*adjust, rh+2*adjust);
}

qreal ModelItem::extra() const {return scenePenWidth();}

QPainterPath ModelItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

QRectF ModelItem::rectToScene(const QRectF &rect) const {return rect;}
QRectF ModelItem::sceneRect() const {return rectToScene(rect());}
QRectF ModelItem::sceneBoundingRect() const {return rectToScene(boundingRect());}

bool ModelItem::intersects(const ModelItem &item) {return sceneBoundingRect().intersects(item.sceneBoundingRect());}
