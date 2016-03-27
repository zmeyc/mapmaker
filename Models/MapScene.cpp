// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QUndoStack>
#include <QJsonArray>

#include "MapScene.h"
#include "Models/LevelObjectsModel.h"
#include "MapItems/MapItem.h"
#include "Commands/DeleteCommand.h"
#include "Utils/Settings.h"

MapScene::MapScene(QObject *parent)
    : QGraphicsScene(parent)
{
    settings_ = Settings::sharedInstance();

    undoStack_ = new QUndoStack(this);

    connect(LevelObjectsModel::sharedInstance(), SIGNAL(levelObjectUpdated(QString,LevelObject*)),
            this, SLOT(onLevelObjectUpdated(QString,LevelObject*)));
}

bool MapScene::modified() const
{
    return modified_;
}

void MapScene::setModified(bool modified)
{
    modified_ = modified;
}

QJsonArray MapScene::toJsonArray(bool selectedOnly) const
{
    QJsonArray objects;
    foreach (QGraphicsItem *item, items()) {
        MapItem *mapItem = dynamic_cast<MapItem *>(item);
        if (!mapItem)
            continue;
        if (selectedOnly &&!mapItem->selected())
            continue;
        LevelObject *obj = mapItem->levelObject();
        if (!obj)
            continue;

        QJsonObject json = obj->toJsonObject();
        objects.append(json);
    }
    return objects;
}

QUndoStack *MapScene::undoStack() const
{
    return undoStack_;
}

void MapScene::setUndoStack(QUndoStack *undoStack)
{
    if (undoStack_ && undoStack_->parent() == this)
        delete undoStack_;
    undoStack_ = undoStack;
}


QList<MapItem *> MapScene::selectedItems() const
{
    QList<MapItem *> items;
    foreach (QGraphicsItem *item, this->items()) {
        MapItem *mapItem = dynamic_cast<MapItem *>(item);
        if (mapItem && mapItem->selected())
            items.append(mapItem);
    }

    return items;
}

void MapScene::selectItems(const MapItems &items, bool select)
{
    foreach (MapItem *item, items) {
        item->setSelected(select);
    }
    setSelectedLevelObject(nullptr);
}

void MapScene::deleteSelectedItems()
{
    foreach (QGraphicsItem *item, items()) {
        MapItem *mapItem = dynamic_cast<MapItem *>(item);
        if (mapItem && mapItem->selected()) {
            deleteItem(mapItem);
        }
    }
}

void MapScene::deleteItem(MapItem *item)
{
    //delete item;
    setModified(true);

    QUndoCommand *deleteCommand = new DeleteCommand(this, item);
    undoStack()->push(deleteCommand);
}

MapScene::MapItems MapScene::cloneItems(const MapItems &items)
{
    MapItems result;
    result.reserve(items.size());
    foreach (MapItem *item, items) {
        MapItem *item2 = new MapItem(*item);
        addItem(item2);
        setModified(true);
        result.append(item2);
    }
    return result;
}

QRectF MapScene::itemsBoundingRect(const MapScene::MapItems &items) const
{
    QRectF rect;
    foreach (MapItem *item, items)
        rect = rect.united(item->boundingRect().translated(item->pos()));

    return rect;
}

void MapScene::setSelectedLevelObject(LevelObject *object)
{
    if (selectedLevelObject_ != object) {
        selectedLevelObject_ = object;
        emit selectedLevelObjectChanged(object);
        QSizeF size;
        if (object)
            size = object->size();
        settings_->setSelectedLevelObjectSize(size);
    }
}

void MapScene::selectSingleItem(MapItem *item)
{
    foreach (QGraphicsItem *child, items()) {
        MapItem *childMapItem = dynamic_cast<MapItem *>(child);
        if (childMapItem)
            childMapItem->setSelected(childMapItem == item);
    }

    setSelectedLevelObject(item ? item->levelObject() : nullptr);
}

void MapScene::onLevelObjectUpdated(const QString &name, LevelObject *obj)
{
    foreach (QGraphicsItem *item, items()) {
        MapItem *mapItem = dynamic_cast<MapItem *>(item);
        if (mapItem && mapItem->name() == name) {
            LevelObject *mapObj = mapItem->levelObject();
            if (!mapObj)
                continue;
            mapObj->setImage(obj->image());
        }
    }
}

