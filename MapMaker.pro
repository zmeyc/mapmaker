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
    Controls/LevelWidget.cpp \
    Controllers/LevelLoader.cpp \
    Utils/Settings.cpp \
    PropertyBrowser.cpp \
    MapView.cpp

HEADERS  += MainWindow.h \
    ObjectBrowser.h \
    Utils/WidgetUtils.h \
    Models/LevelObjectsModel.h \
    Utils/Utils.h \
    Data/LevelObject.h \
    Utils/FileUtils.h \
    Controls/LevelWidget.h \
    Controllers/LevelLoader.h \
    Utils/Settings.h \
    PropertyBrowser.h \
    MapView.h
