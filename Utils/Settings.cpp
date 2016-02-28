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
    if (parser.parse(qApp->arguments())) {
        parser.process(qApp->arguments());
    } else {
        qerr << parser.errorText() << endl;
    }

    if (qApp->arguments().count() != 3) {
        qerr << parser.helpText();
        return false;
    }
    mapFilename_ = qApp->arguments().at(1);
    imagesDirectory_ = qApp->arguments().at(2);
    return true;
}

QString Settings::mapFilename() const
{
    return mapFilename_;
}

void Settings::setMapFilename(const QString &mapFilename)
{
    mapFilename_ = mapFilename;
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
