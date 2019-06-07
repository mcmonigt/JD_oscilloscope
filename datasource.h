/********************************************
 * Project: JD_oscilloscope
 * File Name: datasource.h
 * Modified and Adapted by Tomas McMonigal
 * Original files provided by The Qt Company Ltd. under the terms
 * of the GNU Free Documentation License Version 1.3 published by the
 * Free Software Foundation.
 * Date Modified: 5/31/19
 * Description: Class definition file for DataSource
 ********************************************/

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QtCore/QObject>
#include <QtCharts/QAbstractSeries>
#include <QtSerialPort>
#include <QTextStream>
#include <QByteArray>
#include <QTime>
#include <QtCore/QIODevice>
#include <QtCore/QPointF>
#include <QtCore/QVector>
#include <QtCharts/QChartGlobal>
#include <ftdi/ftd2xx.h>
#include <QElapsedTimer>
#include <QQuickItem>
#include <QObject>
#include <iostream>
#include <QQmlEngine>
#include <QMutex>
#include <QMutexLocker>


QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class DataSource : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    explicit DataSource(QQuickView *appViewer, QObject *parent = 0);

Q_SIGNALS:

public slots:
    void update(QAbstractSeries *series, int series_num);
    void changeTimeScale(int time_scale);
    void testData();
    int readData_fifo();
    int initialize_fifo();
    void modifyVoltageScale(int value);
    void modifyTimeScale(int value);
    void changeTrigger(int value);
    void updateTrigger(int index);
    void set_channel_switch(int sw);
    void triggerOnOff();

signals:
    void signal_modifyTimeScale(QVariant value);
    void signalTimeScale1_10000();
    void signalTimeScale1_1000();
    void signalTimeScale1_100();
    void signalTimeScale1_10();
    void signalTimeScale2_10000();
    void signalTimeScale2_1000();
    void signalTimeScale2_100();
    void signalTimeScale2_10();
    void signalVoltageScale1_1();
    void signalVoltageScale1_2();
    void signalVoltageScale1_3();
    void signalVoltageScale1_4();
    void signalVoltageScale1_5();
    void signalVoltageScale2_1();
    void signalVoltageScale2_2();
    void signalVoltageScale2_3();
    void signalVoltageScale2_4();
    void signalVoltageScale2_5();
    void signalIncreaseTrigger1();
    void signalDecreaseTrigger1();
    void signalChannelChanged1();
    void signalChannelChanged2();
private:
    int m_status_rotary3;
    FT_HANDLE fthandle1;
//    QObject *object;

    qint64 time1 = 0;

    QQuickView *m_appViewer;
    QList<QVector<QPointF> > m_data1;
    QList<QVector<QPointF> > m_data2;
    int m_index;
    double sample_period;

    // data structure members
    QVector<QPointF> s1;
    QVector<QPointF> s2;
    qreal timeScale1;
    qreal timeScale2;
    int period_average = 1000;
    qreal num_samples = 1;
    QElapsedTimer elapsedTimer;
    QVector<int> voltageRotary;
    QVector<int> timeRotary;
    int channel_switch;
    int voltageScale1;
    int voltageScale2;
    qreal trigger1;
    qreal trigger2;
    int trigger1Set;
    int trigger2Set;
    qreal lastYValue1;
    qreal lastYValue2;
    int triggerON;



    // readfifo.h variables that are not repeated
    qreal timeScale;
    QVector<int> rotary1;
    QVector<int> rotary2;
    QVector<int> rotary3;

};

#endif // DATASOURCE_H
