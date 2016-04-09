// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef NEWITEMCOMMAND_H
#define NEWITEMCOMMAND_H

#include <QObject>
#include <QUndoCommand>

class QGraphicsScene;
class MapItem;

class NewItemCommand: public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    explicit NewItemCommand(QGraphicsScene *scene,
                            MapItem *mapItem,
                            QUndoCommand *parent = 0);
    ~NewItemCommand();

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

#endif // NEWITEMCOMMAND_H
