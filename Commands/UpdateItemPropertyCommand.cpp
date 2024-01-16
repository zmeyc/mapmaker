// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QGraphicsScene>
#include "UpdateItemPropertyCommand.h"
#include "MapItems/MapItem.h"
#include "Data/LevelObject.h"
#include "Utils/Utils.h"

UpdateItemPropertyCommand::UpdateItemPropertyCommand(
        QGraphicsScene *scene,
        MapItem *mapItem,
        const QString &propertyName,
        const QVariant &propertyValue,
        const QVariant &previousValue,
        QUndoCommand *parent)
    : QUndoCommand(parent)
    , scene_(scene)
    , mapItem_(mapItem)
    , propertyName_(propertyName)
    , propertyValue_(propertyValue)
    , previousValue_(previousValue)
{
    qdbg << "UpdateItemPropertyCommand[" << ptrToString(this) <<
            "]::UpdateItemPropertyCommand(): mapItem=" << ptrToString(mapItem) <<
            ", scene=" << ptrToString(mapItem->scene()) << Qt::endl;

    mapItem->ref.ref();

    QString name;
    LevelObject *obj = mapItem->levelObject();
    if (obj) {
        name = obj->name();
    } else {
        name = "object";
    }

    QString actionName;
    if (propertyName == "position")
        actionName = tr("Move");
    else
        actionName = tr("Update");

    setText(actionName + " '" + name + "'");
    QObject::connect(mapItem_, SIGNAL(destroyed(QObject*)),
                     this, SLOT(onMapItemDestroyed()));

}

UpdateItemPropertyCommand::~UpdateItemPropertyCommand()
{
    qdbg << "~UpdateItemPropertyCommand[" << ptrToString(this) <<
            "]::UpdateItemPropertyCommand(): mapItem=" << ptrToString(mapItem_) << Qt::endl;
    if (mapItem_ && false == mapItem_->ref.deref()) {
        if (!mapItem_->scene())
            delete mapItem_;
    }
}

void UpdateItemPropertyCommand::redo()
{
    qdbg << "UpdateItemPropertyCommand[" << ptrToString(this) <<
            "]::redo(): mapItem=" << ptrToString(mapItem_) <<
            ", scene=" << ptrToString(mapItem_->scene()) << Qt::endl;
    if (mapItem_) {
        qdbg << "UpdateItemPropertyCommand::redo(): key=" << propertyName_ <<
                ", value=" << propertyValue_.toPointF() << Qt::endl;
        LevelObject *obj = mapItem_->levelObject();
        if (obj) {
            bool success = obj->setProperty(propertyName_.toUtf8().data(), propertyValue_);
            if (!success)
                qerr << "Property does not exist: " << propertyName_ << Qt::endl;
        }
    }
}

void UpdateItemPropertyCommand::undo()
{
    qdbg << "UpdateItemPropertyCommand[" << ptrToString(this) <<
            "]::undo(): mapItem=" << ptrToString(mapItem_) <<
            ", scene=" << ptrToString(mapItem_->scene()) << Qt::endl;
    if (mapItem_) {
        qdbg << "UpdateItemPropertyCommand::undo(): key=" << propertyName_ <<
                ", value=" << propertyValue_.toPointF() << Qt::endl;
        LevelObject *obj = mapItem_->levelObject();
        if (obj) {
            bool success = obj->setProperty(propertyName_.toUtf8().data(), previousValue_);
            if (!success)
                qerr << "Property does not exist: " << propertyName_ << Qt::endl;
        }
    }
}

void UpdateItemPropertyCommand::onMapItemDestroyed()
{
    MapItem *mapItem = (MapItem *)sender();
    qdbg << "UpdateItemPropertyCommand[" << ptrToString(this) <<
            "]::onMapItemDestroyed(): mapItem=" << ptrToString(mapItem) <<
            ", scene=" << ptrToString(mapItem->scene()) << Qt::endl;
    mapItem_ = nullptr; // destroyed by scene
}
