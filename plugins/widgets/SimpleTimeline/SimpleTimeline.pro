######################################################################
#       TimeLine SACS2 build script. Serkov Alexander, 2007
######################################################################
TEMPLATE = lib
TARGET = Timeline
CONFIG += plugin
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool
QT += xml \
    xmlpatterns \
    widgets
DESTDIR = ./../../../bin/plugins/SimpleTimeline
SOURCES += SimpleTimeline.cpp
HEADERS += SimpleTimeline.h \
    Plugin.h \
    ./../../../pluginTool/SignalHolder.h
RESOURCES += TimelineImages.qrc
