#include <QPainter>
#include "Data/LevelObject.h"
#include "MapItem.h"

MapItem::MapItem(LevelObject *obj, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , obj_(obj)
{
    Q_ASSERT(obj);
    Q_ASSERT(!obj->parent());

    obj->setParent(this);
    setPos(obj->position());
    connect(obj, SIGNAL(positionChanged(QPointF)),
            this, SLOT(onPositionChanged(QPointF)));
}

QRectF MapItem::boundingRect() const
{
    QSizeF size = obj_->image().size();
    if (size.isEmpty())
        size = QSizeF(100, 50);

    return QRectF(-size.width() / 2, -size.height() / 2,
                  size.width(), size.height());
}

void MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rect = boundingRect();

    painter->drawImage(rect.topLeft(), obj_->image());
    if (selected_) {
        painter->setPen(Qt::DashLine);
        painter->drawRect(rect);
    } else if (active_) {
        painter->setPen(Qt::DotLine);
        painter->drawRect(rect);
    }
}

void MapItem::setActive(bool active)
{
    if (active_ != active) {
        active_ = active;
        update();
        emit activeChanged(active);
    }
}

void MapItem::setSelected(bool selected)
{
    if (selected_ != selected) {
        selected_ = selected;
        update();
        emit selectedChanged(selected);
    }
}

void MapItem::onPositionChanged(const QPointF &pos)
{
    setPos(pos);
}
