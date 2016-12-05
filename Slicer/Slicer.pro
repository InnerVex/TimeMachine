QT += core
QT -= gui

CONFIG += c++11

TARGET = Slicer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main_old.cpp \
    slicer.cpp \
    parser.cpp \
    pipe_server.cpp \
    socket_server.cpp \
    client.cpp \


HEADERS += \
    slicer.h \
    parser.h \
    pipe_server.h \
    socket_server.h \
    client.h


unix|win32: LIBS += -lQt5Network

unix|win32: LIBS += -lQt5Widgets
QT           += network widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/ipc/localfortuneserver
INSTALLS += target

unix|win32: LIBS += -L$$PWD/../../../Libraries/libvlc/ -llibvlc

INCLUDEPATH += $$PWD/../../../Libraries/libvlc/include
DEPENDPATH += $$PWD/../../../Libraries/libvlc/include
