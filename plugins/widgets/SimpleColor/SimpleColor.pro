# #####################################################################
# PenBox MultiFusion build script. Kukushkin Alexey, 2009
# #####################################################################
DEPENDPATH += . \
    ./../../../pluginTool
INCLUDEPATH += . \
    ./../../../pluginTool \
LIBS += -L./../../../pluginTool
TEMPLATE = lib
CONFIG += plugin
TARGET = SimpleColor
DESTDIR = ./../../../bin/plugins/SimpleColor
SOURCES += SimpleColor.cpp
HEADERS += SimpleColor.h \
    Plugin.h \
    ./../../../pluginTool/SignalHolder.h
QT += xml \
    xmlpatterns \
    widgets
