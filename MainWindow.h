// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class Settings;
class MapView;
class LevelWidget;
class PropertyBrowser;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event) override;

protected slots:
    void loadImages();
    void updateWindowTitle();
    void onNew();
    void onOpen();
    void onSave();
    void onSaveAs();
    void onReloadImages();
    void onPreferences();
    void loadLevel();
    void onSceneCreated(QGraphicsScene *scene);

protected:
    void loadWindowSettings();
    void saveWindowSettings();
    void createFileMenu();
    void createEditMenu();
    void createMapMenu();
    void createViewMenu();

    Settings *settings_ = nullptr;
    MapView *mapView_ = nullptr;
    PropertyBrowser *propertyBrowser_ = nullptr;
};

#endif // MAINWINDOW_H
