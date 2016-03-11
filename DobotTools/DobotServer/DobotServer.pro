#-------------------------------------------------
#
# Project created by QtCreator 2015-12-27T21:56:05
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DobotServer
TEMPLATE = app


SOURCES += \
    src/main.cpp\
    src/MainWindow.cpp\
    src/COptions.cpp\
    src/DobotServer.cpp\
    src/COptionsDialog.cpp

HEADERS  += \
    src/MainWindow.h\
    src/COptions.h\
    src/DobotServer.h\
    src/COptionsDialog.h

SOURCES +=\
    src/QDeviceWatcher\qdevicewatcher.cpp\
    src/QDeviceWatcher\qdevicewatcher_win32.cpp

HEADERS +=\
    src/QDeviceWatcher/qdevicewatcher.h\
    src/QDeviceWatcher/qdevicewatcher_p.h

INCLUDEPATH +=\
    src/QDeviceWatcher

RESOURCES += \
    res/res.qrc

RC_FILE = res/version.rc
