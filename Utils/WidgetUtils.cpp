// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QWidget>
#include <QPalette>
#include <QPen>
#include "WidgetUtils.h"

WidgetUtils::WidgetUtils()
{

}

void WidgetUtils::setBackgroundColor(QWidget *widget, const QColor &color)
{
    QPalette pal(widget->palette());
    pal.setColor(QPalette::Background, color);
    widget->setAutoFillBackground(true);
    widget->setPalette(pal);
}

QRectF WidgetUtils::innerFrame(const QRectF &rect, const QPen &pen, int devicePixelRatio)
{
    qreal penWidth = pen.width();
    if (penWidth == 0) // QPen special case of cosmetic 1-pixel pen (deprecated mode)
        penWidth = 1;
    qreal halfPenWidth = penWidth;
    if (pen.isCosmetic())
        halfPenWidth /= devicePixelRatio;
    halfPenWidth /= 2;
    return rect.adjusted(halfPenWidth,
                         halfPenWidth,
                         -halfPenWidth,
                         -halfPenWidth);
}
