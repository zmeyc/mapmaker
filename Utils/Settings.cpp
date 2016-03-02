// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QCommandLineParser>

#include "Settings.h"
#include "Utils/Utils.h"

Settings::Settings(QObject *parent) : QObject(parent)
{
    gridSize_ = QSizeF(32.0, 32.0);
}

Settings *Settings::sharedInstance()
{
    static Settings *instance = new Settings;
    return instance;
}

bool Settings::parseCommandLine()
{
    QCommandLineParser parser;
    parser.setApplicationDescription("MapMaker\n\n"
                                     "Usage: mapmaker map.json imagesDirectory");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption mapFilenameOption(QStringList() << "m" << "map",
                                         tr("Map filename"),
                                         /* valueName */ tr("map.mmj"),
                                         /* defaultValue */ QString());
    parser.addOption(mapFilenameOption);

    QCommandLineOption imagesDirectoryOption(QStringList() << "i" << "imgdir",
                                             tr("Images directory"),
                                             /* valueName */ tr("directory"),
                                             /* defaultValue */ QString());
    parser.addOption(imagesDirectoryOption);

    if (parser.parse(qApp->arguments())) {
        parser.process(qApp->arguments());
    } else {
        qerr << parser.errorText() << endl;
    }

    mapFilename_ = parser.value(mapFilenameOption);
    imagesDirectory_ = parser.value(imagesDirectoryOption);
    return true;
}

QString Settings::mapFilename() const
{
    return mapFilename_;
}

void Settings::setMapFilename(const QString &mapFilename)
{
    if (mapFilename_ != mapFilename) {
        mapFilename_ = mapFilename;
        emit mapFilenameChanged(mapFilename);
    }
}

QString Settings::imagesDirectory() const
{
    return imagesDirectory_;
}

void Settings::setImagesDirectory(const QString &imagesDirectory)
{
    imagesDirectory_ = imagesDirectory;
}

bool Settings::showGrid() const
{
    return showGrid_;
}

void Settings::setShowGrid(bool showGrid)
{
    if (showGrid_ != showGrid) {
        showGrid_ = showGrid;
        emit showGridChanged(showGrid);
    }
}

QSizeF Settings::gridSize() const
{
    return QSizeF(qMax(1.0, gridSize_.width()),
                  qMax(1.0, gridSize_.height()));
}

void Settings::setGridSize(const QSizeF &gridSize)
{
    //qdbg << "Settings::setGridSize: " << gridSize << endl;
    if (gridSize_ != gridSize) {
        gridSize_ = gridSize;
        emit gridSizeChanged(gridSize);
    }
}

void Settings::setGridSize(qreal uniformSize)
{
    setGridSize(QSizeF(uniformSize, uniformSize));
}

void Settings::setGridSize(int uniformSize)
{
    setGridSize((qreal)uniformSize);
}

bool Settings::snapToGrid() const
{
    return snapToGrid_;
}

void Settings::setSnapToGrid(bool snapToGrid)
{
    snapToGrid_ = snapToGrid;
}
