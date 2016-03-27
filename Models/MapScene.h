// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>

class Settings;
class QUndoStack;
class LevelObject;
class MapItem;

class MapScene : public QGraphicsScene
{
    Q_OBJECT
public:
    using MapItems = QList<MapItem *>;

    explicit MapScene(QObject *parent = 0);

    bool modified() const;
    Q_SLOT void setModified(bool modified = true);

    QJsonArray toJsonArray(bool selectedOnly = false) const;

    QUndoStack *undoStack() const;
    void setUndoStack(QUndoStack *undoStack);

    MapItems selectedItems() const;
    void selectItems(const MapItems &items, bool select);
    void selectAllItems(bool select);
    void deleteSelectedItems();
    void deleteItem(MapItem *item);
    MapItems cloneItems(const MapItems &items);

    QRectF itemsBoundingRect(const MapItems &items) const;

    LevelObject *selectedLevelObject() const { return selectedLevelObject_; }
    Q_SLOT void setSelectedLevelObject(LevelObject *object);

signals:
    void selectedLevelObjectChanged(LevelObject *item);

public slots:
    void selectSingleItem(MapItem *item);

protected slots:
    void onLevelObjectUpdated(const QString &name, LevelObject *obj);

protected:
    Settings *settings_ = nullptr;
    bool modified_ = false;
    QUndoStack *undoStack_ = nullptr;
    LevelObject *selectedLevelObject_ = nullptr;
};

#endif // MAPSCENE_H
