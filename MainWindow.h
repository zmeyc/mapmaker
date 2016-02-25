// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MapLayer;
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
    void loadLevel();
    void onWidgetSelectionChanged(LevelWidget *widget);
    void onWidgetPositionChanged(LevelWidget *widget);

protected:
    MapLayer *mapLayer_ = nullptr;
    PropertyBrowser *propertyBrowser_ = nullptr;
};

#endif // MAINWINDOW_H
