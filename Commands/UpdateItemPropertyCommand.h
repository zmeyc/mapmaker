// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef UPDATEITEMPROPERTYCOMMAND_H
#define UPDATEITEMPROPERTYCOMMAND_H

#include <QObject>
#include <QUndoCommand>
#include <QVariant>

class QGraphicsScene;
class MapItem;

class UpdateItemPropertyCommand: public QObject, public QUndoCommand
{
    Q_OBJECT
public:
    explicit UpdateItemPropertyCommand(
            QGraphicsScene *scene,
            MapItem *mapItem,
            const QString &propertyName,
            const QVariant &propertyValue,
            const QVariant &previousValue,
            QUndoCommand *parent = 0);
    ~UpdateItemPropertyCommand();

    void redo() override;
    void undo() override;

signals:

public slots:

protected slots:
    void onMapItemDestroyed();

protected:
    QGraphicsScene *scene_ = nullptr;
    MapItem *mapItem_ = nullptr;
    QString propertyName_;
    QVariant propertyValue_;
    QVariant previousValue_;
};

#endif // UPDATEITEMPROPERTYCOMMAND_H
