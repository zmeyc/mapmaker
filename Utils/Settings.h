// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSizeF>
#include <QSettings>

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
    Q_SLOT void setImagesDirectory(const QString &imagesDirectory);

    bool showGrid() const;
    Q_SLOT void setShowGrid(bool showGrid);

    bool autoSizeGrid() const;
    Q_SLOT void setAutoSizeGrid(bool autoSizeGrid);

    // Grid size affected by autoSizeGrid option
    QSizeF finalGridSize() const;
    // User-configured grid size
    QSizeF gridSize() const;
    void setGridSize(const QSizeF &gridSize);
    Q_SLOT void setGridSize(qreal uniformSize);
    Q_SLOT void setGridSize(int uniformSize);

    QSizeF selectedLevelObjectSize() const;
    void setSelectedLevelObjectSize(const QSizeF &selectedLevelObjectSize);

    bool snapToGrid() const;
    Q_SLOT void setSnapToGrid(bool snapToGrid);

signals:
    void mapFilenameChanged(const QString &mapFilename);
    void imagesDirectoryChanged(const QString &imagesDirectory);
    void showGridChanged(bool showGrid);
    bool autoSizeGridChanged(bool autoSizeGrid);
    void gridSizeChanged(const QSizeF &combinedGridSize);
    void finalGridSizeChanged(const QSizeF &combinedGridSize);

public slots:
    void save();

protected:
    void load();

    struct {
        QString imagesDirectory_;
    } commandline_;

    QSettings settings_;
    QString mapFilename_;

    // General
    QString imagesDirectory_;

    // Grid
    bool showGrid_ = false;
    bool autoSizeGrid_ = false;
    QSizeF gridSize_;
    QSizeF selectedLevelObjectSize_;
    bool snapToGrid_ = false;
};

#endif // SETTINGS_H
