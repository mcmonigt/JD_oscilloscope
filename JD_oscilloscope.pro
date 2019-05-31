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

