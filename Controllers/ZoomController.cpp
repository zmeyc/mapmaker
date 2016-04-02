#include <algorithm>
#include "ZoomController.h"
#include "Utils/Settings.h"
#include "Utils/Utils.h"

const double zoomLevels[] = {
    1.0, 2.0, 3.0, 4.0
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
