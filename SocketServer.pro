#-------------------------------------------------
#
# Project created by QtCreator 2013-02-18T10:05:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SocketServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connectionserver.cpp \
    server.cpp

HEADERS  += mainwindow.h \
    connectionserver.h \
    server.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
