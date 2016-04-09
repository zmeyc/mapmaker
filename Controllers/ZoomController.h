#ifndef ZOOMCONTROLLER_H
#define ZOOMCONTROLLER_H

#include <QObject>

class ZoomController : public QObject
{
    Q_OBJECT
public:
    ZoomController(QObject *parent = nullptr);

    static ZoomController *sharedInstance();

public slots:
    void previousZoomLevel() const;
    void nextZoomLevel() const;
};

#endif // ZOOMCONTROLLER_H
