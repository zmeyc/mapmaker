// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef WIDGETUTILS_H
#define WIDGETUTILS_H

class QRectF;
class QPen;

class WidgetUtils
{
public:
    WidgetUtils();

    static void setBackgroundColor(QWidget *widget, const QColor &color);
    static QRectF innerFrame(const QRectF &rect, const QPen &pen, int devicePixelRatio);
};

#endif // WIDGETUTILS_H
