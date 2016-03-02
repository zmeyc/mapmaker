// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
        obj["x"] = levelObject->position().x();
        obj["y"] = levelObject->position().y();
        if (levelObject->flipX())
            obj["flipX"] = true;
        if (levelObject->flipY())
            obj["flipY"] = true;
        if (levelObject->size().width())
            obj["width"] = levelObject->size().width();
        if (levelObject->size().height())
            obj["height"] = levelObject->size().height();
        obj["name"] = levelObject->name();
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
        lastErrorDescription_ = "Unable to open the level, continue?";
        return false;
    }
    if (!file.exists()) {
        lastErrorDescription_ = "Level does not exist, create?";
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
    QString notFoundList;
    LevelObjectsModel *model = LevelObjectsModel::sharedInstance();
    foreach (const QJsonValue &value, objects) {
        QJsonObject obj = value.toObject();

        QString name = obj["name"].toString();
        int x = obj["x"].toInt();
        int y = obj["y"].toInt();
        int flipX = obj["flipX"].toBool();
        int flipY = obj["flipY"].toBool();

        //qdbg << "name=" << name << ", x=" << x << ", y=" << y << endl;
        LevelObject *levelObject = model->levelObjectByName(name);
        if (!levelObject) {
            if (!notFoundList.isEmpty())
                notFoundList += ", ";
            notFoundList += name;

            levelObject = model->placeholder();
        }

        LevelObject *newObject = levelObject->clone();
        newObject->setPosition(x, y);
        newObject->setFlipX(flipX);
        newObject->setFlipY(flipY);

        MapItem *item = new MapItem(newObject);
        scene->addItem(item);
        QObject::connect(newObject, SIGNAL(modified()),
                         qobject_cast<MapView *>(scene->parent()), SLOT(setModified()));
    }

    if (!notFoundList.isEmpty()) {
        lastErrorDescription_ = "Some objects were not found: " +
                notFoundList;
        return false;
    }

    return true;
}
