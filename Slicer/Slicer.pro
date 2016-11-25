QT += core
QT -= gui

CONFIG += c++11

TARGET = Slicer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main_old.cpp \
    slicer.cpp \
    parser.cpp

unix|win32: LIBS += -L$$PWD/../../Libraries/libvlc/ -llibvlc

INCLUDEPATH += $$PWD/../../Libraries/libvlc
DEPENDPATH += $$PWD/../../Libraries/libvlc

HEADERS += \
    slicer.h \
    parser.h
