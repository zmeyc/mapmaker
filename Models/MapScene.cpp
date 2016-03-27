// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QUndoStack>

#include "MapScene.h"
#include "Models/LevelObjectsModel.h"
#include "MapItems/MapItem.h"
#include "Commands/DeleteCommand.h"

MapScene::MapScene(QObject *parent)
    : QGraphicsScene(parent)
{
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
