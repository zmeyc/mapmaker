// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include "LevelObject.h"
#include "Utils/FileUtils.h"

LevelObject::LevelObject(QObject *parent)
    : QObject(parent)
{

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
