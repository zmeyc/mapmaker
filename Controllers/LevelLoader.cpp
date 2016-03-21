// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSet>

#include "LevelLoader.h"
#include "Models/LevelObjectsModel.h"
#include "Models/MapScene.h"
#include "MapView.h"
#include "MapItems/MapItem.h"
#include "Data/LevelObject.h"
#include "Utils/Utils.h"

LevelLoader::LevelLoader(QObject *parent)
    : QObject(parent)
{

}

LevelLoader *LevelLoader::sharedInstance()
{
    static LevelLoader *instance = new LevelLoader;
    return instance;
}

bool LevelLoader::saveToFile(MapScene *scene, const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        lastErrorDescription_ = "Unable to open the file";
        return false;
    }

    QJsonObject level;

    QJsonArray objects;
    foreach (QGraphicsItem *item, scene->items()) {
        MapItem *mapItem = dynamic_cast<MapItem *>(item);
        LevelObject *levelObject = mapItem ? mapItem->levelObject() : nullptr;
        if (!levelObject)
            continue;

        QJsonObject obj;
        obj["x"] = levelObject->position().x() - levelObject->size().width() / 2;
        obj["y"] = levelObject->position().y() - levelObject->size().height() / 2;
        if (levelObject->flipX())
            obj["flipX"] = true;
        if (levelObject->flipY())
            obj["flipY"] = true;
        if (levelObject->size().width())
            obj["width"] = levelObject->size().width();
        if (levelObject->size().height())
            obj["height"] = levelObject->size().height();
        obj["name"] = levelObject->name();
        LevelObject::Properties properties = levelObject->customProperties();
        if (!properties.isEmpty()) {
            QVariantMap v;
            for (LevelObject::Properties::iterator i = properties.begin();
                 i != properties.end(); ++i)
            {
                v[i.key()] = i.value();
            }
            obj["properties"] = QJsonValue::fromVariant(v);
        }
        objects.append(obj);
    }

    level["objects"] = objects;
    QJsonDocument document(level);

    QByteArray data(document.toJson());
    if (data.size() != file.write(data)) {
        lastErrorDescription_ = "Unable to save all data (disk full?)";
        return false;
    }

    return true;
}

QString LevelLoader::lastErrorDescription() const
{
    return lastErrorDescription_;
}

void LevelLoader::setLastErrorDescription(const QString &lastErrorDescription)
{
    lastErrorDescription_ = lastErrorDescription;
}

bool LevelLoader::loadFromFile(MapScene *scene, const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        lastErrorDescription_ = "Unable to open the level";
        return false;
    }
    if (!file.exists()) {
        lastErrorDescription_ = "Level does not exist";
        return false;
    }

    QByteArray data = file.readAll();
    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        lastErrorDescription_ = "JSON parse error: " + parseError.errorString();
        return false;
    }

    QJsonObject level = document.object();
    QJsonArray objects = level["objects"].toArray();
    QSet<QString> notFound;
    LevelObjectsModel *model = LevelObjectsModel::sharedInstance();
    foreach (const QJsonValue &value, objects) {
        QJsonObject obj = value.toObject();

        QString name = obj["name"].toString();
        qreal width = obj["width"].toDouble();
        qreal height = obj["height"].toDouble();
        qreal x = obj["x"].toDouble() + width / 2;
        qreal y = obj["y"].toDouble() + height / 2;
        QSizeF size(width, height);
        int flipX = obj["flipX"].toBool();
        int flipY = obj["flipY"].toBool();
        QVariantMap properties = obj["properties"].toVariant().toMap();

        //qdbg << "name=" << name << ", x=" << x << ", y=" << y << endl;
        LevelObject *newObject = nullptr;
        LevelObject *proto = model->levelObjectByName(name);
        if (proto) {
            newObject = proto->clone();
            newObject->setFlipX(flipX);
            newObject->setFlipY(flipY);

        } else {
            newObject = model->placeholder()->clone();
            newObject->setName(name);
            notFound.insert(name);
        }
        newObject->setPosition(x, y);
        if (!size.isNull())
            newObject->setSize(size);
        for (QVariantMap::iterator i = properties.begin();
             i != properties.end(); ++i)
        {
            newObject->setCustomProperty(i.key(), i.value().toString());
        }

        MapItem *item = new MapItem(newObject);
        scene->addItem(item);
        QObject::connect(newObject, SIGNAL(modified()),
                         qobject_cast<MapView *>(scene->parent()), SLOT(setModified()));
    }

    if (!notFound.isEmpty()) {
        QStringList objects = notFound.toList();
        qSort(objects);
        lastErrorDescription_ = "Some objects were not found: " +
                 objects.join(", ");
        return false;
    }

    return true;
}
