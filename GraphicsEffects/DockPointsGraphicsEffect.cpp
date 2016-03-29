#include "DockPointsGraphicsEffect.h"


DockPointsGraphicsEffect::DockPointsGraphicsEffect(QObject *parent)
    : QGraphicsEffect(parent)
{

}

QRectF DockPointsGraphicsEffect::boundingRectFor(const QRectF &sourceRect) const
{
    return sourceRect;
}

void DockPointsGraphicsEffect::draw(QPainter *painter)
{
    //drawSource(painter);
}
