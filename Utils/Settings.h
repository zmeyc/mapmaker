// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSizeF>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    static Settings *sharedInstance();

    bool parseCommandLine();

    QString mapFilename() const;
    void setMapFilename(const QString &mapFilename);

    QString imagesDirectory() const;
    void setImagesDirectory(const QString &imagesDirectory);

    bool showGrid() const;
    Q_SLOT void setShowGrid(bool showGrid);

signals:
    void showGridChanged(bool showGrid);

public slots:

protected:
    QString mapFilename_;
    QString imagesDirectory_;
    bool showGrid_ = false;
    QSizeF gridSize_;
};

#endif // SETTINGS_H
