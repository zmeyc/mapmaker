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
    qdbg << "DeleteCommand::DeleteCommand()" << endl;
    QString name;
    LevelObject *obj = mapItem->levelObject();
    if (obj)
        name = obj->name();
    else
        name = "object";
    setText(QObject::tr("Delete") + " '" + name + "'");
}

void DeleteCommand::redo()
{
    qdbg << "DeleteCommand::redo()" << endl;
    scene_->removeItem(mapItem_);
}

void DeleteCommand::undo()
{
    qdbg << "DeleteCommand::undo()" << endl;
    scene_->addItem(mapItem_);
}
