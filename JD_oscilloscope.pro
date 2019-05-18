QT += charts qml quick serialport core

#QMAKE_CXXFLAGS = -L. -lftd2xx -Wl,-rpath,/usr/local/lib

HEADERS += \
    WinTypes.h \
    datasource.h \
    ftd2xx.h

SOURCES += \
    main.cpp \
    datasource.cpp

RESOURCES += \
    resources.qrc

DISTFILES += \
    qml/qmloscilloscope/*

target.path = $$[QT_INSTALL_EXAMPLES]/charts/qmloscilloscope
INSTALLS += target

unix|win32: LIBS += -lftd2xx

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/release/build/release/ -lftd2xx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/release/build/debug/ -lftd2xx
else:unix: LIBS += -L$$PWD/release/build/ -lftd2xx

INCLUDEPATH += $$PWD/release/build
DEPENDPATH += $$PWD/release/build

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/release/build/release/libftd2xx.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/release/build/debug/libftd2xx.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/release/build/release/ftd2xx.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/release/build/debug/ftd2xx.lib
else:unix: PRE_TARGETDEPS += $$PWD/release/build/libftd2xx.a
