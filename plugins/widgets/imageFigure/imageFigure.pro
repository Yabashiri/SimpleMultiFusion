#-------------------------------------------------
#
# Project created by QtCreator 2016-05-17T04:48:43
#
#-------------------------------------------------

#QT       += core gui

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TARGET = imageFigure
#TEMPLATE = app


#SOURCES += main.cpp\
       # imagefigure.cpp

#HEADERS  += imagefigure.h

#FORMS    += imagefigure.ui
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool
LIBS += -L./../../../pluginTool
TEMPLATE = lib
CONFIG += plugin
TARGET = imageFigure
DESTDIR = ./../../../bin/plugins/imageFigure
SOURCES += imageFigure.cpp
HEADERS += imageFigure.h \
    Plugin.h \
    ./../../../pluginTool/SignalHolder.h \
    ../../../pluginTool/imageFigure.h
QT += opengl \
        xml \
    xmlpatterns
