#include <QGraphicsScene>
#include "DeleteCommand.h"
#include "MapItems/MapItem.h"
#include "Data/LevelObject.h"
#include "Utils/Utils.h"

DeleteCommand::DeleteCommand(QGraphicsScene *scene,
                             MapItem *mapItem,
                             QUndoCommand *parent)
    : QUndoCommand(parent)
    , scene_(scene)
    , mapItem_(mapItem)
{
    qerr << "DeleteCommand[" << ptrToString(this) <<
            "]::DeleteCommand(): mapItem=" << ptrToString(mapItem) <<
            ", scene=" << ptrToString(mapItem->scene()) << endl;

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

DeleteCommand::~DeleteCommand()
{
    qerr << "~DeleteCommand[" << ptrToString(this) <<
            "]::DeleteCommand(): mapItem=" << ptrToString(mapItem_) << endl;
    if (mapItem_ && false == mapItem_->ref.deref()) {
        if (!mapItem_->scene())
            delete mapItem_;
    }
}

void DeleteCommand::redo()
{
    qerr << "DeleteCommand[" << ptrToString(this) <<
            "]::redo(): mapItem=" << ptrToString(mapItem_) <<
            ", scene=" << ptrToString(mapItem_->scene()) << endl;
    if (mapItem_)
        scene_->removeItem(mapItem_);
}

void DeleteCommand::undo()
{
    qerr << "DeleteCommand[" << ptrToString(this) <<
            "]::undo(): mapItem=" << ptrToString(mapItem_) <<
            ", scene=" << ptrToString(mapItem_->scene()) << endl;
    if (mapItem_)
        scene_->addItem(mapItem_);
}

void DeleteCommand::onMapItemDestroyed()
{
    MapItem *mapItem = (MapItem *)sender();
    qerr << "DeleteCommand[" << ptrToString(this) <<
            "]::onMapItemDestroyed(): mapItem=" << ptrToString(mapItem) <<
            ", scene=" << ptrToString(mapItem->scene()) << endl;
    mapItem_ = nullptr; // destroyed by scene
}
