// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QPainter>
#include <QStyleOptionFocusRect>
#include "LevelWidget.h"
#include "Data/LevelObject.h"
#include "Utils/WidgetUtils.h"
#include "Utils/Utils.h"

LevelWidget::LevelWidget(QWidget *parent)
    : QWidget(parent)
{
    qdbg << "LevelObjectWidget()" << endl;
    //WidgetUtils::setBackgroundColor(this, Qt::lightGray);
}

void LevelWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    if (obj_) {
        QTransform transf = painter.transform();
        transf.scale(flipX_ ? -1 : 1, flipY_ ? -1 : 1);
        painter.setTransform(transf);
        QImage image = obj_->image();
        painter.drawImage(flipX_ ? -image.width() : 0,
                          flipY_ ? -image.height() : 0,
                          image);
        painter.resetTransform();
    } else {
        painter.fillRect(rect(), Qt::gray);
        painter.drawText(rect(), Qt::AlignCenter | Qt::AlignVCenter, tr("No object"));
    }

    if (selected_) {
        painter.setPen(Qt::DashLine);
        painter.drawRect(rect());
    } else if (active_) {
        painter.setPen(Qt::DotLine);
        painter.drawRect(rect());
    }
}

QSize LevelWidget::sizeHint() const
{
    QSize size;
    if (obj_)
        size = obj_->image().size();
    if (size.isEmpty())
        size = QSize(100, 50);
    //qdbg << "LevelObjectWidget: sizeHint.w=" << size.width() << endl;
    return size;
}

void LevelWidget::moveEvent(QMoveEvent *event)
{
    QWidget::moveEvent(event);

    emit positionChanged();
}

LevelObject *LevelWidget::levelObject() const
{
    return obj_;
}

void LevelWidget::setLevelObject(LevelObject *obj)
{
    obj_ = obj;
    updateGeometry();
    update();
}

bool LevelWidget::selected() const
{
    return selected_;
}

void LevelWidget::setSelected(bool selected)
{
    if (selected_ == selected)
        return;
    selected_ = selected;
    update();
    emit selectedChanged(selected);
}

bool LevelWidget::active() const
{
    return active_;
}

void LevelWidget::setActive(bool active)
{
    if (active_ == active)
        return;
    active_ = active;
    update();
}

bool LevelWidget::flipX() const
{
    return flipX_;
}

void LevelWidget::setFlipX(bool flipX)
{
    if (flipX_ == flipX)
        return;
    flipX_ = flipX;
    update();
}

bool LevelWidget::flipY() const
{
    return flipY_;
}

void LevelWidget::setFlipY(bool flipY)
{
    if (flipY_ == flipY)
        return;
    flipY_ = flipY;
    update();
}
