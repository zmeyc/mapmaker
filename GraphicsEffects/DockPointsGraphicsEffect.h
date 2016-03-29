#ifndef DOCKPOINTSGRAPHICSEFFECT_H
#define DOCKPOINTSGRAPHICSEFFECT_H

#include <QGraphicsEffect>

class DockPointsGraphicsEffect : public QGraphicsEffect
{
    Q_OBJECT
public:
    DockPointsGraphicsEffect(QObject *parent = nullptr);

    QRectF boundingRectFor(const QRectF &sourceRect) const override;
    void draw(QPainter *painter) override;

signals:

public slots:
};

#endif // DOCKPOINTSGRAPHICSEFFECT_H
