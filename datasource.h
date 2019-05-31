/********************************************
 * Project: JD_oscilloscope
 * File Name: datasource.h
 * Modified and Adapted by Tomas McMonigal
 * Original files provided by The Qt Company Ltd. under the terms
 * of the GNU Free Documentation License Version 1.3 published by the
 * Free Software Foundation.
 * Date Modified: 4/14/19
 * Description:
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
//class QXYSeries;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class DataSource : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(int status_rotary3 READ status_rotary3 WRITE setStatus_rotary3 NOTIFY status_rotary3Changed)
    QThread workerThread;
public:
    explicit DataSource(QQuickView *appViewer,QSerialPort *serial, QObject *parent = 0);

Q_SIGNALS:

public slots:
    void update(QAbstractSeries *series, int series_num);
    int timeElapsed();
    void changeTimeScale(int time_scale);
    void testData();
    int readData_fifo();
    int initialize_fifo();
    int read_data();
    void set_channel_switch(QVariant sw);
    void modifyVoltageScale(QVariant value);
    void changeTrigger1(QVariant value);
    void modifyTimeScale(QVariant value);
    void update_data(QVector<QPointF> vector1, QVector<QPointF> vector2);
//    void triggerChannel1Changed(qreal value);
//    void triggetChannel2Changed(qreal value);
    void checkTrigger();
    void updateTrigger(int index);

//public:
//    int status_rotary3() const;
//    void setStatus_rotary3(int value);

signals:
    void operate();
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
//    void status_rotary3Changed();
private:
    QMutex m_Mutex;
    int m_status_rotary3;
    // Serial members
    QSerialPort *m_serialPort = nullptr;
    QByteArray m_readData;
    QTextStream m_standardOutput;
    QTime t;
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
    QQuickItem *chartView;
    int voltageScale1;
    int voltageScale2;
    qreal trigger1;
    qreal trigger2;
//    qreal triggerValue1;
//    qreal triggerValue2;
    int trigger1Set;
    int trigger2Set;
    qreal maxValueChannel1;
    qreal lastYValue1;
    qreal lastYValue2;
//    qreal maxValueChannel2;
    int mutex;
};

#endif // DATASOURCE_H
