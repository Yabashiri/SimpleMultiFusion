#-------------------------------------------------
#
# Project created by QtCreator 2016-05-17T04:43:50
#
#-------------------------------------------------

#QT       += core gui

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TARGET = loadImage
#TEMPLATE = app


#SOURCES += main.cpp\
#        imagebutton.cpp

#HEADERS  += imagebutton.h
# #####################################################################
# ToolBox SACS2 build script. Serkov Alexander, 2007
# #####################################################################
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool
LIBS += -L./../../../pluginTool
TEMPLATE = lib
CONFIG += plugin
TARGET = loadImage
DESTDIR = ./../../../bin/plugins/loadImage
SOURCES += loadImage.cpp
HEADERS += loadImage.h \
    Plugin.h \
    ./../../../pluginTool/SignalHolder.h \
    ../../../pluginTool/loadimage.h
QT += opengl \
        xml \
    xmlpatterns
