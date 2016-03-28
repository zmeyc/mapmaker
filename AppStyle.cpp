#include "AppStyle.h"

AppStyle::AppStyle(QObject *parent)
    : QObject(parent)
    , selectedItemPenWidthPt_(1)
    , activeItemPenWidthPt_(1)
{
    // Warning: selectionPen's width is in pixels (not in points)
    // because it's cosmetic. As devicePixelRation is not known at this
    // step, the width can't be set right now.
    selectedItemPen_.setColor(Qt::black);
    selectedItemPen_.setStyle(Qt::DashLine);
    selectedItemPen_.setCosmetic(true);

    activeItemPen_.setColor(Qt::black);
    activeItemPen_.setStyle(Qt::DotLine);
    activeItemPen_.setCosmetic(true);
}

AppStyle *AppStyle::sharedInstance()
{
    static AppStyle *instance = new AppStyle;
    return instance;
}

QPen AppStyle::selectedItemPen(int devicePixelRatio) const
{
    selectedItemPen_.setWidth(selectedItemPenWidthPt_ * devicePixelRatio);
    return selectedItemPen_;
}

QPen AppStyle::activeItemPen(int devicePixelRatio) const
{
    activeItemPen_.setWidth(activeItemPenWidthPt_ * devicePixelRatio);
    return activeItemPen_;
}
