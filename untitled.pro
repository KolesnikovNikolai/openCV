QT += core
QT += gui

CONFIG += c++14

TARGET = untitled
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    qimageextend.cpp \
    sobel.cpp \
    gauss.cpp \
    pyramid.cpp

HEADERS += \
    qimageextend.h \
    sobel.h \
    gauss.h \
    pyramid.h
