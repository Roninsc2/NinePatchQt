#-------------------------------------------------
#
# Project created by QtCreator 2015-04-04T00:51:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NinePacth
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    ninepatch.cpp

HEADERS  += widget.h \
    ninepatch.h

FORMS    += widget.ui

QMAKE_CXXFLAGS += -std=c++11 -Werror
