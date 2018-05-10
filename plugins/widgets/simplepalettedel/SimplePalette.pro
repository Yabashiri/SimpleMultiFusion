# Красноставская Яна, 2018

TEMPLATE = lib
TARGET = SimplePalette
CONFIG += plugin
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool
LIBS += -L./../../../pluginTool
DESTDIR = ./../../bin/plugins/SimplePalette

HEADERS += SimplePalette.h \
    ./../../../pluginTool/Plugin.h \
    ./../../../pluginTool/SignalHolder.h
SOURCES += SimplePalette.cpp
QT += xml \
    xmlpatterns \
    widgets
