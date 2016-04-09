// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef DELETEITEMCOMMAND_H
#define DELETEITEMCOMMAND_H

#include <QObject>
#include <QUndoCommand>

class QGraphicsScene;
class MapItem;

class DeleteItemCommand: public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    explicit DeleteItemCommand(QGraphicsScene *scene,
                           MapItem *mapItem,
                           QUndoCommand *parent = 0);
    ~DeleteItemCommand();

    void redo() override;
    void undo() override;

signals:

public slots:

protected slots:
    void onMapItemDestroyed();

protected:
    QGraphicsScene *scene_ = nullptr;
    MapItem *mapItem_ = nullptr;
};

#endif // DELETEITEMCOMMAND_H
