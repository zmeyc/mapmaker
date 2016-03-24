#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QUndoCommand>

class QGraphicsScene;
class MapItem;

class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(QGraphicsScene *scene,
                           MapItem *mapItem,
                           QUndoCommand *parent = 0);

    void redo() override;
    void undo() override;

signals:

public slots:

protected:
    QGraphicsScene *scene_ = nullptr;
    MapItem *mapItem_ = nullptr;
};

#endif // DELETECOMMAND_H
