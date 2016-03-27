// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>

class QUndoStack;
class LevelObject;

class MapScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MapScene(QObject *parent = 0);

    QUndoStack *undoStack() const;
    void setUndoStack(QUndoStack *undoStack);

signals:

public slots:

protected slots:
    void onLevelObjectUpdated(const QString &name, LevelObject *obj);

protected:
    QUndoStack *undoStack_ = nullptr;
};

#endif // MAPSCENE_H
