// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QCommandLineParser>

#include "Settings.h"
#include "Utils/Utils.h"

const char *mapFilenameKey = "mapFilename";
const char *geometryKey = "window/geometry";
const char *imagesDirectoryKey = "imagesDirectory";
const char *showGridKey = "view/showGrid";
const char *autoSizeGridKey = "view/autoSizeGrid";
const char *gridSizeKey = "view/gridSize";
const char *snapToGridKey = "view/snapToGrid";
const char *showDockPointsKey = "view/showDockPoints";
const char *snapToDockPointsKey = "view/snapToDockPoints";

Settings::Settings(QObject *parent)
    : QObject(parent)
    , settings_(QSettings::IniFormat,
                QSettings::UserScope,
                QCoreApplication::organizationName(),
                QCoreApplication::applicationName())
{
    qdbg << "Settings::Settings()" << endl;
    load();
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
                                     "Usage: mapmaker -m map.json -i imagesDirectory");
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

    QString mapFilename = parser.value(mapFilenameOption);;
    if (!mapFilename.isEmpty())
        mapFilename_ = mapFilename;
    commandline_.imagesDirectory_ = parser.value(imagesDirectoryOption);
    return true;
}

QString Settings::mapFilename() const
{
    return mapFilename_;
}

void Settings::setMapFilename(const QString &mapFilename)
{
    if (mapFilename_ == mapFilename)
        return;

    mapFilename_ = mapFilename;
    settings_.setValue(mapFilenameKey, mapFilename);
    emit mapFilenameChanged(mapFilename);
}

void Settings::resetMapFilename()
{
    setMapFilename(QString());
}

QByteArray Settings::geometry() const
{
    return geometry_;
}

void Settings::setGeometry(const QByteArray &geometry)
{
    if (geometry_ == geometry)
        return;

    geometry_ = geometry;
    settings_.setValue(geometryKey, geometry);
    emit geometryChanged(geometry);
}

QString Settings::imagesDirectory() const
{
    if (!commandline_.imagesDirectory_.isEmpty())
        return commandline_.imagesDirectory_;
    return imagesDirectory_;
}

void Settings::setImagesDirectory(const QString &imagesDirectory)
{
    qdbg << "Settings::setImagesDirectory: " << imagesDirectory << endl;
    commandline_.imagesDirectory_.clear();
    imagesDirectory_ = imagesDirectory;
    settings_.setValue(imagesDirectoryKey, imagesDirectory_);
    emit imagesDirectoryChanged(imagesDirectory);
}

bool Settings::showGrid() const
{
    return showGrid_;
}

void Settings::setShowGrid(bool showGrid)
{
    if (showGrid_ == showGrid)
        return;

    showGrid_ = showGrid;
    settings_.setValue(showGridKey, showGrid_);
    emit showGridChanged(showGrid);
}

bool Settings::autoSizeGrid() const
{
    return autoSizeGrid_;
}

void Settings::setAutoSizeGrid(bool autoSizeGrid)
{
    if (autoSizeGrid_ == autoSizeGrid)
        return;

    QSizeF previousGridSize = finalGridSize();

    autoSizeGrid_ = autoSizeGrid;
    settings_.setValue(autoSizeGridKey, autoSizeGrid_);
    emit autoSizeGridChanged(autoSizeGrid);

    QSizeF newGridSize = finalGridSize();
    if (previousGridSize != newGridSize)
        emit finalGridSizeChanged(newGridSize);
}

QSizeF Settings::finalGridSize() const
{
    if (autoSizeGrid_ && !selectedLevelObjectSize_.isEmpty())
        return selectedLevelObjectSize_;
    return gridSize_;
}

QSizeF Settings::gridSize() const
{
    return gridSize_;
}

void Settings::setGridSize(const QSizeF &gridSize)
{
    if (gridSize_ == gridSize)
        return;

    QSizeF previousGridSize = this->finalGridSize();

    gridSize_ = gridSize;
    settings_.setValue(gridSizeKey, gridSize_);
    emit gridSizeChanged(gridSize);

    QSizeF newGridSize = this->finalGridSize();
    if (previousGridSize != newGridSize)
        emit finalGridSizeChanged(newGridSize);
}

void Settings::setGridSize(qreal uniformSize)
{
    setGridSize(QSizeF(uniformSize, uniformSize));
}

void Settings::setGridSize(int uniformSize)
{
    setGridSize((qreal)uniformSize);
}


QSizeF Settings::selectedLevelObjectSize() const
{
    return selectedLevelObjectSize_;
}

void Settings::setSelectedLevelObjectSize(const QSizeF &selectedLevelObjectSize)
{
    if (selectedLevelObjectSize_ == selectedLevelObjectSize)
        return;

    QSizeF previousGridSize = finalGridSize();

    selectedLevelObjectSize_ = selectedLevelObjectSize;

    QSizeF newGridSize = finalGridSize();
    if (previousGridSize != newGridSize)
        emit finalGridSizeChanged(newGridSize);

}

bool Settings::snapToGrid() const
{
    return snapToGrid_;
}

void Settings::setSnapToGrid(bool snapToGrid)
{
    if (snapToGrid_ == snapToGrid)
        return;

    snapToGrid_ = snapToGrid;
    settings_.setValue(snapToGridKey, snapToGrid_);
    emit snapToGridChanged(snapToGrid);
}

bool Settings::showDockPoints() const
{
    return showDockPoints_;
}

void Settings::setShowDockPoints(bool showDockPoints)
{
    if (showDockPoints_ == showDockPoints)
        return;

    showDockPoints_ = showDockPoints;
    settings_.setValue(showDockPointsKey, showDockPoints_);
    emit showDockPointsChanged(showDockPoints);
}

bool Settings::snapToDockPoints() const
{
    return snapToDockPoints_;
}

void Settings::setSnapToDockPoints(bool snapToDockPoints)
{
    if (snapToDockPoints_ == snapToDockPoints)
        return;

    snapToDockPoints_ = snapToDockPoints;
    settings_.setValue(snapToDockPointsKey, snapToDockPoints_);
    emit snapToDockPointsChanged(snapToDockPoints);
}

double Settings::zoom() const
{
    return zoom_;
}

void Settings::setZoom(double zoom)
{
    if (zoom_ == zoom)
        return;
    zoom_ = zoom;
    emit zoomChanged(zoom);
}

void Settings::setZoom100()
{
    setZoom(1.0);
}

void Settings::setZoom200()
{
    setZoom(2.0);
}

void Settings::setZoom300()
{
    setZoom(3.0);
}

void Settings::setZoom400()
{
    setZoom(4.0);
}

void Settings::load()
{
    mapFilename_ = settings_.value(mapFilenameKey).toString();
    geometry_ = settings_.value(geometryKey).toByteArray();
    imagesDirectory_ = settings_.value(imagesDirectoryKey).toString();
    showGrid_ = settings_.value(showGridKey, false).toBool();
    autoSizeGrid_ = settings_.value(autoSizeGridKey, false).toBool();
    gridSize_ = settings_.value(gridSizeKey, QSizeF(32.0, 32.0)).toSizeF();
    snapToGrid_ = settings_.value(snapToGridKey, false).toBool();
    showDockPoints_ = settings_.value(showDockPointsKey, true).toBool();
    snapToDockPoints_ = settings_.value(snapToDockPointsKey, true).toBool();
    qout << "Settings loaded" << endl;
}

void Settings::save()
{
    settings_.sync();
    qout << "Settings saved" << endl;
}
