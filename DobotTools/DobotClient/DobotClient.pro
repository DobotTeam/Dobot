#-------------------------------------------------
#
# Project created by QtCreator 2015-12-28T22:23:37
#
#-------------------------------------------------

QT       += core gui network xml serialport printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DobotClient
TEMPLATE = app


SOURCES += \
        src/main.cpp\
        src/MainWindow.cpp\
        src/DobotClient.cpp\
        src/COptions.cpp\
        src/CConfigJogDialog.cpp\
        src/CConfigPlayBackDialog.cpp\
        src/CPlayBackManager.cpp\
        src/CConfigUserDefDialog.cpp\
        src/CEEGManager.cpp\
        src/CEEGCurveDialog.cpp\
        src/qcustomplot.cpp\

HEADERS  += \
        src/MainWindow.h\
        src/DobotClient.h\
        src/COptions.h\
        src/CConfigJogDialog.h\
        src/CConfigPlayBackDialog.h\
        src/CPlayBackManager.h\
        src/CConfigUserDefDialog.h\
        src/CEEGManager.h\
        src/CEEGCurveDialog.h\
        src/qcustomplot.h\

RESOURCES += \
    res/res.qrc

RC_FILE = res/version.rc
