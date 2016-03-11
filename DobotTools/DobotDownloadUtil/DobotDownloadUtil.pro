#-------------------------------------------------
#
# Project created by QtCreator 2015-11-30T22:36:03
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DobotDownloadUtil
TEMPLATE = app


SOURCES += src/main.cpp\
        src/MainWindow.cpp \
        src/CSelectSerialDialog.cpp

HEADERS  += src/MainWindow.h \
        src/CSelectSerialDialog.h


RESOURCES += \
    res/res.qrc

RC_ICONS = res/Dobot.ico
