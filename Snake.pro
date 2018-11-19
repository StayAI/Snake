#-------------------------------------------------
#
# Project created by QtCreator 2017-11-16T19:13:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Snake
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    snake.cpp

HEADERS  += widget.h \
    snake.h

FORMS    += widget.ui

RESOURCES += \
    image.qrc

