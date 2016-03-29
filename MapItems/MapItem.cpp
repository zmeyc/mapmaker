// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QPainter>
#include <QGraphicsEffect>
#include "MapItem.h"
#include "AppStyle.h"
#include "Data/LevelObject.h"
#include "GraphicsEffects/DockPointsGraphicsEffect.h"
#include "Utils/WidgetUtils.h"
#include "Utils/Utils.h"

MapItem::MapItem(LevelObject *obj, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , obj_(obj)
{
    commonInit();
}

MapItem::MapItem(const MapItem &other)
    : QGraphicsItem(other.parentItem())
    , obj_(other.obj_->clone())
{
    commonInit();
    active_ = other.active_;
    selected_ = other.selected_;
}

void MapItem::setLevelObject(LevelObject *obj)
{
    if (obj_) {
        disconnect(obj_, nullptr, this, nullptr);
        delete obj_;
    }
    obj_ = obj->clone();
    commonInit();
}


void MapItem::commonInit()
{
    Q_ASSERT(obj_);
    Q_ASSERT(!obj_->parent());

    name_ = obj_->name();
    obj_->setParent(this);
    setPos(obj_->position());
    connect(obj_, SIGNAL(positionChanged(QPointF)),
            this, SLOT(onPositionChanged(QPointF)));
    connect(obj_, SIGNAL(flipXChanged(bool)),
            this, SLOT(update()));
    connect(obj_, SIGNAL(flipYChanged(bool)),
            this, SLOT(update()));
    connect(obj_, SIGNAL(imageChanged()),
            this, SLOT(update()));

    connect(obj_, SIGNAL(willChangeSize(QSizeF)),
            this, SLOT(onWillChangeSize(QSizeF)));
    connect(obj_, SIGNAL(destroyed(QObject*)),
            this, SLOT(onLevelObjectDestroyed()));
}

QString MapItem::name() const
{
    return name_;
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

    if (selected_ || active_) {
        painter->save();

        int dpr = painter->device()->devicePixelRatio();
        AppStyle *style = AppStyle::sharedInstance();
        const QPen &pen = active_ ? style->activeItemPen(dpr)
                                  : style->selectedItemPen(dpr);
        painter->setPen(pen);

        QRectF frame = WidgetUtils::innerFrame(rect, pen, dpr);
        painter->drawRect(frame);

        painter->restore();
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
    if (selected_ == selected)
        return;

    if (selected && obj_ && !obj_->dockPoints().isEmpty()) {
        DockPointsGraphicsEffect *dockPointsEffect = new DockPointsGraphicsEffect;
        dockPointsEffect->setDockPoints(obj_->dockPoints());
        setGraphicsEffect(dockPointsEffect);
    } else {
        setGraphicsEffect(nullptr);
    }

    selected_ = selected;
    update();
    emit selectedChanged(selected);
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

void MapItem::onLevelObjectDestroyed()
{
    obj_ = nullptr;
}
