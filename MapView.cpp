// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QDragEnterEvent>
#include <QMimeData>
#include <QByteArray>
#include <QApplication>
#include <QPainter>

#include "MapView.h"
#include "MapItems/MapItem.h"
#include "Models/LevelObjectsModel.h"
#include "Models/MapScene.h"
#include "Controls/LevelWidget.h"
#include "Utils/Settings.h"
#include "Utils/WidgetUtils.h"
#include "Utils/Utils.h"

const char mimeType[] = "application/x-levelobject";

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent)
{
    Settings *settings = Settings::sharedInstance();
    setBackgroundBrush(Qt::lightGray);

    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
    setViewportUpdateMode(FullViewportUpdate);

    setScene(new MapScene(this));
    //setSceneRect(0, 0, 0, 0);

    connect(settings, SIGNAL(showGridChanged(bool)),
            this, SLOT(onShowGridChanged(bool)));
}

MapScene *MapView::mapScene() const
{
    return qobject_cast<MapScene *>(scene());
}

void MapView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);

    painter->fillRect(sceneRect(), Qt::white);
}

void MapView::drawForeground(QPainter *painter, const QRectF &rect)
{
    //qdbg << "MapView::drawForeground()" << endl;
    QGraphicsView::drawForeground(painter, rect);

    if (selecting_) {
        QPainter painter(viewport());
        painter.setPen(Qt::DashLine);
        painter.drawRect(selectionRect());
    }
}

void MapView::paintEvent(QPaintEvent *event)
{
    //qdbg << "MapView::paintEvent()" << endl;
    QGraphicsView::paintEvent(event);
}

void MapView::dragEnterEvent(QDragEnterEvent *event)
{
    //qdbg << "dragEvent: mime=" << event->mimeData()->formats().first() << endl;
    if (!event->mimeData()->hasFormat(mimeType))
        return;
    event->acceptProposedAction();
}

void MapView::dragMoveEvent(QDragMoveEvent *event)
{
    Q_UNUSED(event);
}

void MapView::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
}

void MapView::dropEvent(QDropEvent *event)
{
    qdbg << "dropEvent: text=" << event->mimeData()->formats().first() << endl;
    if (!event->mimeData()->hasFormat(mimeType))
        return;
    event->acceptProposedAction();

    QPoint dragOffset;
    QByteArray nameData;

    QByteArray encodedData = event->mimeData()->data(mimeType);
    QDataStream dataStream(&encodedData, QIODevice::ReadOnly);
    dataStream >> nameData >> dragOffset;
    QString name = QString::fromUtf8(nameData);
    LevelObject *obj = LevelObjectsModel::sharedInstance()->levelObjectByName(name);
    if (!obj)
        return;

    QSizeF objSize = obj->size();
    QPoint dragOffsetShifted(dragOffset.x() - objSize.width() / 2,
                             dragOffset.y() - objSize.height() / 2);

    QPoint viewportTargetPos(event->pos() - dragOffsetShifted);

    MapItem *item = new MapItem(obj->clone());
    item->levelObject()->setPosition(mapToScene(viewportTargetPos));
    scene()->addItem(item);

    modified_ = true;
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        MapItem *item = dynamic_cast<MapItem *>(itemAt(event->pos()));
        if (item) {
            if (qApp->keyboardModifiers() & Qt::ControlModifier) {
                item->setSelected(!item->selected());
                setSelectedLevelObject(item->levelObject());
            } else {
                if (!item->selected())
                    selectSingleItem(item);
            }
            prevPos_ = event->pos();
            dragging_ = true;
        } else {
            if (!(qApp->keyboardModifiers() & Qt::ShiftModifier))
                selectSingleItem(nullptr);
            startPos_ = event->pos();
            selecting_ = true;
        }
    } else if (event->button() == Qt::RightButton) {
        prevPos_ = event->pos();
        scrolling_ = true;
    }
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (event->button() == Qt::LeftButton) {
        dragging_ = false;
        if (selecting_) {
            selecting_ = false;
            selectActiveWidgets();
            scene()->update();
        }
    } else if (event->button() == Qt::RightButton) {
        scrolling_ = false;
    }
}

void MapView::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    if (selecting_) {
        QRectF rect = selectionRect();
        foreach (QGraphicsItem *item, scene()->items()) {
            const QPolygonF itemViewportBounds = mapFromScene(item->sceneBoundingRect());
            bool intersects = !itemViewportBounds.intersected(rect).isEmpty();
            MapItem *mapItem = dynamic_cast<MapItem *>(item);
            if (mapItem)
                mapItem->setActive(intersects);
        }
        scene()->update();
        return;
    }

    if (!dragging_ && !scrolling_)
        return;

    // Dragging and scrolling are similar right now,
    // except scrolling moves all widgets and dragging
    // moves only selected ones
    foreach (QGraphicsItem *item, scene()->items()) {
        MapItem *mapItem = dynamic_cast<MapItem *>(item);
        if (dragging_ && !mapItem->selected())
            continue;
        QPointF newPos = mapItem->levelObject()->position() + event->pos() - prevPos_;
        mapItem->levelObject()->setPosition(newPos);
        modified_ = true;
    }
    prevPos_ = event->pos();
}

void MapView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Backspace:
        deleteSelectedWidgets();
        break;
    }
}

bool MapView::modified() const
{
    return modified_;
}

void MapView::setModified(bool modified)
{
    modified_ = modified;
}

void MapView::selectActiveWidgets()
{
    const QList<QGraphicsItem *> items = scene()->items();

    MapItem *lastItem = nullptr;
    foreach (QGraphicsItem *item, items) {
        MapItem *mapItem = dynamic_cast<MapItem *>(item);
        if (!mapItem)
            continue;

        if (mapItem->active()) {
            mapItem->setActive(false);
            mapItem->setSelected(true);
            lastItem = mapItem;
        }
    }

    setSelectedLevelObject(lastItem ? lastItem->levelObject() : nullptr);
}

void MapView::deleteSelectedWidgets()
{
    foreach (LevelWidget *widget, findChildren<LevelWidget *>()) {
        if (widget->selected())
            delete widget;
    }
}

void MapView::setSelectedLevelObject(LevelObject *object)
{
    if (selectedLevelObject_ != object) {
        selectedLevelObject_ = object;
        emit selectedLevelObjectChanged(object);
    }
}

void MapView::selectSingleItem(MapItem *item)
{
    foreach (QGraphicsItem *child, scene()->items()) {
        MapItem *childMapItem = dynamic_cast<MapItem *>(child);
        if (childMapItem)
            childMapItem->setSelected(childMapItem == item);
    }

    setSelectedLevelObject(item ? item->levelObject() : nullptr);
}

void MapView::onShowGridChanged(bool showGrid)
{
    Q_UNUSED(showGrid);
    scene()->update();
}

QRect MapView::selectionRect() const
{
    QPoint pos(mapFromGlobal(QCursor::pos()));
    return QRect(startPos_.x(), startPos_.y(),
                 pos.x() - startPos_.x(), pos.y() - startPos_.y());
}
