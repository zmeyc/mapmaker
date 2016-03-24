#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QObject>
#include <QUndoCommand>
#include "MapItems/MapItem.h"

class QGraphicsScene;

class DeleteCommand: public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    explicit DeleteCommand(QGraphicsScene *scene,
                           MapItem *mapItem,
                           QUndoCommand *parent = 0);
    ~DeleteCommand();

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

#endif // DELETECOMMAND_H
