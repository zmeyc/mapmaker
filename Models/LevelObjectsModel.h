// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef LEVELOBJECTSMODEL_H
#define LEVELOBJECTSMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "Data/LevelObject.h"

class LevelObjectsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit LevelObjectsModel(QObject *parent = 0);

    static LevelObjectsModel *sharedInstance();

    bool addImagesFromDirectory(const QString &directory);
    LevelObject *levelObjectByName(const QString &name);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    QPoint dragOffset() const;
    void setDragOffset(const QPoint &dragOffset);

signals:

public slots:

protected:
    QVector<LevelObject *> entries_;
    QPoint dragOffset_;
};

#endif // LEVELOBJECTSMODEL_H
