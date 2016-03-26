// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QCommandLineParser>

#include "Settings.h"
#include "Utils/Utils.h"

const char *imagesDirectoryKey = "imagesDirectory";
const char *showGridKey = "grid/showGrid";
const char *autoSizeGridKey = "grid/autoSizeGrid";
const char *gridSizeKey = "grid/gridSize";

Settings::Settings(QObject *parent)
    : QObject(parent)
    , settings_(QSettings::IniFormat,
                QSettings::UserScope,
                QCoreApplication::organizationName(),
                QCoreApplication::applicationName())
{
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
    commandline_.imagesDirectory_ = parser.value(imagesDirectoryOption);
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
    if (showGrid_ != showGrid) {
        showGrid_ = showGrid;
        settings_.setValue(showGridKey, showGrid_);
        emit showGridChanged(showGrid);
    }
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
    snapToGrid_ = snapToGrid;
}

void Settings::load()
{
    imagesDirectory_ = settings_.value(imagesDirectoryKey).toString();
    showGrid_ = settings_.value(showGridKey, false).toBool();
    autoSizeGrid_ = settings_.value(autoSizeGridKey, false).toBool();
    gridSize_ = settings_.value(gridSizeKey, QSizeF(32.0, 32.0)).toSizeF();
    qout << "Settings loaded" << endl;
}

void Settings::save()
{
    settings_.sync();
    qout << "Settings saved" << endl;
}
