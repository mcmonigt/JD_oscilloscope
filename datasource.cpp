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

    // starts time
    t.start();


//    generateData(0, 5, 1024);
    readData();
}

int DataSource::timeElapsed(){
    return t.elapsed();
}

void DataSource::update(QAbstractSeries *series, int series_num)
{
//    readData();
    if (series_num == 1){
        if (series) {
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

//            qDebug() << "plotting 3";
            xySeries->append(t.elapsed(), 1);




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
//            qDebug() << "plotting 2";
            xySeries->append(t.elapsed(), 2);





//            m_index++;
//            if (m_index > m_data2.count() - 1)
//                m_index = 0;

//            QVector<QPointF> points = m_data2.at(m_index);
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

void DataSource::readData()
{
//    m_data1.clear();
//    m_data2.clear();
//    QVector<QPointF> points_ch_1;
//    QVector<QPointF> points_ch_2;
//    qreal points_ch_1;
//    Q\ points_ch_2;
//    points_ch_1.reserve(100);
//    points_ch_2.reserve(100);


//    char buf[100], buf2[100], data[100];
//    int ch1_0, ch1_1, ch1_2, ch1_3;
//    int ch2_0, ch2_1, ch2_2, ch2_3;
//    int r1, r2, r3, sw;
//    int marker = 0;
//    if(!m_serialPort->waitForReadyRead((-1))){
//            qDebug() << "error reading second byte array";
//    }
//    qint64 lineLength = m_serialPort->readLine(buf, sizeof(buf));
//    if(!m_serialPort->waitForReadyRead((-1))){
//            qDebug() << "error reading second byte array";
//    }
//    qint64 lineLength2 = m_serialPort->readLine(buf2, sizeof(buf2));
//    strcpy(data, buf);
//    strcat(data, buf2);

//    for (int i = 0; i < 100; i++){
//        if (data[i] == 's'){
//            i++;
//            ch1_0 = data[i++] - 48;
//            ch1_1 = data[i++] - 48;
//            ch1_2 = data[i++] - 48;
//            ch1_3 = data[i++] - 48;
//            ch2_0 = data[i++] - 48;
//            ch2_1 = data[i++] - 48;
//            ch2_2 = data[i++] - 48;
//            ch2_3 = data[i++] - 48;
//            r1 = data[i++] - 48;
//            r2 = data[i++] - 48;
//            r3 = data[i++] - 48;
//            sw = data[i++] - 48;
//            marker = 1;
//            break;
//        }
//    }
//    if (marker == 0){
//        qDebug() << "ERROR: NO SERIAL DATA READ";
//        return;
//    }

//    int raw_channel1 = ch1_0 * 1000 + ch1_1 * 100 + ch1_2 * 10 + ch1_3;
//    int raw_channel2 = ch2_0 * 1000 + ch2_1 * 100 + ch2_2 * 10 + ch2_3;
////        points_ch_1.append(QPointF(raw_channel1, i));
////        points_ch_2.append(QPointF(raw_channel2, i));
//    points_ch_1.append(QPointF(t.elapsed(), 3));
//    points_ch_2.append(QPointF(t.elapsed(), 4));


//    for (int i = 0; i < 100; i++){
//        char buf[100], buf2[100], data[100];
//        int ch1_0, ch1_1, ch1_2, ch1_3;
//        int ch2_0, ch2_1, ch2_2, ch2_3;
//        int r1, r2, r3, sw;
//        int marker = 0;
//        if(!m_serialPort->waitForReadyRead((-1))){
//                qDebug() << "error reading second byte array";
//        }
//        qint64 lineLength = m_serialPort->readLine(buf, sizeof(buf));
//        if(!m_serialPort->waitForReadyRead((-1))){
//                qDebug() << "error reading second byte array";
//        }
//        qint64 lineLength2 = m_serialPort->readLine(buf2, sizeof(buf2));
//        strcpy(data, buf);
//        strcat(data, buf2);

//        for (int i = 0; i < 100; i++){
//            if (data[i] == 's'){
//                i++;
//                ch1_0 = data[i++] - 48;
//                ch1_1 = data[i++] - 48;
//                ch1_2 = data[i++] - 48;
//                ch1_3 = data[i++] - 48;
//                ch2_0 = data[i++] - 48;
//                ch2_1 = data[i++] - 48;
//                ch2_2 = data[i++] - 48;
//                ch2_3 = data[i++] - 48;
//                r1 = data[i++] - 48;
//                r2 = data[i++] - 48;
//                r3 = data[i++] - 48;
//                sw = data[i++] - 48;
//                marker = 1;
//                break;
//            }
//        }
//        if (marker == 0){
//            qDebug() << "ERROR: NO SERIAL DATA READ";
//            return;
//        }

//        int raw_channel1 = ch1_0 * 1000 + ch1_1 * 100 + ch1_2 * 10 + ch1_3;
//        int raw_channel2 = ch2_0 * 1000 + ch2_1 * 100 + ch2_2 * 10 + ch2_3;
    //        points_ch_1.append(QPointF(raw_channel1, i));
    //        points_ch_2.append(QPointF(raw_channel2, i));
//        points_ch_1.append(QPointF(t.elapsed(), 3));
//        points_ch_2.append(QPointF(t.elapsed(), 4));
//    }
//    m_data1.append(points_ch_1);
//    m_data2.append(points_ch_2);


//    qDebug() << "channel_1: " << raw_channel1;
//    qDebug() << "channel_2: " << raw_channel2;
//    qDebug() << "rotary 1: " << r1;
//    qDebug() << "rotary 2: " << r2;
//    qDebug() << "rotary 3: " << r3;
//    qDebug() << "switch: " << sw;









//    // clear previous data
//    m_data.clear();

//    // append new data

//    QFile inputFile(QString("/home/tomas/Documents/spring2019/ece342/Oscilloscope/Code/data_signal1.txt"));
//    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)){
////        qDebug() << " file opened";
//        QTextStream in(&inputFile);
//        qreal x(0);
//        qreal y(0);
//        QVector<QPointF> points;
//        points.reserve(1024);
//        while(!in.atEnd()){
//            in >> x >> y;

////            std::cout << "x: " << x << " y: " << y << std::endl;
//            if (x != 0 && y != 0){
//                points.append(QPointF(x, y));
//            }
////            points.append(QPointF(x, y));


//        }
//        m_data.append(points);
//        inputFile.close();
//    }
//    else {
//        qDebug() << " Could not open file for writing";
//    }

}

void DataSource::plotData(int type, int value){
//    if (series){
//        *series1 = static_cast<QXYSeries*>(series)
//    }
//    switch (type) {
//        case 0:
//            // channel 1
//            points.append(QPointF(t.elapsed(), value));
//        case 1:
//            // channel 2
//            points.append(QPointF(t.elapsed(), value));
//    }

}
