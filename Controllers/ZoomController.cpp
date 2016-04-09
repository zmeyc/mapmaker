#include <algorithm>
#include "ZoomController.h"
#include "Utils/Settings.h"
#include "Utils/Utils.h"

const double zoomLevels[] = {
    0.05,
    0.25 / 4.0,
    1.0 / 12.0,
    0.25 / 2.0,
    1.0 / 6.0,
    0.25,
    1.0 / 3.0,
    0.5,
    2.0 / 3.0,
    1.0,
    2.0,
    3.0,
    4.0,
    5.0,
    6.0,
    7.0,
    8.0,
    12.0,
    16.0,
    32.0
};
const size_t zoomLevelsCount = sizeof(zoomLevels) / sizeof(zoomLevels[0]);

ZoomController::ZoomController(QObject *parent)
    : QObject(parent)
{

}

ZoomController *ZoomController::sharedInstance()
{
    static ZoomController *instance = new ZoomController;
    return instance;
}

void ZoomController::previousZoomLevel() const
{
    double zoom = Settings::sharedInstance()->zoom();
    // Returns value equal or greater than zoom
    const double *it = std::lower_bound(&zoomLevels[0], &zoomLevels[zoomLevelsCount], zoom);
    if (it == &zoomLevels[0]) {
        // No previous zoom level
        return;
    }
    --it;
    Settings::sharedInstance()->setZoom(*it);
}

void ZoomController::nextZoomLevel() const
{
    double zoom = Settings::sharedInstance()->zoom();
    // Returns value greater than zoom
    const double *it = std::upper_bound(&zoomLevels[0], &zoomLevels[zoomLevelsCount], zoom);
    if (it == &zoomLevels[zoomLevelsCount]) {
        // No next zoom level
        return;
    }
    Settings::sharedInstance()->setZoom(*it);
}
