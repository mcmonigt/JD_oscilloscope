#ifndef READFIFO_H
#define READFIFO_H

#include <QtCore/QObject>
#include <QTime>
#include <QtCore/QPointF>
#include <QtCore/QVector>
#include <ftdi/ftd2xx.h>
#include <QElapsedTimer>
#include <QtCore/QDebug>
#include <QDebug>
#include <iostream>
#include <QIODevice>
#include <stdio.h>
#include <string.h>
#include <ftdi/ftd2xx.h>
#include <unistd.h>
#include <QElapsedTimer>
#include <iomanip>
#include <QThread>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QMetaObject>
#define MAX_DATA 10000

#define BUFFER_SIZE 130000
#define BUFF_SIZE 130000
#define MAX_PACKETS 10000

Q_DECLARE_METATYPE(QVector<QPointF>)
Q_DECLARE_METATYPE(qreal)

class ReadFifo : public QObject
{
    Q_OBJECT
public :
    ReadFifo(){
        qRegisterMetaType<QVector<QPointF>>();
        s1.reserve(MAX_DATA);
        s2.reserve(MAX_DATA);
        elapsedTimer.start();
        rotary1.reserve(4);
        rotary2.reserve(4);
        rotary3.reserve(4);
//        QQmlEngine engine;
//        QQmlComponent component(&engine, "/home/mcmonigt/Applications/qt_applications/JD_oscilloscope/qml/qmloscilloscope/main.qml");
//        object = component.create();
        initialize_fifo();

//        QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/qml/qmloscilloscope/ScopeView.qml")));
//        object = component.create();
    }
    virtual ~ReadFifo() {};
public slots:
    int readData_fifo();
    int get_sample_period();
    int initialize_fifo();
//    void setTriggerChannel1();
//    void setTriggerChannel2();
signals:
    void resultReady(QVector<QPointF> s1, QVector<QPointF> s2);
    void signal_rotary1(QVariant r1);
    void signal_rotary2(QVariant r2);
    void signal_rotary3(QVariant r3);
    void signal_channel_switch(QVariant sw);
//    void signalTriggerChannel1Changed(qreal value);
//    void signalTriggerChannel2Changed(qreal value);
private:
//    QQmlComponent component;
//    QObject* object;
    qreal timeScale;
    int period_average = 1000;
    qreal num_samples = 1;
    QTime t;
    FT_HANDLE fthandle1;
    qint64 time1 = 0;
    double sample_period;
    QElapsedTimer elapsedTimer;
    QVector<QPointF> s1;
    QVector<QPointF> s2;
    QVector<int> rotary1;
    QVector<int> rotary2;
    QVector<int> rotary3;
    int channel_switch;
//    qreal triggerValue1;
//    qreal triggerValue2;
};

#endif // READFIFO_H
