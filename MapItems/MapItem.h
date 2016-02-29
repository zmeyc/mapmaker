#ifndef MAPITEM_H
#define MAPITEM_H

#include <QObject>
#include <QGraphicsItem>

class LevelObject;

class MapItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit MapItem(LevelObject *levelObject, QGraphicsItem *parent = nullptr);

    LevelObject *levelObject() const { return obj_; }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

    bool active() const { return active_; }
    bool selected() const { return selected_; }

signals:
    void activeChanged(bool active);
    void selectedChanged(bool selected);

public slots:
    void setActive(bool active);
    void setSelected(bool selected);

protected slots:
    void update();
    void onPositionChanged(const QPointF &pos);

protected:
    bool active_ = false;
    bool selected_ = false;
    LevelObject *obj_ = nullptr;
};

#endif // MAPITEM_H
