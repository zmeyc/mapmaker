// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QSplitter>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTimer>
#include <QMenuBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QUndoStack>

#include "MainWindow.h"
#include "MapView.h"
#include "ObjectBrowser.h"
#include "PropertyBrowser.h"
#include "Controllers/LevelLoader.h"
#include "Dialogs/SettingsDialog/SettingsDialog.h"
#include "Models/MapScene.h"
#include "Models/LevelObjectsModel.h"
#include "Utils/Settings.h"
#include "Utils/Utils.h"
#include "Utils/FileUtils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , settings_(Settings::sharedInstance())
{
    connect(settings_, SIGNAL(mapFilenameChanged(QString)),
            this, SLOT(updateWindowTitle()));
    updateWindowTitle();

    resize(800, 600);

    mapView_ = new MapView(this);
    connect(mapView_, SIGNAL(sceneCreated(QGraphicsScene*)),
            this, SLOT(onSceneCreated(QGraphicsScene*)));

    ObjectBrowser *objectBrowser = new ObjectBrowser(this);
    propertyBrowser_ = new PropertyBrowser(this);

    connect(mapView_, SIGNAL(selectedLevelObjectChanged(LevelObject*)),
            propertyBrowser_, SLOT(setLevelObject(LevelObject*)));

    QSplitter *rightColumnSplitter = new QSplitter(this);
    rightColumnSplitter->setOrientation(Qt::Vertical);
    rightColumnSplitter->addWidget(objectBrowser);
    rightColumnSplitter->addWidget(propertyBrowser_);
    rightColumnSplitter->setSizes(QList<int>() << 350 << 250);

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(mapView_);
    splitter->addWidget(rightColumnSplitter);

    splitter->setSizes(QList<int>() << 400 << 200);

    setCentralWidget(splitter);

    onSceneCreated(mapView_->scene());

    connect(settings_, SIGNAL(imagesDirectoryChanged(QString)),
            this, SLOT(loadImages()));
    loadImages();

    if (!settings_->mapFilename().isEmpty())
        QTimer::singleShot(0, this, SLOT(loadLevel()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    if (!mapView_->modified())
        return;

    QMessageBox::StandardButton result =
        QMessageBox::warning(this, "Confirmation",
            "Level modified. Save the changes?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (result == QMessageBox::Yes) {
        LevelLoader *levelLoader = LevelLoader::sharedInstance();
        if (!levelLoader->saveToFile(
                    mapView_,
                    settings_->mapFilename())) {
            QMessageBox::critical(this, "Error", levelLoader->lastErrorDescription());
            event->ignore();
        }
    } else if (result == QMessageBox::Cancel){
        event->ignore();
    }
}

void MainWindow::loadImages()
{
    LevelObjectsModel *model = LevelObjectsModel::sharedInstance();
    model->reset();
    if (!settings_->imagesDirectory().isEmpty()) {
        model->addImagesFromDirectory(settings_->imagesDirectory());
    }
}

void MainWindow::updateWindowTitle()
{
    QString title;
    if (settings_->mapFilename().isEmpty()) {
        title = "New Document | MapMaker";
    } else {
        QFileInfo fileInfo(settings_->mapFilename());
        title = fileInfo.fileName() + " | MapMaker";
    }
    setWindowTitle(title);
}

void MainWindow::onNew()
{
    settings_->resetMapFilename();
    mapView_->resetScene();
}

void MainWindow::onOpen()
{
    QStringList locations = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    QString dataLocation;
    if (!locations.empty())
        dataLocation = locations.first();

    QString mapFilename = QFileDialog::getOpenFileName(this,
             tr("MapMaker"), dataLocation,
             tr("MapMaker Files (*.mmj);;All files (*.*)"));
    if (mapFilename.isNull())
        return;
    settings_->setMapFilename(mapFilename);
    loadLevel();
}

void MainWindow::onPreferences()
{
    SettingsDialog *dialog = new SettingsDialog(this);
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setWindowFlags((dialog->windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->exec();
}

void MainWindow::loadLevel()
{
    LevelLoader *levelLoader = LevelLoader::sharedInstance();
    if (!levelLoader->loadFromFile(mapView_, settings_->mapFilename())) {
        /* QMessageBox::StandardButton result = */
            QMessageBox::warning(this, "Error",
                                 levelLoader->lastErrorDescription(),
                                 QMessageBox::Ok);
        // If loading completed with errors, treat the map as modified
        mapView_->setModified(true);
    }

}

void MainWindow::onSceneCreated(QGraphicsScene *scene)
{
    Q_UNUSED(scene);
    menuBar()->clear();
    createFileMenu();
    createEditMenu();
    createMapMenu();
    createViewMenu();
}

void MainWindow::createFileMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QKeySequence newShortcut(tr("Ctrl+N", "File|New"));
    fileMenu->addAction(tr("New"),
                        this, SLOT(onNew()), newShortcut);

    QKeySequence openShortcut(tr("Ctrl+O", "File|Open..."));
    fileMenu->addAction(tr("Open..."),
                        this, SLOT(onOpen()), openShortcut);
}

void MainWindow::createEditMenu()
{
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    QUndoStack *undoStack = mapView_->mapScene()->undoStack();

    QAction *undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);
    editMenu->addAction(undoAction);

    QAction *redoAction = undoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);
    editMenu->addAction(redoAction);

    editMenu->addSeparator();

    QKeySequence preferencesShortcut(tr("Ctrl+K", "Edit|Preferences..."));
    editMenu->addAction(tr("Preferences..."),
                        this, SLOT(onPreferences()), preferencesShortcut);
}

void MainWindow::createMapMenu()
{
    //QMenu *mapMenu = menuBar()->addMenu(tr("&Map"));
}

void MainWindow::createViewMenu()
{
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    QKeySequence showGridShortcut(tr("Ctrl+'", "View|Show Grid"));
    QAction *showGridAction = viewMenu->addAction(
                tr("Show Grid"), settings_, SLOT(setShowGrid(bool)), showGridShortcut);
    showGridAction->setCheckable(true);
    showGridAction->setChecked(settings_->showGrid());

    QKeySequence snapToGridShortcut(tr("Ctrl+Shift+;", "View|Snap to Grid"));
    QAction *snapToGridAction = viewMenu->addAction(
                tr("Snap to Grid"), settings_, SLOT(setSnapToGrid(bool)), snapToGridShortcut);
    snapToGridAction->setCheckable(true);
    snapToGridAction->setChecked(settings_->snapToGrid());
}
