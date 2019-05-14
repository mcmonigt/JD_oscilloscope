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

#include "datasource.h"
#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <QIODevice>
#include <stdio.h>
#include <string.h>
#define MAX_DATA 10000
#define VCC 3.3

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DataSource::DataSource(QQuickView *appViewer, QSerialPort *serial, QObject *parent) :
    QObject(parent),
    m_appViewer(appViewer),
    m_serialPort(serial),
    m_index(-1)
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    //itializes time scale to 10000 ms
    timeScale = 10000;

    // starts time
    t.start();

    // reserves memory for data structures
    s1.reserve(MAX_DATA);
    s2.reserve(MAX_DATA);

    // tests time scaling implementation
    testData();

    // initializes serial port
    // setupSerial(serial);


//    generateData(0, 5, 1024);
//    readData();
}


int DataSource::timeElapsed(){
    return t.elapsed();
}

void DataSource::testData(){
    for (int i = 0; i < MAX_DATA; i++){
        QPointF series1_data(i, 3);
        QPointF series2_data(i, 4);
        s1.append(series1_data);
        s2.append(series2_data);
    }
}

void DataSource::setupSerial(QSerialPort* serial){
    // sets up serial communications
    serial->setPortName("ttyUSB0");
    if(!serial->setBaudRate(QSerialPort::Baud9600))
        qDebug() << serial->errorString();
    if(!serial->setDataBits(QSerialPort::Data8))
        qDebug() << serial->errorString();
    if(!serial->setParity(QSerialPort::NoParity))
        qDebug() << serial->errorString();
    if(!serial->setFlowControl(QSerialPort::NoFlowControl))
        qDebug() << serial->errorString();
    if(!serial->setStopBits(QSerialPort::OneStop))
        qDebug() << serial->errorString();
    if(!serial->open(QIODevice::ReadOnly))
        qDebug() << serial->errorString();
}

void DataSource::changeTimeScale(int time_scale){
    timeScale = time_scale;
}

void DataSource::update(QAbstractSeries *series, int series_num)
{
//    readData();
    if (series_num == 1){
        if (series) {
            int time_len = timeScale;
            // next two lines define boundaries of window (x-axis)
            int minX = -time_len/2;
            int maxX = time_len/2;
            // points is local variable used to store elements in the window give by time_len
            QVector<QPointF> points;
            QXYSeries *xySeries = static_cast<QXYSeries *>(series);
            // finds the first value to fit window size (x-axis) of length time_len
            QPointF last_point = s1.value(s1.size() - 1);
            qreal last_x_value = last_point.x();
            qreal window_start = last_x_value - time_len;
            int first_element_index;
            for (int i = 0; i < s1.size(); i++){
                if (s1.value(i).x() >= window_start){
//                    qDebug() << "finds first element";
                    first_element_index = i;
                    break;
                }
            }

            // appends to points elements within window given with the new x-axis values for window to start from minX
            for (int i = first_element_index; i < s1.size(); i++){
                qreal x_axis = s1.value(i).x() - s1.value(first_element_index).x() + minX;
//                qDebug() << "adding element x:" << x_axis << " y:" << s1.value(i).y();
                QPointF element(x_axis, s1.value(i).y());
                points.append(element);
            }
            // plots window
            xySeries->replace(points);

//            qreal start_point = s1.value(s1.size() - 1)


//            m_index++;
//            if (m_index > m_data1.count() - 1)
//                m_index = 0;

//            QVector<QPointF> points = m_data1.at(m_index);
//            // Use replace instead of clear + append, it's optimized for performance
//            xySeries->replace(points);
        }

    }
    else{

        if (series) {
            int time_len = timeScale;
            // next two lines define boundaries of window (x-axis)
            int minX = -time_len/2;
            int maxX = time_len/2;
            // points is local variable used to store elements in the window give by time_len
            QVector<QPointF> points;
            QXYSeries *xySeries = static_cast<QXYSeries *>(series);
            // finds the first value to fit window size (x-axis) of length time_len
            QPointF last_point = s2.value(s2.size() - 1);
            qreal last_x_value = last_point.x();
            qreal window_start = last_x_value - time_len;
            int first_element_index;
            for (int i = 0; i < s2.size(); i++){
                if (s2.value(i).x() >= window_start){
                    first_element_index = i;
                    break;
                }
            }

            // appends to points elements within window given with the new x-axis values for window to start from minX
            for (int i = first_element_index; i < s2.size(); i++){
                qreal x_axis = s2.value(i).x() - s2.value(first_element_index).x() + minX;
//                qDebug() << "adding element x:" << x_axis << " y:" << s2.value(i).y();
                QPointF element(x_axis, s2.value(i).y());
                points.append(element);
            }
            // plots window
            xySeries->replace(points);

//            qreal start_point = s1.value(s1.size() - 1)


//            m_index++;
//            if (m_index > m_data1.count() - 1)
//                m_index = 0;

//            QVector<QPointF> points = m_data1.at(m_index);
//            // Use replace instead of clear + append, it's optimized for performance
//            xySeries->replace(points);
        }
    }

}

void DataSource::generateData(int type, int rowCount, int colCount)
{
//    readData();
//    // Remove previous data
//    m_data.clear();

//    // Append the new data depending on the type
//    for (int i(0); i < rowCount; i++) {
//        QVector<QPointF> points;
//        points.reserve(colCount);
//        for (int j(0); j < colCount; j++) {
//            qreal x(0);
//            qreal y(0);
//            switch (type) {
//            case 0:
//                // data with sin + random component in the range [0, 1)
//                y = qSin(M_PI / 50 * j) + 0.5 + QRandomGenerator::global()->generateDouble();
//                x = j;
//                break;
//            case 1:
//                // linear data
//                x = j;
//                y = (qreal) i / 10;
//                break;
//            default:
//                // unknown, do nothing
//                break;
//            }
//            points.append(QPointF(x, y));
//        }
//        m_data.append(points);
//    }
}

void DataSource::readData(QAbstractSeries* series){
    QXYSeries *xySeries = static_cast<QXYSeries *>(series);
    char buf[100], buf2[100], data[100];
    int ch1_0, ch1_1, ch1_2, ch1_3;
    int ch2_0, ch2_1, ch2_2, ch2_3;
    int r1, r2, r3, sw;
    int marker = 0;
    if(!m_serialPort->waitForReadyRead((-1))){
            qDebug() << "error reading second byte array";
    }
    qint64 lineLength = m_serialPort->readLine(buf, sizeof(buf));
    if(!m_serialPort->waitForReadyRead((-1))){
            qDebug() << "error reading second byte array";
    }
    qint64 lineLength2 = m_serialPort->readLine(buf2, sizeof(buf2));
    strcpy(data, buf);
    strcat(data, buf2);
//            qDebug() << data;
    for (int i = 0; i < 100; i++){
        if (data[i] == 's'){
            i++;
            ch1_0 = data[i++] - 48;
            ch1_1 = data[i++] - 48;
            ch1_2 = data[i++] - 48;
            ch1_3 = data[i++] - 48;
            ch2_0 = data[i++] - 48;
            ch2_1 = data[i++] - 48;
            ch2_2 = data[i++] - 48;
            ch2_3 = data[i++] - 48;
            r1 = data[i++] - 48;
            r2 = data[i++] - 48;
            r3 = data[i++] - 48;
            sw = data[i++] - 48;
            marker = 1;
            break;
        }
    }
    if (marker == 0){
        qDebug() << "ERROR: NO SERIAL DATA READ";
        return;
    }
    qDebug() << ch1_0 << ch1_1 << ch1_2 << ch1_3 << ch2_0 << ch2_1 << ch2_2 << ch2_3 << r1 << r2 << r3 << sw;

    qreal raw_channel1 = ch1_0 * 1000 + ch1_1 * 100 + ch1_2 * 10 + ch1_3;
    qreal raw_channel2 = ch2_0 * 1000 + ch2_1 * 100 + ch2_2 * 10 + ch2_3;

    // converts raw adc value to voltage
    qreal voltage_channel1 = VCC/2 - (raw_channel1/4095)*VCC;
    qreal voltage_channel2 = VCC/2 - (raw_channel2/4095)*VCC;
    // adds data to their respective data structures (vectors)
    qreal t_elapsed = t.elapsed();
    QPointF point_s1(t_elapsed, voltage_channel1);
    QPointF point_s2(t_elapsed, voltage_channel2);
    if (s1.size() < MAX_DATA){
        s1.push_back(point_s1);
    }
    else {
        s1.removeFirst();
        s1.push_back(point_s1);
    }
    if (s2.size() < MAX_DATA){
        s2.push_back(point_s2);
    }
    else {
        s2.removeFirst();
        s2.push_back(point_s2);
    }

//    xySeries->append(t.elapsed(), 1);


}

