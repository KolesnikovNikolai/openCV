QT += core
QT += gui

CONFIG += c++14

TARGET = untitled
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    pyramid.cpp \
    interestingpoints.cpp \
    kernel.cpp \
    image.cpp \
    descriptor.cpp

HEADERS += \
    pyramid.h \
    interestingpoints.h \
    kernel.h \
    image.h \
    descriptor.h
