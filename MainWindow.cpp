// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QApplication>
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
#include "Controllers/ZoomController.h"
#include "Dialogs/SettingsDialog/SettingsDialog.h"
#include "Models/MapScene.h"
#include "Models/LevelObjectsModel.h"
#include "Utils/Settings.h"
#include "Utils/Utils.h"
#include "Utils/FileUtils.h"

const QString mapFilenameFilter = QObject::tr(
            "MapMaker Files (*.mmj);;All files (*.*)");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , settings_(Settings::sharedInstance())
{
    connect(settings_, SIGNAL(mapFilenameChanged(QString)),
            this, SLOT(updateWindowTitle()));
    updateWindowTitle();

    loadWindowSettings();

    mapView_ = new MapView(this);
    connect(mapView_, SIGNAL(sceneCreated(QGraphicsScene*)),
            this, SLOT(onSceneCreated(QGraphicsScene*)));

    ObjectBrowser *objectBrowser = new ObjectBrowser(this);
    propertyBrowser_ = new PropertyBrowser(this);

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

    qdbg << "MainWindow::MainWindow(): mapFilename=" <<
            settings_->mapFilename() << Qt::endl;
    if (!settings_->mapFilename().isEmpty())
        QTimer::singleShot(0, this, SLOT(loadLevel()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    if (mapView_->mapScene()->modified()) {
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
                return;
            }
        } else if (result == QMessageBox::Cancel){
            event->ignore();
            return;
        }
    }

    saveWindowSettings();
    settings_->save(); // Not done automatically on Cmd-Q
    event->accept();
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
    QString lastMapFilename = settings_->mapFilename();
    QDir mapDir = QFileInfo(lastMapFilename).absoluteDir();
    QString mapPath = mapDir.absolutePath();

    QString dataLocation = mapPath;
    if (dataLocation.isEmpty()) {
        QStringList locations = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
        if (!locations.empty())
            dataLocation = locations.first();
    }

    QString mapFilename = QFileDialog::getOpenFileName(this,
             tr("Open Map"), dataLocation,
             mapFilenameFilter);
    if (mapFilename.isNull())
        return;
    settings_->setMapFilename(mapFilename);
    loadLevel();
}

void MainWindow::onSave()
{
    LevelLoader *levelLoader = LevelLoader::sharedInstance();
    if (!levelLoader->saveToFile(
                mapView_,
                settings_->mapFilename())) {
        QMessageBox::critical(this, "Error", levelLoader->lastErrorDescription());
        return;
    }
    mapView_->mapScene()->setModified(false);
}

void MainWindow::onSaveAs()
{
    //QString lastMapFilename = settings_->mapFilename();
    //QDir mapDir = QFileInfo(lastMapFilename).absoluteDir();

    QFileDialog dialog;
    dialog.setWindowTitle(tr("Save Map As"));
    //dialog.setDirectory(mapDir);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(mapFilenameFilter);
    dialog.selectFile(settings_->mapFilename());
    //dialog.setOption(QFileDialog::DontUseNativeDialog);
    if (dialog.exec() == QDialog::Accepted && !dialog.selectedFiles().isEmpty()) {
        QString filename = dialog.selectedFiles().first();
        LevelLoader *levelLoader = LevelLoader::sharedInstance();
        if (!levelLoader->saveToFile(
                    mapView_,
                    filename)) {
            QMessageBox::critical(this, "Error", levelLoader->lastErrorDescription());
            return;
        }
        mapView_->mapScene()->setModified(false);
        settings_->setMapFilename(filename);
    }
}

void MainWindow::onReloadImages()
{
    loadImages();
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
        mapView_->mapScene()->setModified(true);
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

    connect(scene, SIGNAL(selectedLevelObjectChanged(LevelObject*)),
            propertyBrowser_, SLOT(setLevelObject(LevelObject*)));
}

void MainWindow::loadWindowSettings()
{
    QByteArray geometry = settings_->geometry();
    if (geometry.isEmpty()) {
        QScreen *screen = QApplication::primaryScreen();
        if (screen) {
            QRect rect = screen->availableGeometry();
            resize(rect.width() * 2 / 3, rect.height() * 3 / 4);
            move((rect.width() - width()) / 2,
                 (rect.height() - height()) / 2);
        }
    } else {
        restoreGeometry(geometry);
    }
}

void MainWindow::saveWindowSettings()
{
    settings_->setGeometry(saveGeometry());
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

    fileMenu->addSeparator();

    QKeySequence saveShortcut(tr("Ctrl+S", "File|Save"));
    fileMenu->addAction(tr("Save"),
                        this, SLOT(onSave()), saveShortcut);

    QKeySequence saveAsShortcut(tr("Shift+Ctrl+S", "File|Save As"));
    fileMenu->addAction(tr("Save As"),
                        this, SLOT(onSaveAs()), saveAsShortcut);

    fileMenu->addSeparator();

    QKeySequence reloadImagesShortcut(tr("Ctrl+R", "File|Reload Images and Metadata"));
    fileMenu->addAction(tr("Reload Images and Metadata"),
                        this, SLOT(onReloadImages()), reloadImagesShortcut);
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

    QAction *copyAction = editMenu->addAction(
                tr("&Copy"), mapView_, SLOT(copy()), QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy selected objects to the clipboard"));

    QAction *pasteAction = editMenu->addAction(
                tr("&Paste"), mapView_, SLOT(paste()), QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste the clipboard's contents"));

    editMenu->addAction(
                tr("Select &All"), mapView_, SLOT(selectAll()), QKeySequence::SelectAll);

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

    ZoomController *zoomController = ZoomController::sharedInstance();

    QKeySequence zoomInShortcut(tr("Ctrl++", "View|Zoom In"));
    viewMenu->addAction(
                tr("Zoom In"), zoomController, SLOT(nextZoomLevel()), zoomInShortcut);

    QKeySequence zoomOutShortcut(tr("Ctrl+-", "View|Zoom Out"));
    viewMenu->addAction(
                tr("Zoom Out"), zoomController, SLOT(previousZoomLevel()), zoomOutShortcut);

    QKeySequence zoom100Shortcut(tr("Ctrl+1", "View|100%"));
    viewMenu->addAction(
                tr("100%"), settings_, SLOT(setZoom100()), zoom100Shortcut);

    QKeySequence zoom200Shortcut(tr("Ctrl+2", "View|200%"));
    viewMenu->addAction(
                tr("200%"), settings_, SLOT(setZoom200()), zoom200Shortcut);

    QKeySequence zoom300Shortcut(tr("Ctrl+3", "View|300%"));
    viewMenu->addAction(
                tr("300%"), settings_, SLOT(setZoom300()), zoom300Shortcut);

    QKeySequence zoom400Shortcut(tr("Ctrl+4", "View|400%"));
    viewMenu->addAction(
                tr("400%"), settings_, SLOT(setZoom400()), zoom400Shortcut);

    viewMenu->addSeparator();

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

    viewMenu->addSeparator();

    QKeySequence showDockPointsShortcut(tr("Ctrl+D", "View|Show Dock Points"));
    QAction *showDockPointsAction = viewMenu->addAction(
                tr("Show Dock Points"), settings_, SLOT(setShowDockPoints(bool)), showDockPointsShortcut);
    showDockPointsAction->setCheckable(true);
    showDockPointsAction->setChecked(settings_->showDockPoints());

    QKeySequence snapToDockPointsShortcut(tr("Ctrl+Shift+D", "View|Snap to Dock Points"));
    QAction *snapToDockPointsAction = viewMenu->addAction(
                tr("Snap to Dock Points"), settings_, SLOT(setSnapToDockPoints(bool)), snapToDockPointsShortcut);
    snapToDockPointsAction->setCheckable(true);
    snapToDockPointsAction->setChecked(settings_->snapToDockPoints());
}
