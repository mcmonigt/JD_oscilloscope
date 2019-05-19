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
#include <ftdi/ftd2xx.h>
#include <unistd.h>
#include <QElapsedTimer>
//#include <sys/mman.h>
//#include <sys/types.h>
//#include <sys/wait.h>


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

    //itializes time scale to 10000 us
    timeScale = 10000;

    // starts time
    t.start();
    elapsedTimer.start();

    //maps memory for data structures to be able to be accessed by child process
//    *s1 = mmap(NULL, sizeof *s1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);



    // reserves memory for data structures
    s1.reserve(MAX_DATA);
    s2.reserve(MAX_DATA);
//    s1_x.reserve(MAX_DATA);
//    s1_y.reserve(MAX_DATA);
//    s2_x.reserve(MAX_DATA);
//    s2_y.reserve(MAX_DATA);


    // tests time scaling implementation
//    testData();

    // initializes serial port
    // setupSerial(serial);


//    generateData(0, 5, 1024);
//    readData();
    initialize_fifo();
    readData_fifo();
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
//                std::cout << "plotting point x: " << s1.value(i).x() << " y:" << s1.value(i).y() << std::endl;
                points.append(element);
//                printf("appending points\n");
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
    std::cout << "voltage channe 1: " << voltage_channel1 << " voltage channel2: " << voltage_channel2 << std::endl;
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

int DataSource::initialize_fifo(){
    FT_STATUS status;	//device status

    //0 is index of device since we're only opening 1 device
    status = FT_Open(0, &fthandle1);

    if (status != FT_OK) {	//if device is not opened succesfully
//        qDebug() << "open device status is not ok " << status;
        printf("open device status is not ok %d\n", status);
        return 0;
    }

    //set read and write timeouts as 500ms
    status = FT_SetTimeouts(fthandle1, 500, 500);

    if (status != FT_OK) {	//if timeout are not setup succesfully
//        printf("timeout device status not Ok %d\n", status);
        qDebug() << "timeout device status not ok " << status;
    }

    UCHAR MaskA = 0x00; // set data bus to inputs
    UCHAR modeA = 0x40; //configure ft232h into synch fifo mode
    //fifo mode must already have been programmed in eeprom
    //fifo mode must already have been programmed in eeprom

    //set the chip mode
    status = FT_SetBitMode(fthandle1, MaskA, modeA);

    if (status != FT_OK)//if mode seelect was not succesfull
        printf("mode A status not ok %d\n", status);

    usleep(500); // sleep for 500 microseconds

    return 0;
}

int DataSource::readData_fifo(){

    FT_STATUS status;
    int count = 0;

    DWORD RxBytes;	//bytes in rx  qeue
    DWORD TxBytes;	//bytes in tx queue
    DWORD EventDword;	//current event status state

    //get the values of rx,tx queue and event status
    status = FT_GetStatus(fthandle1, &RxBytes, &TxBytes, &EventDword);


//    printf("bytes in RX queue %d\n", RxBytes);



    char data[65536]; //buffer for incoming data
    DWORD r_data_len = RxBytes;		//bytes to be read from device
    DWORD data_read;		//num bytes read from device
    memset(data, '\0', sizeof(data));




    //read r_data_len number of bytes from device into data_in buffer
    status = FT_Read(fthandle1, data, r_data_len, &data_read);

//    std::cout << "data read: " << data_read << std::endl;


    if (status != FT_OK) {
        printf("status not OK %d\n", status);

        printf("total bytes read %d\n", count);
        return 0;
    }
    else {
        qreal raw_channel1, raw_channel2, voltage_channel1, voltage_channel2;
        int ch1_0, ch1_1, ch1_2, ch1_3;
        int ch2_0, ch2_1, ch2_2, ch2_3;
        int r1, r2, r3, sw;
        int marker = 0;
        unsigned int indexS;
        for (int i = 0; i < 1000; i++){
            if (data[i] == 's'){
                indexS = i;
//                std::cout << "found first s at index " << i << std::endl;
                marker = 1;
                break;
            }
        }
        if (marker == 0){
            std::cout << "ERROR: NO SERIAL DATA READ" << std::endl;
            return 0;
        }
        int points_total_each_channel = 0;
        unsigned int i = indexS;
        unsigned int index_last_byte = indexS + data_read;
        while (i < data_read - 13){ // - 13 because we must have at least one packet of data left at the end
            if (data[i] == 's'){
                i++;
                ch1_0 = data[i++] - '0';
                ch1_1 = data[i++] - '0';
                ch1_2 = data[i++] - '0';
                ch1_3 = data[i++] - '0';
                ch2_0 = data[i++] - '0';
                ch2_1 = data[i++] - '0';
                ch2_2 = data[i++] - '0';
                ch2_3 = data[i++] - '0';
                r1 = data[i++] - '0';
                r2 = data[i++] - '0';
                r3 = data[i++] - '0';
                sw = data[i++] - '0';

                raw_channel1 = ch1_0 * 1000 + ch1_1 * 100 + ch1_2 * 10 + ch1_3;
                raw_channel2 = ch2_0 * 1000 + ch2_1 * 100 + ch2_2 * 10 + ch2_3;

                // converts raw adc value to voltage
                voltage_channel1 = 2 * (VCC/2 - (raw_channel1/4095)*VCC);
                voltage_channel2 = 2 * (VCC/2 - (raw_channel2/4095)*VCC);
                std::cout << "voltage channel1: " << voltage_channel1 << " voltage channel2: " << voltage_channel2 << std::endl;

                std::cout << voltage_channel1 << std::endl;
                std::cout << voltage_channel2 << std::endl;

                points_total_each_channel++;

                // adds data to their respective data structures (vectors)
                qreal t_elapsed = elapsedTimer.nsecsElapsed() / 1000; // microseconds
                std::cout << t_elapsed << std::endl;
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

            }
            else {
                i++;
            }
        }
//        std::cout << "points total added: " << points_total_each_channel << std::endl;
        // converts raw adc value to voltage


        //close device
//        status = FT_Close(fthandle1);

        return 0;
    }
}
