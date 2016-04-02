// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QGraphicsScene>
#include "NewItemCommand.h"
#include "MapItems/MapItem.h"
#include "Data/LevelObject.h"
#include "Utils/Utils.h"

NewItemCommand::NewItemCommand(QGraphicsScene *scene,
                             MapItem *mapItem,
                             QUndoCommand *parent)
    : QUndoCommand(parent)
    , scene_(scene)
    , mapItem_(mapItem)
{
    qerr << "NewItemCommand[" << ptrToString(this) <<
            "]::NewItemCommand(): mapItem=" << ptrToString(mapItem) <<
            ", scene=" << ptrToString(mapItem->scene()) << endl;

    mapItem->ref.ref();

    QString name;
    LevelObject *obj = mapItem->levelObject();
    if (obj)
        name = obj->name();
    else
        name = "object";
    setText(QObject::tr("New") + " '" + name + "'");
    QObject::connect(mapItem_, SIGNAL(destroyed(QObject*)),
                     this, SLOT(onMapItemDestroyed()));

}

NewItemCommand::~NewItemCommand()
{
    qerr << "~NewItemCommand[" << ptrToString(this) <<
            "]::NewItemCommand(): mapItem=" << ptrToString(mapItem_) << endl;
    if (mapItem_ && false == mapItem_->ref.deref()) {
        if (!mapItem_->scene())
            delete mapItem_;
    }
}

void NewItemCommand::redo()
{
    qerr << "NewItemCommand[" << ptrToString(this) <<
            "]::redo(): mapItem=" << ptrToString(mapItem_) <<
            ", scene=" << ptrToString(mapItem_->scene()) << endl;
    if (mapItem_)
        scene_->addItem(mapItem_);
}

void NewItemCommand::undo()
{
    qerr << "NewItemCommand[" << ptrToString(this) <<
            "]::undo(): mapItem=" << ptrToString(mapItem_) <<
            ", scene=" << ptrToString(mapItem_->scene()) << endl;
    if (mapItem_)
        scene_->removeItem(mapItem_);
}

void NewItemCommand::onMapItemDestroyed()
{
    MapItem *mapItem = (MapItem *)sender();
    qerr << "NewItemCommand[" << ptrToString(this) <<
            "]::onMapItemDestroyed(): mapItem=" << ptrToString(mapItem) <<
            ", scene=" << ptrToString(mapItem->scene()) << endl;
    mapItem_ = nullptr; // destroyed by scene
}
