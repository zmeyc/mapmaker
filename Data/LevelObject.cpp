// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include "LevelObject.h"
#include "Utils/FileUtils.h"

LevelObject::LevelObject(QObject *parent)
    : QObject(parent)
{

}

LevelObject *LevelObject::clone(QObject *parent)
{
    LevelObject *obj = new LevelObject(parent);
    obj->name_ = name_;
    obj->filename_ = filename_;
    obj->image_ = image_;
    obj->position_ = position_;
    obj->flipX_ = flipX_;
    obj->flipY_ = flipY_;
    return obj;
}

QString LevelObject::name() const
{
    return name_;
}

void LevelObject::setName(const QString &name)
{
    name_ = name;
}

QString LevelObject::filename() const
{
    return filename_;
}

void LevelObject::setFilename(const QString &filename)
{
    filename_ = filename;
    if (name_.isEmpty())
        name_ = nameFromFilename(filename);
}

QImage LevelObject::image() const
{
    return image_;
}

void LevelObject::setImage(const QImage &image)
{
    image_ = image;
}

QPointF LevelObject::position() const
{
    return position_;
}

void LevelObject::setPosition(const QPointF &pos)
{
    if (position_ != pos) {
        position_ = pos;
        emit positionChanged(pos);
    }
}

void LevelObject::setPosition(qreal x, qreal y)
{
    setPosition(QPointF(x, y));
}

QSizeF LevelObject::size() const
{
    return image_.size();
}

bool LevelObject::flipX() const
{
    return flipX_;
}

void LevelObject::setFlipX(bool flipX)
{
    flipX_ = flipX;
}

bool LevelObject::flipY() const
{
    return flipY_;
}

void LevelObject::setFlipY(bool flipY)
{
    flipY_ = flipY;
}
