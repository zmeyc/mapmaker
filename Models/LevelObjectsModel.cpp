// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QDirIterator>
#include <QImage>
#include <QMimeData>
#include <QJsonArray>
#include <QJsonParseError>

#include "LevelObjectsModel.h"
#include "Utils/Utils.h"
#include "Data/MimeTypes.h"

LevelObjectsModel::LevelObjectsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    placeholder_ = new LevelObject;
}

LevelObjectsModel *LevelObjectsModel::sharedInstance()
{
    static LevelObjectsModel *instance = new LevelObjectsModel;
    return instance;
}

void LevelObjectsModel::reset()
{
    beginResetModel();
    foreach (LevelObject *obj, entries_) {
        delete obj;
    }
    entries_.clear();
    entriesByName_.clear();
    endResetModel();
}

bool LevelObjectsModel::addImagesFromDirectory(const QString &directory)
{
    bool allLoaded = true;

    QVector<LevelObject *> objectsToAdd;
    QMap<QString, LevelObject *> byName;
    objectsToAdd.reserve(1024);

    QDirIterator it(directory, QStringList() << "*.png", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filename = it.next();
        QImage image;
        if (image.load(filename)) {
            //qvrb << "Loaded: " << filename << endl;
        } else {
            qerr << "Unable to load: " << filename << Qt::endl;
            allLoaded = false;
        }

        LevelObject *obj = new LevelObject(this);
        obj->setFilename(filename);
        obj->setImage(image);
        objectsToAdd.append(obj);
        byName[obj->name()] = obj;

        QFileInfo fileInfo(filename);
        QString path = fileInfo.path();
        QString basename = fileInfo.baseName();
        QString metaFilename = path + "/" + basename + "-meta.json";
        //qdbg << metaFilename << endl;
        loadMetadata(obj, metaFilename);
    }

    beginResetModel();
    entries_.append(objectsToAdd);
    for (auto i = byName.begin(); i != byName.end(); ++i) {
        entriesByName_[i.key()] = i.value();
    }
    endResetModel();

    foreach (LevelObject *obj, entries_) {
        emit levelObjectUpdated(obj->name(), obj);
    }

    return allLoaded;
}

LevelObject *LevelObjectsModel::levelObjectByName(const QString &name)
{
    foreach (LevelObject *obj, entries_) {
        if (obj->name() == name)
            return obj;
    }
    return nullptr;
}

int LevelObjectsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return entries_.count();
}

QVariant LevelObjectsModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= rowCount()) {
        Q_ASSERT(false);
        return QVariant();
    }

    LevelObject *entry = entries_[row];

    switch (role) {
    //case Qt::DisplayRole:
    //    return entry->name();
    case Qt::DecorationRole: {
        return entry->image();
    }
    case Qt::SizeHintRole: {
        QSize size(entry->image().size());
        // Items are offset by 3 pixels vertically. Bottom
        // of the sprite is not drawn. Bug?
        // Compensate the offset:
        size.setHeight(size.height() + 3);
        return size;
    }
    default:
        break;
    }

    return QVariant();
}

Qt::ItemFlags LevelObjectsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

    return Qt::ItemIsDragEnabled | defaultFlags;
}

QStringList LevelObjectsModel::mimeTypes() const
{
    return QStringList() << levelObjectNameMimeType;
}

QMimeData *LevelObjectsModel::mimeData(const QModelIndexList &indexes) const
{
    if (indexes.empty())
        return nullptr;
    LevelObject *obj = entries_.at(indexes.front().row());
    //qdbg << "LevelObjectsModel: mimeData: row=" << indexes.front().row() << ", obj.name=" << obj->name() << endl;
    QByteArray encodedData;
    QDataStream dataStream(&encodedData, QIODevice::WriteOnly);
    dataStream << obj->name().toUtf8() << dragOffset_;
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(levelObjectNameMimeType, encodedData);
    return mimeData;
}

QPoint LevelObjectsModel::dragOffset() const
{
    return dragOffset_;
}

void LevelObjectsModel::setDragOffset(const QPoint &dragOffset)
{
    dragOffset_ = dragOffset;
}

LevelObject *LevelObjectsModel::placeholder() const
{
    return placeholder_;
}

bool LevelObjectsModel::loadMetadata(LevelObject *obj, const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        lastErrorDescription_ = "Unable to open the image metadata file";
        return false;
    }

    QByteArray data = file.readAll();
    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        lastErrorDescription_ = "JSON parse error: " + parseError.errorString();
        return false;
    }

    QJsonObject meta = document.object();
    QJsonArray dockPoints = meta["dockPoints"].toArray();
    foreach (const QJsonValue &value, dockPoints) {
        QJsonObject json = value.toObject();
        qreal x = json["x"].toDouble() - obj->size().width() / 2;
        qreal y = json["y"].toDouble() - obj->size().height() / 2;

        obj->addDockPoint(x, y);
    }

    return true;
}
