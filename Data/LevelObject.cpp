// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QMetaMethod>
#include "LevelObject.h"
#include "Models/LevelObjectsModel.h"
#include "Utils/FileUtils.h"

LevelObject::LevelObject(QObject *parent)
    : QObject(parent)
    , size_(0, 0)
{
    // TODO: automate?
    connect(this, SIGNAL(positionChanged(QPointF)),
            this, SIGNAL(modified()));
    connect(this, SIGNAL(sizeChanged(QSizeF)),
            this, SIGNAL(modified()));
    connect(this, SIGNAL(flipXChanged(bool)),
            this, SIGNAL(modified()));
    connect(this, SIGNAL(flipYChanged(bool)),
            this, SIGNAL(modified()));
    connect(this, SIGNAL(customPropertyChanged(QString,QString)),
            this, SIGNAL(modified()));
}

LevelObject *LevelObject::clone(QObject *parent)
{
    LevelObject *obj = new LevelObject(parent);
    obj->name_ = name_;
    obj->filename_ = filename_;
    obj->image_ = image_;
    obj->position_ = position_;
    obj->size_ = size_;
    obj->flipX_ = flipX_;
    obj->flipY_ = flipY_;
    obj->customProperties_ = customProperties_;
    obj->dockPoints_ = dockPoints_;
    return obj;
}

QJsonObject LevelObject::toJsonObject() const
{
    QJsonObject json;
    json["x"] = position().x() - size().width() / 2;
    json["y"] = position().y() - size().height() / 2;
    if (flipX())
        json["flipX"] = true;
    if (flipY())
        json["flipY"] = true;
    if (size().width())
        json["width"] = size().width();
    if (size().height())
        json["height"] = size().height();
    json["name"] = name();
    LevelObject::Properties properties = customProperties();
    if (!properties.isEmpty()) {
        QVariantMap v;
        for (LevelObject::Properties::iterator i = properties.begin();
             i != properties.end(); ++i)
        {
            v[i.key()] = i.value();
        }
        json["properties"] = QJsonValue::fromVariant(v);
    }
    return json;
}

LevelObject *LevelObject::createFromJson(const QJsonObject &json,
                                         QObject *parent)
{
    LevelObjectsModel *model = LevelObjectsModel::sharedInstance();

    QString name = json["name"].toString();
    qreal width = json["width"].toDouble();
    qreal height = json["height"].toDouble();
    qreal x = json["x"].toDouble() + width / 2;
    qreal y = json["y"].toDouble() + height / 2;
    QSizeF size(width, height);
    int flipX = json["flipX"].toBool();
    int flipY = json["flipY"].toBool();
    QVariantMap properties = json["properties"].toVariant().toMap();

    //qdbg << "name=" << name << ", x=" << x << ", y=" << y << endl;
    LevelObject *obj = nullptr;
    LevelObject *proto = model->levelObjectByName(name);
    if (proto) {
        obj = proto->clone(parent);
        obj->setFlipX(flipX);
        obj->setFlipY(flipY);

    } else {
        obj = model->placeholder()->clone(parent);
        obj->setName(name);
    }
    obj->setPosition(x, y);
    if (!size.isNull())
        obj->setSize(size);
    for (QVariantMap::iterator i = properties.begin();
         i != properties.end(); ++i)
    {
        obj->setCustomProperty(i.key(), i.value().toString());
    }
    return obj;
}

QString LevelObject::name() const
{
    return name_;
}

void LevelObject::setName(const QString &name)
{
    if (name_ != name) {
        name_ = name;
        emit nameChanged(name);
    }
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
    setSize(image.size());
    emit imageChanged();
}

QPointF LevelObject::position() const
{
    return position_;
}

qreal LevelObject::x() const
{
    return position_.x();
}

qreal LevelObject::y() const
{
    return position_.y();
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

void LevelObject::setX(qreal x)
{
    if (position_.x() != x) {
        position_.setX(x);
        emit positionChanged(position_);
    }
}

void LevelObject::setY(qreal y)
{
    if (position_.y() != y) {
        position_.setY(y);
        emit positionChanged(position_);
    }
}

void LevelObject::moveBy(qreal dx, qreal dy)
{
    QPointF pos = position_;
    pos.setX(pos.x() + dx);
    pos.setY(pos.y() + dy);
    setPosition(pos);
}

QSizeF LevelObject::size() const
{
    return size_;
}

void LevelObject::setSize(const QSizeF &size)
{
    if (size_ != size) {
        emit willChangeSize(size);
        size_ = size;
        emit sizeChanged(size_);
    }
}

void LevelObject::setWidth(qreal width)
{
    if (size_.width() != width) {
        setSize(QSize(width, size_.height()));
    }
}

void LevelObject::setHeight(qreal height)
{
    if (size_.height() != height) {
        setSize(QSize(size_.width(), height));
    }
}

bool LevelObject::flipX() const
{
    return flipX_;
}

void LevelObject::setFlipX(bool flipX)
{
    if (flipX_ != flipX) {
        flipX_ = flipX;
        emit flipXChanged(flipX);
    }
}

bool LevelObject::flipY() const
{
    return flipY_;
}

void LevelObject::setFlipY(bool flipY)
{
    if (flipY_ != flipY) {
        flipY_ = flipY;
        emit flipYChanged(flipY);
    }
}

QString LevelObject::customProperty(const QString &key) const
{
    Properties::const_iterator i = customProperties_.find(key);
    if (i == customProperties_.end())
        return QString();
    return i.value();
}

void LevelObject::setCustomProperty(const QString &key, const QString &value)
{
    customProperties_[key] = value;
    emit customPropertyChanged(key, value);
}

void LevelObject::resetCustomProperty(const QString &key)
{
    customProperties_.remove(key);
}

LevelObject::Properties LevelObject::customProperties() const
{
    return customProperties_;
}

QVector<QPointF> LevelObject::dockPoints() const
{
    return dockPoints_;
}

QVector<QPointF> LevelObject::dockPointsWithXYFlip() const
{
    QVector<QPointF> result;
    result.reserve(dockPoints_.size());
    foreach (const QPointF &point, dockPoints_) {
        result.append(QPointF(flipX_ ? -point.x() : point.x(),
                              flipY_ ? -point.y() : point.y()));
    }
    return result;
}

void LevelObject::setDockPoints(const QVector<QPointF> &dockPoints)
{
    dockPoints_ = dockPoints;
    emit dockPointsChanged();
}

void LevelObject::addDockPoint(qreal x, qreal y)
{
    QPointF point(x, y);
    dockPoints_.append(point);
    emit dockPointsChanged();
}
