// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QtMath>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QByteArray>
#include <QApplication>
#include <QPainter>
#include <QUndoStack>

#include "MapView.h"
#include "MapItems/MapItem.h"
#include "Models/LevelObjectsModel.h"
#include "Models/MapScene.h"
#include "Utils/Settings.h"
#include "Utils/WidgetUtils.h"
#include "Utils/Utils.h"

const char mimeType[] = "application/x-levelobject";

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent)
{
    settings_ = Settings::sharedInstance();
    setBackgroundBrush(Qt::lightGray);

    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
    setViewportUpdateMode(FullViewportUpdate);
    //setAlignment(Qt::AlignLeft | Qt::AlignTop);

    setScene(new MapScene(this));
    //setSceneRect(0, 0, 0, 0);

    updateGridPixmap();

    connect(settings_, SIGNAL(showGridChanged(bool)),
            this, SLOT(updateGridPixmap()));
    connect(settings_, SIGNAL(finalGridSizeChanged(QSizeF)),
            this, SLOT(updateGridPixmap()));
}

MapScene *MapView::mapScene() const
{
    return qobject_cast<MapScene *>(scene());
}

void MapView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);

    if (settings_->showGrid()) {
        QRectF r = sceneRect();

        QSizeF gridSize = settings_->finalGridSize();
        QSizeF grid(qMax(1.0, gridSize.width()),
                    qMax(1.0, gridSize.height()));
        QPointF shift(qFloor(r.left() / grid.width()) * grid.width(),
                      qFloor(r.top() / grid.height()) * grid.height());
        painter->translate(shift);
        r.translate(-shift);
        painter->fillRect(r, gridPixmap_);
    } else {
        painter->fillRect(sceneRect(), Qt::white);
    }
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

    LevelObject *newObject = obj->clone();
    newObject->setPosition(mapToScene(viewportTargetPos));
    connect(newObject, SIGNAL(modified()),
            mapScene(), SLOT(setModified()));
    MapItem *item = new MapItem(newObject);
    mapScene()->addItem(item);
    mapScene()->setModified(true);
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        MapItem *item = dynamic_cast<MapItem *>(itemAt(event->pos()));
        if (item) {
            if (qApp->keyboardModifiers() & Qt::ControlModifier) {
                item->setSelected(!item->selected());
                mapScene()->setSelectedLevelObject(nullptr);
            } else {
                if (!item->selected())
                    mapScene()->selectSingleItem(item);
            }
            startPos_ = event->pos();
            prevPos_ = event->pos();
            draggedItems_ = mapScene()->selectedItems();
            dragInitialBounds_ = mapScene()->itemsBoundingRect(draggedItems_);
            dragPrevBounds_ = dragInitialBounds_;
            dragState_ = AboutToDrag;
        } else {
            if (!(qApp->keyboardModifiers() & Qt::ShiftModifier))
                mapScene()->selectSingleItem(nullptr);
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
        dragState_ = NotDragging;
        if (selecting_) {
            selecting_ = false;
            selectActiveItems();
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
        // FIXME: optimize
        //QRect rect = selectionRect().normalized();
        //foreach (QGraphicsItem *item, scene()->items(mapToScene(rect))) {
        QRectF rect = selectionRect();
        foreach (QGraphicsItem *item, scene()->items()) {
            QPolygonF itemViewportBounds = mapFromScene(item->sceneBoundingRect());
            bool intersects = !itemViewportBounds.intersected(rect).isEmpty();
            MapItem *mapItem = dynamic_cast<MapItem *>(item);
            if (mapItem)
                mapItem->setActive(intersects);
        }
        scene()->update();
        return;
    }

    if (scrolling_) {
//        QPointF scenePos = mapToScene(event->pos());
//        QPointF prev = mapToScene(prevPos_);
//        QPointF diff = scenePos - prev;
//        scroll(diff.x(), diff.y());

        prevPos_ = event->pos();
        return;
    }

    if (dragState_ == AboutToDrag || dragState_ == Dragging) {
        if (dragState_ == AboutToDrag) {
            //qdbg << "MapView::mouseMoveEvent(): About to drag" << endl;
            if (qApp->keyboardModifiers() & Qt::AltModifier) {
                //qdbg << "MapView::mouseMoveEvent(): Cloned items" << endl;
                MapItems oldItems = draggedItems_;
                draggedItems_ = mapScene()->cloneItems(oldItems);
                mapScene()->selectItems(oldItems, false);
            }
            dragState_ = Dragging;
        } else {
            //qdbg << "MapView::mouseMoveEvent(): Dragging" << endl;
        }

        QRectF shiftedRect = dragInitialBounds_.translated(event->pos() - startPos_);
        QSizeF gridSize = settings_->finalGridSize();

        QRectF targetRect;
        if (settings_->snapToGrid())
            targetRect = snapToGrid(shiftedRect, gridSize, /* bothSides */ true);
        else
            targetRect = shiftedRect;

        MapScene *scene = qobject_cast<MapScene *>(this->scene());
        QPointF shift = targetRect.topLeft() - dragPrevBounds_.topLeft();
        foreach (MapItem *item, draggedItems_) {
            QPointF pos = item->levelObject()->position();
            item->levelObject()->setPosition(pos + shift);
            scene->setModified(true);
        }

        dragPrevBounds_ = targetRect;
    }
}

void MapView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Backspace:
        mapScene()->deleteSelectedItems();
        break;
    case Qt::Key_Right:
        moveOrCloneSelectedItemsBy(1, 0);
        break;
    case Qt::Key_Left:
        moveOrCloneSelectedItemsBy(-1, 0);
        break;
    case Qt::Key_Down:
        moveOrCloneSelectedItemsBy(0, 1);
        break;
    case Qt::Key_Up:
        moveOrCloneSelectedItemsBy(0, -1);
        break;
    }
}

void MapView::resetScene()
{
    MapScene *scene = mapScene();
    scene->setSelectedLevelObject(nullptr);
    scene->clear();
    if (scene->parent() == this)
        delete scene;
    scene = new MapScene(this);
    setScene(scene);
    emit sceneCreated(scene);
}

void MapView::selectActiveItems()
{
    QList<QGraphicsItem *> items = scene()->items();

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

    mapScene()->setSelectedLevelObject(nullptr);
}

void MapView::moveOrCloneSelectedItemsBy(int dx, int dy)
{
    if (qApp->keyboardModifiers() & Qt::AltModifier) {
        MapItems selectedItems = mapScene()->selectedItems();
        if (selectedItems.count() != 1)
            return; // Cloning multiple items is not supported
        foreach (MapItem *item, selectedItems) {
            // If there's a similar item nearby, destroy self
            QRectF itemRect = item->boundingRect().translated(
                        item->pos());
            QRectF targetRect = itemRect.translated(
                        dx * itemRect.width(),
                        dy * itemRect.height());
            qdbg << "itemPos=" << item->pos() << ", itemRect=" << itemRect << ", targetRect=" << targetRect << endl;
            // FIXME: why doesn't targeted selection work?
            QList<QGraphicsItem *> neighbours = scene()->items(targetRect.toRect(), Qt::ContainsItemBoundingRect);
            bool destroyedItem = false;
            foreach (QGraphicsItem *gi, neighbours) {
                MapItem *neighbour = dynamic_cast<MapItem *>(gi);
                if (!neighbour)
                    continue;
                if (neighbour->boundingRect().translated(neighbour->pos()) != targetRect)
                    continue;
                if (neighbour->name() != item->name())
                    continue;
                // Found a similar item, destroy this one
                mapScene()->deleteItem(item);
                neighbour->setSelected(true);
                destroyedItem = true;
            }
            if (destroyedItem)
                return;

            // Clone the item
            MapItem *item2 = new MapItem(*item);
            QPointF objPos = item2->levelObject()->position();
            objPos.setX(objPos.x() + dx * itemRect.width());
            objPos.setY(objPos.y() + dy * itemRect.height());
            item2->levelObject()->setPosition(objPos);
            mapScene()->addItem(item2);
            mapScene()->setModified(true);

            // Deselect the original one
            item->setSelected(false);
        }

        mapScene()->setSelectedLevelObject(nullptr);
    } else {
        foreach (MapItem *item, mapScene()->selectedItems()) {
            //item->moveBy(dx, dy);
            LevelObject *obj = item->levelObject();
            obj->moveBy(dx, dy);
        }
    }
}

void MapView::copy()
{

}

void MapView::paste()
{

}

void MapView::updateGridPixmap()
{
    //qdbg << "MapView::updateGridPixmap()" << endl;
    if (settings_->showGrid()) {
        QSize gridSize = settings_->finalGridSize().toSize();
        gridPixmap_ = QPixmap(gridSize);
        gridPixmap_.fill(Qt::white);
        QPainter painter(&gridPixmap_);
        //painter.drawPoint(0, 0);
        painter.setPen(Qt::darkGray);
        painter.drawLine(0, 0, gridSize.width(), 0);
        painter.drawLine(0, 0, 0, gridSize.height());
        //painter.drawPoint(gridSize.width(), gridSize.height());
    } else {
        gridPixmap_ = QPixmap();
    }

    scene()->update();
}

QRect MapView::selectionRect() const
{
    QPoint pos(mapFromGlobal(QCursor::pos()));
    return QRect(startPos_.x(), startPos_.y(),
                 pos.x() - startPos_.x(), pos.y() - startPos_.y());
}

void MapView::snapToGrid(qreal firstLine, qreal secondLine, qreal gridSize, qreal *newFirstLine, qreal *newSecondLine) const
{
    if (gridSize == 0) {
        if (newFirstLine)
            *newFirstLine = firstLine;
        if (newSecondLine)
            *newSecondLine = secondLine;

        return;
    }

    struct NearestLine {
        qreal line;
        qreal dist;
    };

    auto nearestGridLine = [](qreal line, qreal gridSize) -> NearestLine {
        Q_ASSERT(gridSize > 0);
        NearestLine first, second;
        first.line = qFloor(line / gridSize) * gridSize;
        first.dist = qAbs(line - first.line);
        second.line = first.line + gridSize;
        second.dist = qAbs(line - second.line);
        return first.dist < second.dist ? first : second;
    };

    if (firstLine == secondLine) {
        NearestLine nearest = nearestGridLine(firstLine, gridSize);
        if (newFirstLine)
            *newFirstLine = nearest.line;
        if (newSecondLine)
            *newSecondLine = nearest.line;

        return;
    }

    NearestLine firstNearest = nearestGridLine(firstLine, gridSize);
    NearestLine secondNearest = nearestGridLine(secondLine, gridSize);

    if (firstNearest.dist < secondNearest.dist) {
        qreal shift = firstNearest.line - firstLine;
        if (newFirstLine)
            *newFirstLine = firstNearest.line;
        if (newSecondLine)
            *newSecondLine = secondLine + shift;
    } else {
        qreal shift = secondNearest.line - secondLine;
        if (newFirstLine)
            *newFirstLine = firstLine + shift;
        if (newSecondLine)
            *newSecondLine = secondNearest.line;
    }
}

QRectF MapView::snapToGrid(const QRectF &rect, const QSizeF &grid, bool bothSides) const
{
    qreal right = bothSides ? rect.right() : rect.left();
    qreal bottom = bothSides ? rect.bottom() : rect.top();

    qreal newLeft = 0;
    qreal newRight = 0;
    qreal newTop = 0;
    qreal newBottom = 0;

    snapToGrid(rect.left(), right, grid.width(), &newLeft, bothSides ? &newRight : nullptr);
    snapToGrid(rect.top(), bottom, grid.height(), &newTop, bothSides ? &newBottom: nullptr);

    return bothSides
            ? QRectF(QPointF(newLeft, newTop), QPointF(newRight, newBottom))
            : QRectF(QPointF(newLeft, newTop), rect.size());
}
