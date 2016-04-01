#ifndef DOCKPOINTSGRAPHICSEFFECT_H
#define DOCKPOINTSGRAPHICSEFFECT_H

#include <QGraphicsEffect>
#include <QVector>
#include <QPoint>

class DockPointsGraphicsEffect : public QGraphicsEffect
{
    Q_OBJECT
public:
    DockPointsGraphicsEffect(QObject *parent = nullptr);

    QRectF boundingRectFor(const QRectF &sourceRect) const override;
    void draw(QPainter *painter) override;

    void setDockPoints(const QVector<QPointF> &dockPoints);

    bool flipX() const;
    void setFlipX(bool flipX);

    bool flipY() const;
    void setFlipY(bool flipY);

signals:

public slots:

protected slots:

protected:
    QRectF dockPointRect(const QPointF &dockPoint, const QRectF &sourceRect) const;

    QVector<QPointF> dockPoints_;
    bool flipX_ = false;
    bool flipY_ = false;
};

#endif // DOCKPOINTSGRAPHICSEFFECT_H
