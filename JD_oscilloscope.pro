############################################################
# Project: JD_oscilloscope
# File Name: datasource.h
# Modified  and Adapted by Tomas McMonigal
# Original files provided by The Qt Company Ltd. under the terms
# of the GNU Free Documentation License Version 1.3 published by the
# Free Software Foundation.
# Date Modified: 4/14/19
# Description: Qt Makefile
###############################################################

QT += charts qml quick serialport core

HEADERS += \
    datasource.h \
    readfifo.h

SOURCES += \
    main.cpp \
    datasource.cpp \
    readfifo.cpp

RESOURCES += \
    resources.qrc

DISTFILES += \
    qml/qmloscilloscope/*

target.path = $$[QT_INSTALL_EXAMPLES]/charts/qmloscilloscope
INSTALLS += target

unix|win32: LIBS += -L/usr/include/local/lib -lftd2xx

