// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <QObject>

class MapView;

class LevelLoader : public QObject
{
    Q_OBJECT
public:
    explicit LevelLoader(QObject *parent = 0);

    static LevelLoader *sharedInstance();

    bool loadFromFile(MapView *view, const QString &filename);
    bool saveToFile(MapView *view, const QString &filename);

    QString lastErrorDescription() const;
    void setLastErrorDescription(const QString &lastErrorDescription);

signals:

public slots:

protected:
    QString lastErrorDescription_;
};

#endif // LEVELLOADER_H
