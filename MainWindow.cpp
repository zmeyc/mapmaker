// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QSplitter>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTimer>

#include "MainWindow.h"
#include "MapLayer.h"
#include "ObjectBrowser.h"
#include "Controls/LevelWidget.h"
#include "PropertyBrowser.h"
#include "Controllers/LevelLoader.h"
#include "Utils/Settings.h"
#include "Utils/Utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800, 600);

    mapLayer_ = new MapLayer(this);
    connect(mapLayer_, SIGNAL(widgetSelectionChanged(LevelWidget*)),
            this, SLOT(onWidgetSelectionChanged(LevelWidget*)));
    connect(mapLayer_, SIGNAL(widgetPositionChanged(LevelWidget*)),
            this, SLOT(onWidgetPositionChanged(LevelWidget*)));

    ObjectBrowser *objectBrowser = new ObjectBrowser(this);
    propertyBrowser_ = new PropertyBrowser(this);

    QSplitter *rightColumnSplitter = new QSplitter(this);
    rightColumnSplitter->setOrientation(Qt::Vertical);
    rightColumnSplitter->addWidget(objectBrowser);
    rightColumnSplitter->addWidget(propertyBrowser_);
    rightColumnSplitter->setSizes(QList<int>() << 400 << 200);

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(mapLayer_);
    splitter->addWidget(rightColumnSplitter);

    splitter->setSizes(QList<int>() << 400 << 200);

    setCentralWidget(splitter);

    QTimer::singleShot(0, this, SLOT(loadLevel()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    if (!mapLayer_->modified())
        return;

    QMessageBox::StandardButton result =
        QMessageBox::warning(this, "Confirmation",
            "Level modified. Save the changes?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (result == QMessageBox::Yes) {
        LevelLoader *levelLoader = LevelLoader::sharedInstance();
        if (!levelLoader->saveToFile(
                    mapLayer_,
                    Settings::sharedInstance()->mapFilename())) {
            QMessageBox::critical(this, "Error", levelLoader->lastErrorDescription());
            event->ignore();
        }
    } else if (result == QMessageBox::Cancel){
        event->ignore();
    }
}

void MainWindow::loadLevel()
{
    LevelLoader *levelLoader = LevelLoader::sharedInstance();
    Settings *settings = Settings::sharedInstance();
    if (!levelLoader->loadFromFile(mapLayer_, settings->mapFilename())) {
        QMessageBox::StandardButton result =
            QMessageBox::warning(this, "Error",
                                 levelLoader->lastErrorDescription(),
                                 QMessageBox::Yes | QMessageBox::Cancel);
        if (result == QMessageBox::Yes)
            mapLayer_->setModified(true);
        else
            close();
    }

}

void MainWindow::onWidgetSelectionChanged(LevelWidget *widget)
{
    bool selected = widget->selected();
    if (!selected) {
        if (propertyBrowser_->levelWidget() == widget)
            propertyBrowser_->setLevelWidget(nullptr);
    } else {
        propertyBrowser_->setLevelWidget(widget);
    }
}

void MainWindow::onWidgetPositionChanged(LevelWidget *widget)
{
    if (propertyBrowser_->levelWidget() == widget)
        propertyBrowser_->updatePosition();
}
