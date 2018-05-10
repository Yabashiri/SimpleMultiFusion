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
TARGET = ColorPicker
DESTDIR = ./../../../bin/plugins/ColorPicker
SOURCES += ColorPicker.cpp
HEADERS += ColorPicker.h \
    Plugin.h \
    ./../../../pluginTool/SignalHolder.h
QT += xml \
    xmlpatterns \
    widgets
