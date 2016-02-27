// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "LevelLoader.h"
#include "MapView.h"
#include "Controls/LevelWidget.h"
#include "Models/LevelObjectsModel.h"
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

bool LevelLoader::saveToFile(MapView *mapView, const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        lastErrorDescription_ = "Unable to open the file";
        return false;
    }

    QJsonObject level;

    QJsonArray objects;
    foreach (LevelWidget *widget, mapView->findChildren<LevelWidget *>()) {
        QJsonObject obj;
        obj["x"] = widget->pos().x();
        obj["y"] = widget->pos().y();
        if (widget->flipX())
            obj["flipX"] = true;
        if (widget->flipY())
            obj["flipY"] = true;
        obj["name"] = widget->levelObject()->name();
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

bool LevelLoader::loadFromFile(MapView *mapView, const QString &filename)
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
    foreach (const QJsonValue &value, objects) {
        QJsonObject obj = value.toObject();

        QString name = obj["name"].toString();
        int x = obj["x"].toInt();
        int y = obj["y"].toInt();
        int flipX = obj["flipX"].toBool();
        int flipY = obj["flipY"].toBool();

        qdbg << "name=" << name << ", x=" << x << ", y=" << y << endl;
        LevelObject *levelObject = LevelObjectsModel::sharedInstance()->levelObjectByName(name);
        if (!levelObject) {
            if (!notFoundList.isEmpty())
                notFoundList += ", ";
            notFoundList += name;
            continue;
        }

        LevelWidget *objWidget = new LevelWidget(mapView);
        objWidget->setLevelObject(levelObject);
        objWidget->move(x, y);
        objWidget->setFlipX(flipX);
        objWidget->setFlipY(flipY);
        objWidget->show();
        mapView->addLevelWidget(objWidget);
    }

    if (!notFoundList.isEmpty()) {
        lastErrorDescription_ = "Some objects were not found and will be deleted: " +
                notFoundList;
        return false;
    }

    return true;
}
