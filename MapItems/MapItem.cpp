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
    connect(obj, SIGNAL(flipXChanged(bool)),
            this, SLOT(update()));
    connect(obj, SIGNAL(flipYChanged(bool)),
            this, SLOT(update()));

    connect(obj, SIGNAL(willChangeSize(QSizeF)),
            this, SLOT(onWillChangeSize(QSizeF)));
}

QRectF MapItem::boundingRect() const
{
    QSizeF size = obj_->size();
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

    QImage image = obj_->image();
    if (image.isNull()) {
        painter->save();
        painter->setPen(Qt::black);
        painter->setBrush(Qt::gray);
        painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignCenter | Qt::AlignVCenter,
                          !obj_->name().isEmpty() ? obj_->name() : tr("No name"));
        painter->restore();
    } else {
        bool flipX = obj_->flipX();
        bool flipY = obj_->flipY();
        if (!flipX && !flipY) {
            painter->drawImage(rect.topLeft(), obj_->image());
        } else {
            painter->save();
            QTransform transf = painter->transform();
            transf.scale(flipX ? -1 : 1, flipY ? -1 : 1);
            painter->setTransform(transf);
            painter->drawImage(rect.topLeft(), image);
            painter->restore();
        }
    }

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

void MapItem::update()
{
    QGraphicsItem::update();
}

void MapItem::onPositionChanged(const QPointF &pos)
{
    setPos(pos);
}

void MapItem::onWillChangeSize(const QSizeF &newSize)
{
    Q_UNUSED(newSize);
    prepareGeometryChange();
}
