#-------------------------------------------------
#
# Project created by QtCreator 2015-09-09T15:00:05
#
#-------------------------------------------------

QT       += core gui
QT += widgets serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = oscilloscop
TEMPLATE = app


SOURCES += main.cpp\
    renderarea.cpp \
    mainwindow.cpp \
    parser.cpp

HEADERS  += \
    renderarea.h \
    mainwindow.h \
    define.h \
    parser.h

LIBS += -lpthread
#INCLUDEPATH +=

RESOURCES += \
    image.qrc
