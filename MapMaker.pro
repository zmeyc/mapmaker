# MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#-------------------------------------------------
#
# Project created by QtCreator 2016-02-20T23:50:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MapMaker
TEMPLATE = app

include(ThirdParty/qtpropertybrowser/src/qtpropertybrowser.pri)7

SOURCES += main.cpp\
        MainWindow.cpp \
    ObjectBrowser.cpp \
    Utils/WidgetUtils.cpp \
    Models/LevelObjectsModel.cpp \
    Utils/Utils.cpp \
    Data/LevelObject.cpp \
    Utils/FileUtils.cpp \
    Controllers/LevelLoader.cpp \
    Utils/Settings.cpp \
    PropertyBrowser.cpp \
    MapView.cpp \
    Models/MapScene.cpp \
    MapItems/MapItem.cpp \
    Dialogs/SettingsDialog/SettingsDialog.cpp \
    Dialogs/SettingsDialog/GridPage.cpp \
    Dialogs/NewPropertyDialog/NewPropertyDialog.cpp \
    Commands/DeleteCommand.cpp \
    Dialogs/SettingsDialog/GeneralPage.cpp \
    Controls/DirectoryLineEdit.cpp \
    AppStyle.cpp \
    GraphicsEffects/DockPointsGraphicsEffect.cpp \
    Controllers/ZoomController.cpp \
    Commands/NewItemCommand.cpp

HEADERS  += MainWindow.h \
    ObjectBrowser.h \
    Utils/WidgetUtils.h \
    Models/LevelObjectsModel.h \
    Utils/Utils.h \
    Data/LevelObject.h \
    Utils/FileUtils.h \
    Controllers/LevelLoader.h \
    Utils/Settings.h \
    PropertyBrowser.h \
    MapView.h \
    Models/MapScene.h \
    MapItems/MapItem.h \
    Dialogs/SettingsDialog/SettingsDialog.h \
    Dialogs/SettingsDialog/GridPage.h \
    Dialogs/NewPropertyDialog/NewPropertyDialog.h \
    Commands/DeleteCommand.h \
    Dialogs/SettingsDialog/GeneralPage.h \
    Controls/DirectoryLineEdit.h \
    Data/MimeTypes.h \
    AppStyle.h \
    GraphicsEffects/DockPointsGraphicsEffect.h \
    Controllers/ZoomController.h \
    Commands/NewItemCommand.h
