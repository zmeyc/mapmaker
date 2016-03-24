#include <QUndoStack>

#include "MapScene.h"

MapScene::MapScene(QObject *parent)
    : QGraphicsScene(parent)
{
    undoStack_ = new QUndoStack(this);
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
