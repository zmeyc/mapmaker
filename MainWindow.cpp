// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QSplitter>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTimer>
#include <QMenuBar>

#include "MainWindow.h"
#include "MapView.h"
#include "ObjectBrowser.h"
#include "Controls/LevelWidget.h"
#include "PropertyBrowser.h"
#include "Controllers/LevelLoader.h"
#include "Dialogs/SettingsDialog/SettingsDialog.h"
#include "Utils/Settings.h"
#include "Utils/Utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800, 600);

    mapView_ = new MapView(this);

    ObjectBrowser *objectBrowser = new ObjectBrowser(this);
    propertyBrowser_ = new PropertyBrowser(this);

    connect(mapView_, SIGNAL(selectedLevelObjectChanged(LevelObject*)),
            propertyBrowser_, SLOT(setLevelObject(LevelObject*)));

    QSplitter *rightColumnSplitter = new QSplitter(this);
    rightColumnSplitter->setOrientation(Qt::Vertical);
    rightColumnSplitter->addWidget(objectBrowser);
    rightColumnSplitter->addWidget(propertyBrowser_);
    rightColumnSplitter->setSizes(QList<int>() << 400 << 200);

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(mapView_);
    splitter->addWidget(rightColumnSplitter);

    splitter->setSizes(QList<int>() << 400 << 200);

    setCentralWidget(splitter);

    createMenu();

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
                    mapView_->mapScene(),
                    Settings::sharedInstance()->mapFilename())) {
            QMessageBox::critical(this, "Error", levelLoader->lastErrorDescription());
            event->ignore();
        }
    } else if (result == QMessageBox::Cancel){
        event->ignore();
    }
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
    Settings *settings = Settings::sharedInstance();
    if (!levelLoader->loadFromFile(mapView_->mapScene(), settings->mapFilename())) {
        QMessageBox::StandardButton result =
            QMessageBox::warning(this, "Error",
                                 levelLoader->lastErrorDescription(),
                                 QMessageBox::Yes | QMessageBox::Cancel);
        if (result == QMessageBox::Yes)
            mapView_->setModified(true);
        else
            close();
    }

}

void MainWindow::createMenu()
{
    Settings *settings = Settings::sharedInstance();

    QMenuBar *bar = menuBar();

    QMenu *editMenu = bar->addMenu(tr("&Edit"));

    QKeySequence preferencesShortcut(tr("Ctrl+K", "Edit|Preferences..."));
    editMenu->addAction(
                tr("Preferences..."), this, SLOT(onPreferences()), preferencesShortcut);

    QMenu *viewMenu = bar->addMenu(tr("&View"));

    QKeySequence showGridShortcut(tr("Ctrl+'", "View|Show Grid"));
    QAction *showGridAction = viewMenu->addAction(
                tr("Show Grid"), settings, SLOT(setShowGrid(bool)), showGridShortcut);
    showGridAction->setCheckable(true);
    showGridAction->setChecked(settings->showGrid());

    QKeySequence snapToGridShortcut(tr("Ctrl+Shift+;", "View|Snap to Grid"));
    QAction *snapToGridAction = viewMenu->addAction(
                tr("Snap to Grid"), settings, SLOT(setSnapToGrid(bool)), snapToGridShortcut);
    snapToGridAction->setCheckable(true);
    snapToGridAction->setChecked(settings->snapToGrid());
}
