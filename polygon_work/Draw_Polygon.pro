#-------------------------------------------------
#
# Project created by QtCreator 2017-10-04T00:10:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Draw_Polygon
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    imgwidget.cpp \
    polygon.cpp \
    utility.cpp

HEADERS += \
        mainwindow.h \
    imgwidget.h \
    polygon.h \
    utility.h

FORMS += \
        mainwindow.ui \
    imgwidget.ui
