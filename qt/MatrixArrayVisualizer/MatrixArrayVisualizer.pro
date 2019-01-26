#-------------------------------------------------
#
# Copyright (c) 2018
# @author Rafael Cisneros
#
# AIST
#
# All rights reserved.
#
#-------------------------------------------------

QT      += widgets

TARGET   = MatrixArrayVisualizer
TEMPLATE = app


SOURCES += main.cpp\
           widget.cpp\
           plot.cpp
           
HEADERS += widget.h\
           plot.h

INCLUDEPATH += ../3rdparty/qextserialport/src
LIBS +=  ../3rdparty/qextserialport/libQt5ExtSerialPort.so

INCLUDEPATH += ../3rdparty/qwt-6.1.4/src
LIBS += ../3rdparty/qwt-6.1.4/lib/libqwt.so
