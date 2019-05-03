/********************************************
 * Project: JD_oscilloscope
 * File Name: datasource.h
 * Modified  and Adapted by Tomas McMonigal
 * Original files provided by The Qt Company Ltd. under the terms
 * of the GNU Free Documentation License Version 1.3 published by the
 * Free Software Foundation.
 * Date Modified: 4/14/19
 * Description: hi
 ********************************************/



#include <QtWidgets/QApplication>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>
#include <QtCore/QDir>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "datasource.h"

int main(int argc, char *argv[])
{
    // The QApplication class manages the GUI application flow control and main settings
    // Qt Charts uses Qt Graphics View Framework for drawing, therefore, QApplication must be used
    QApplication app(argc, argv);

    // The QQuickView class provides a window for displaying a Qt Quick user interface
    QQuickView viewer;

    // The following are needed to make examples run without having to install the module
    // in desktop environments.
#ifdef Q_OS_WIN
    QString extraImportPath(QStringLiteral("%1/../../../../%2"));
#else
    QString extraImportPath(QStringLiteral("%1/../../../%2"));
#endif

    // QQmlEngine class provides an environment for instantiating QML components
    // Prior to creating any QML components, an application must have created a QQmlEngine to gain access to a QML context
    // This next two line adds a path directory where the engine searches for installed modules in a URL-based directory structure
    viewer.engine()->addImportPath(extraImportPath.arg(QGuiApplication::applicationDirPath(),
                                      QString::fromLatin1("qml")));

    // The fuction Qobject::connect(const QObject* sender, const char* signal, const QObject* receiver, const char* method)
    // creates a connection of the given type from the signal in the sender object to the method in the receiver object.
    // Basically, it allows the program to end when the window is closed.
    QObject::connect(viewer.engine(), &QQmlEngine::quit, &viewer, &QWindow::close);

    // Sets the tittle for the graph
    viewer.setTitle(QStringLiteral("JD Oscilloscope"));

    // Creates the object for serial communications
    QSerialPort serial;

    // Creates an object of class DataSource which is created and defined in datasource.h and datasource.cpp
    // the object viewer of class QQuickView is passed as an argument to the constructor of DataSource which
    // links it to the QQuickView private member of the DataSource class for later use
    DataSource dataSource(&viewer, &serial);

    // The following line accesses the QQmlContext class member, contexts allow data to be exposed to the QML components instantiated by the QML engine.
    // Each QQmlContext contains a set of properties, distinct from its QObject properties, that allow data to be explicitly bound to a context by name.
    // The context properties are defined and updated by calling QQmlContext::setContextProperty().
    viewer.rootContext()->setContextProperty("dataSource", &dataSource);

    // Sets the source to the url, loads the QML component and instantiates it
    viewer.setSource(QUrl("qrc:/qml/qmloscilloscope/main.qml"));

    // Sets the root item to automatically resize the view to the size of the view.
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);

    // sets the color of the window
    viewer.setColor(QColor("#404040"));

    // Allows the window to be displayed
    viewer.show();

//    QObject::connect(&serial, &QSerialPort::readyRead, [&]
//    {

//        dataSource.readData();
//    });
    QObject::connect(&serial,
                         static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>
                         (&QSerialPort::error),
                         [&](QSerialPort::SerialPortError error)
    {
        //this is called when a serial communication error occurs
        qDebug() << "An error occured: " << error;
        return qApp->quit();
    });

    // Executes the app
    return app.exec();
}
