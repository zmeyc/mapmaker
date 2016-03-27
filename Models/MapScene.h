// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>

class QUndoStack;
class LevelObject;
class MapItem;

class MapScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MapScene(QObject *parent = 0);

    bool modified() const;
    Q_SLOT void setModified(bool modified = true);

    QUndoStack *undoStack() const;
    void setUndoStack(QUndoStack *undoStack);

    void deleteSelectedItems();
    void deleteItem(MapItem *item);

signals:

public slots:

protected slots:
    void onLevelObjectUpdated(const QString &name, LevelObject *obj);

protected:

    bool modified_ = false;
    QUndoStack *undoStack_ = nullptr;
};

#endif // MAPSCENE_H
