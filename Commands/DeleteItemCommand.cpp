// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QGraphicsScene>
#include "DeleteItemCommand.h"
#include "MapItems/MapItem.h"
#include "Data/LevelObject.h"
#include "Utils/Utils.h"

DeleteItemCommand::DeleteItemCommand(QGraphicsScene *scene,
                             MapItem *mapItem,
                             QUndoCommand *parent)
    : QUndoCommand(parent)
    , scene_(scene)
    , mapItem_(mapItem)
{
    qerr << "DeleteCommand[" << ptrToString(this) <<
            "]::DeleteCommand(): mapItem=" << ptrToString(mapItem) <<
            ", scene=" << ptrToString(mapItem->scene()) << Qt::endl;

    mapItem->ref.ref();

    QString name;
    LevelObject *obj = mapItem->levelObject();
    if (obj)
        name = obj->name();
    else
        name = "object";
    setText(QObject::tr("Delete") + " '" + name + "'");
    QObject::connect(mapItem_, SIGNAL(destroyed(QObject*)),
                     this, SLOT(onMapItemDestroyed()));

}

DeleteItemCommand::~DeleteItemCommand()
{
    qerr << "~DeleteCommand[" << ptrToString(this) <<
            "]::DeleteCommand(): mapItem=" << ptrToString(mapItem_) << Qt::endl;
    if (mapItem_ && false == mapItem_->ref.deref()) {
        if (!mapItem_->scene())
            delete mapItem_;
    }
}

void DeleteItemCommand::redo()
{
    qerr << "DeleteCommand[" << ptrToString(this) <<
            "]::redo(): mapItem=" << ptrToString(mapItem_) <<
            ", scene=" << ptrToString(mapItem_->scene()) << Qt::endl;
    if (mapItem_)
        scene_->removeItem(mapItem_);
}

void DeleteItemCommand::undo()
{
    qerr << "DeleteCommand[" << ptrToString(this) <<
            "]::undo(): mapItem=" << ptrToString(mapItem_) <<
            ", scene=" << ptrToString(mapItem_->scene()) << Qt::endl;
    if (mapItem_)
        scene_->addItem(mapItem_);
}

void DeleteItemCommand::onMapItemDestroyed()
{
    MapItem *mapItem = (MapItem *)sender();
    qerr << "DeleteCommand[" << ptrToString(this) <<
            "]::onMapItemDestroyed(): mapItem=" << ptrToString(mapItem) <<
            ", scene=" << ptrToString(mapItem->scene()) << Qt::endl;
    mapItem_ = nullptr; // destroyed by scene
}
