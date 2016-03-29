// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef MAPITEM_H
#define MAPITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QSharedData>

class LevelObject;
class DockPointsGraphicsEffect;

class MapItem : public QObject, public QGraphicsItem, public QSharedData
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit MapItem(LevelObject *levelObject, QGraphicsItem *parent = nullptr);
    explicit MapItem(const MapItem &other);

    LevelObject *levelObject() const { return obj_; }
    void setLevelObject(LevelObject *obj);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

    bool active() const { return active_; }
    bool selected() const { return selected_; }

    QString name() const;

signals:
    void activeChanged(bool active);
    void selectedChanged(bool selected);

public slots:
    void setActive(bool active);
    void setSelected(bool selected);

protected slots:
    void update();
    void onPositionChanged(const QPointF &pos);
    void onWillChangeSize(const QSizeF &newSize);
    void onShowDockPointsChanged(bool showDockPoints);
    void onDockPointsChanged();
    void onLevelObjectDestroyed();

protected:
    void commonInit();
    DockPointsGraphicsEffect *dockPointsEffect();

    bool active_ = false;
    bool selected_ = false;
    LevelObject *obj_ = nullptr;
    QString name_;
};

#endif // MAPITEM_H
