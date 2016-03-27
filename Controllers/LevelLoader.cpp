// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSet>

#include "LevelLoader.h"
#include "Models/MapScene.h"
#include "Models/LevelObjectsModel.h"
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

bool LevelLoader::saveToFile(MapView *view, const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        lastErrorDescription_ = "Unable to open the file";
        return false;
    }

    QJsonObject level;

    MapScene *scene = (MapScene *)view->scene();
    QJsonArray objects = scene->toJsonArray();
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

bool LevelLoader::loadFromFile(MapView *view, const QString &filename)
{
    view->resetScene();
    MapScene *scene = (MapScene *)view->scene();

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
    LevelObjectsModel *model = LevelObjectsModel::sharedInstance();
    QSet<QString> notFound;
    foreach (const QJsonValue &value, objects) {
        QJsonObject json = value.toObject();
        LevelObject *obj = LevelObject::createFromJson(json);
        if (obj == model->placeholder())
            notFound.insert(obj->name());

        MapItem *item = new MapItem(obj);
        scene->addItem(item);
        QObject::connect(obj, SIGNAL(modified()),
                         scene, SLOT(setModified()));
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
