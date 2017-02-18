QT += core
QT += gui

CONFIG += c++14

TARGET = untitled
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    qimageextend.cpp \
    pyramid.cpp \
    interestingpoints.cpp \
    kernel.cpp

HEADERS += \
    qimageextend.h \
    pyramid.h \
    interestingpoints.h \
    kernel.h
