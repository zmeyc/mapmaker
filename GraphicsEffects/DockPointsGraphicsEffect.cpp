#include <Qpainter>
#include "DockPointsGraphicsEffect.h"
#include "MapItems/MapItem.h"
#include "Utils/Settings.h"
#include "Utils/Utils.h"

const int dockPointWidth = 2;

DockPointsGraphicsEffect::DockPointsGraphicsEffect(QObject *parent)
    : QGraphicsEffect(parent)
{
}

QRectF DockPointsGraphicsEffect::boundingRectFor(const QRectF &sourceRect) const
{
    QRectF rect = sourceRect;
    foreach (const QPointF &dockPoint, dockPoints_) {
        rect = rect.united(dockPointRect(dockPoint, sourceRect));
    }
    return rect;
}

void DockPointsGraphicsEffect::draw(QPainter *painter)
{
    drawSource(painter);

    //painter->fillRect(QRect(0, 0, 20, 20), Qt::red);

    QRectF sourceRect = sourceBoundingRect();

    foreach (const QPointF &dockPoint, dockPoints_) {
        QRectF rect = dockPointRect(dockPoint, sourceRect);
        //qdbg << "Drawing dock point: " << rect << endl;
        painter->fillRect(rect, Qt::red);
    }
}

void DockPointsGraphicsEffect::setDockPoints(const QVector<QPointF> &dockPoints)
{
    //qdbg << "DockPointsGraphicsEffect::setDockPoints: size=" << dockPoints.size() << endl;
    dockPoints_ = dockPoints;
    updateBoundingRect();
    update();
}

QRectF DockPointsGraphicsEffect::dockPointRect(const QPointF &dockPoint, const QRectF &sourceRect) const
{
    Q_UNUSED(sourceRect);

    QRectF rect(dockPoint.x() - dockPointWidth / 2,
               dockPoint.y() - dockPointWidth / 2,
               dockPointWidth,
               dockPointWidth);
    return rect;
}
