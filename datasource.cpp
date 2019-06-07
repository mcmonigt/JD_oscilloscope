/********************************************
 * Project: JD_oscilloscope
 * File Name: datasource.h
 * Modified and Adapted by Tomas McMonigal
 * Original files provided by The Qt Company Ltd. under the terms
 * of the GNU Free Documentation License Version 1.3 published by the
 * Free Software Foundation.
 * Date Modified: 5/31/19
 * Description: Reads data from FT232H Chip in FIFO mode, processes the data and stores it.
 * This data is then passed on to ScopeView.qml to be plotted.
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
#include <iomanip>
#include <string.h>
#include <QThread>
#include <QChartView>
#include <QGraphicsView>
#include <QChart>
#include <QQmlEngine>
#include <QString>
#include <QQmlApplicationEngine>

#define MAX_DATA 10000
#define VCC 3.345
#define BUFFER_SIZE 130000
#define BUFF_SIZE 130000
#define MAX_PACKETS 10000

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)


DataSource::DataSource(QQuickView *appViewer, QObject *parent) :
    QObject(parent),
    m_appViewer(appViewer),
    m_index(-1),
    m_status_rotary3(0)
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    //itializes time scale to 10000 us
    timeScale1 = 10000;
    timeScale2 = 10000;

    //sets voltage scales to initial value in qml
    voltageScale1 = 5;
    voltageScale2 = 5;

    //initializes triggers
    trigger1 = 0;
    trigger2 = 0;

    triggerON = 1;

    channel_switch = 0;

    trigger1Set = 1;
    trigger2Set = 1;

    lastYValue1 = 0;
    lastYValue2 = 0;

    // reserves memory for data structures
    s1.reserve(MAX_DATA);
    s2.reserve(MAX_DATA);


    // ReadFifo contructor
    elapsedTimer.start();
    rotary1.reserve(4);
    rotary2.reserve(4);
    rotary3.reserve(4);
    initialize_fifo();

}


void DataSource::updateTrigger(int index){
    if (channel_switch == 0){
        if (lastYValue1 < trigger1 && s1.value(index).y() > trigger1){
            trigger1Set = 0;
        }
        else{
            trigger1Set = 1;
        }
    }
    else if (channel_switch == 1){
        if (lastYValue2 < trigger2 && s2.value(index).y() > trigger2){
            trigger2Set = 0;
        }
        else{
            trigger2Set = 1;
        }
    }
}

void DataSource::triggerOnOff(){
    std::cout << "triggerOnOff" << std::endl;
    if (triggerON){
        triggerON = 0;
    }
    else {
        triggerON = 1;
    }
}




void DataSource::modifyTimeScale(int value){
//        std::cout << "DataSource:: entering modifyTimeScale" << std::endl;
    if (channel_switch == 0){
        if (value == 1){
            if (timeScale1 == 1000){
                emit signalTimeScale1_10000();
                timeScale1 = 10000;
            }
            else if (timeScale1 == 100){
                emit signalTimeScale1_1000();
                timeScale1 = 1000;
            }
            else if (timeScale1 == 10){
                emit signalTimeScale1_100();
                timeScale1 = 100;
            }
        }
        else if (value == 0){
            if (timeScale1 == 10000){
                emit signalTimeScale1_1000();
                timeScale1 = 1000;
            }
            else if (timeScale1 == 1000){
                emit signalTimeScale1_100();
                timeScale1 = 100;
            }
            else if (timeScale1 == 100){
                emit signalTimeScale1_10();
                timeScale1 = 10;
            }
        }
    }
    else if (channel_switch == 1){
        if (value == 1){
            if (timeScale2 == 1000){
                emit signalTimeScale2_10000();
                timeScale2 = 10000;
            }
            else if (timeScale2 == 100){
                emit signalTimeScale2_1000();
                timeScale2 = 1000;
            }
            else if (timeScale2 == 10){
                emit signalTimeScale2_100();
                timeScale2 = 100;
            }
        }
        else if (value == 0){
            if (timeScale2 == 10000){
                emit signalTimeScale2_1000();
                timeScale2 = 1000;
            }
            else if (timeScale2 == 1000){
                emit signalTimeScale2_100();
                timeScale2 = 100;
            }
            else if (timeScale2 == 100){
                emit signalTimeScale2_10();
                timeScale2 = 10;
            }
        }
    }
}

void DataSource::changeTrigger(int value){
    if (channel_switch == 0){
        if (value == 1){
            trigger1 += 0.05;
        }
        else if (value == 0){
            trigger1 -= 0.05;
        }
        std::cout << "trigger channel 1 value = " << trigger1 << std::endl;
    }
    else if (channel_switch == 1){
        if (value == 1){
            trigger2 += 0.05;
        }
        else if (value == 0){
            trigger2 -= 0.05;
        }
        std::cout << "trigger channel 2 value = " << trigger2 << std::endl;
    }
}


void DataSource::modifyVoltageScale(int value){
//    std::cout << "DataSource: modifyVoltageScale" << std::endl;
    // case when the switch is on channel 1
    if (channel_switch == 0){
        // case for increasing voltage scale
        if (value == 1){
            if (voltageScale1 == 1){
                voltageScale1 = 2;
                emit signalVoltageScale1_2();
            }
            else if (voltageScale1 == 2){
                voltageScale1 = 3;
                emit signalVoltageScale1_3();
            }
            else if (voltageScale1 == 3){
                voltageScale1 = 4;
                emit signalVoltageScale1_4();
            }
            else if (voltageScale1 == 4){
                voltageScale1 = 5;
                emit signalVoltageScale1_5();
            }
        }
        // case for decreasing voltage scale
        else if (value == 0){
            if (voltageScale1 == 5){
                voltageScale1 = 4;
                emit signalVoltageScale1_4();
            }
            else if (voltageScale1 == 4){
                voltageScale1 = 3;
                emit signalVoltageScale1_3();
            }
            else if (voltageScale1 == 3){
                voltageScale1 = 2;
                emit signalVoltageScale1_2();
            }
            else if (voltageScale1 == 2){
                voltageScale1 = 1;
                emit signalVoltageScale1_1();
            }
        }
    }
    // case when the switch is on channel 2
    else if (channel_switch == 1){
        // case for increasing voltage scale
        if (value == 1){
            if (voltageScale2 == 1){
                voltageScale2 = 2;
                emit signalVoltageScale2_2();
            }
            else if (voltageScale2 == 2){
                voltageScale2 = 3;
                emit signalVoltageScale2_3();
            }
            else if (voltageScale2 == 3){
                voltageScale2 = 4;
                emit signalVoltageScale2_4();
            }
            else if (voltageScale2 == 4){
                voltageScale2 = 5;
                emit signalVoltageScale2_5();
            }
        }
        // case for decreasing voltage scale
        else if (value == 0){
            if (voltageScale2 == 5){
                voltageScale2 = 4;
                emit signalVoltageScale2_4();
            }
            else if (voltageScale2 == 4){
                voltageScale2 = 3;
                emit signalVoltageScale2_3();
            }
            else if (voltageScale2 == 3){
                voltageScale2 = 2;
                emit signalVoltageScale2_2();
            }
            else if (voltageScale2 == 2){
                voltageScale2 = 1;
                emit signalVoltageScale2_1();
            }
        }
    }
}


void DataSource::set_channel_switch(int sw){
    if (sw == 0){
        channel_switch = 0;
        emit signalChannelChanged1();
    }
    else if (sw == 1){
        channel_switch = 1;
        emit signalChannelChanged2();
    }
}


void DataSource::testData(){
    for (int i = 0; i < MAX_DATA; i++){
        QPointF series1_data(i, 3);
        QPointF series2_data(i, 4);
        s1.append(series1_data);
        s2.append(series2_data);
    }
}

void DataSource::changeTimeScale(int time_scale){
    if (channel_switch == 0){
        timeScale1 = time_scale;
    }
    else if (channel_switch == 1){
        timeScale2 = time_scale;
    }
}

void DataSource::update(QAbstractSeries *series, int series_num)
{
    readData_fifo();
    if (series_num == 1){
        if (series) {
            int time_len = timeScale1;
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
                    first_element_index = i;
                    break;
                }
            }
            QPointF first_element = s1.value(first_element_index);

            updateTrigger(first_element_index);

            // appends to points elements within window given with the new x-axis values for window to start from minX
            qreal maxValue = 0;
            for (int i = first_element_index; i < s1.size(); i++){
                qreal x_axis = s1.value(i).x() - first_element.x() + minX;
                qreal y_axis = s1.value(i).y();
                if (y_axis > maxValue){
                    maxValue = y_axis;
                }
                QPointF element(x_axis, y_axis);
                points.append(element);
            }
            if (triggerON){
                if (!trigger1Set){
                    xySeries->replace(points);
                }
            }
            else {
                xySeries->replace(points);
            }

            lastYValue1 = first_element.y();
        }

    }
    else if (series_num == 2){

        if (series) {
            int time_len = timeScale2;
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
            QPointF first_element = s2.value(first_element_index);
            updateTrigger(first_element_index);

            // appends to points elements within window given with the new x-axis values for window to start from minX
            for (int i = first_element_index; i < s2.size(); i++){
                qreal x_axis = s2.value(i).x() - s2.value(first_element_index).x() + minX;
                QPointF element(x_axis, s2.value(i).y());
                points.append(element);
            }
            // plots window
            if (triggerON){
                if (!trigger2Set){
                    xySeries->replace(points);
                }
            }
            else {
                xySeries->replace(points);
            }
            lastYValue2 = first_element.y();
        }
    }

    else if (series_num == 4){

        if (series){
            int min = -5000;
            int max = 5000;
            QVector<QPointF> points;
            QXYSeries *xySeries = static_cast<QXYSeries *>(series);
            if (channel_switch == 0){
                for (int i = min; i <= max; i++){
                    qreal x_axis = i;
                    QPointF element(x_axis, trigger1);
                    points.append(element);
                }
                xySeries->replace(points);
            }
            else if (channel_switch == 1){
                for (int i = min; i <= max; i++){
                    qreal x_axis = i;
                    QPointF element(x_axis, trigger2);
                    points.append(element);
                }
                xySeries->replace(points);
            }
        }
    }
}


int DataSource::initialize_fifo(){
    std::cout << "initializing fifo" << std::endl;
    FT_STATUS status;	//device status

    //0 is index of device since we're only opening 1 device

    status = FT_Open(0, &fthandle1);

    if (status != FT_OK) {	//if device is not opened succesfully
//        qDebug() << "open device status is not ok " << status;
        printf("initialize_fifo: open device status is not ok %d\n", status);
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

    //set the chip mode
    status = FT_SetBitMode(fthandle1, MaskA, modeA);

    if (status != FT_OK)//if mode seelect was not succesfull
        printf("mode A status not ok %d\n", status);

    FT_SetUSBParameters(fthandle1,0x20000,0x20000);  //set buffer ft232h buffer size
    usleep(500); // sleep for 500 microseconds

    elapsedTimer.restart();

    // initializes rotary enconders
    for (int i = 0; i < 4; i++){
        rotary1.append(i);
        rotary2.append(i);
        rotary3.append(i);
    }

    return 0;

}



int DataSource::readData_fifo(){

    // ******************************* INITIALIZATION OF VARIABLES ****************************************
    //            std::cout << "entering read data fifo" << std::endl;
    DWORD data_read; //num bytes read from device
    DWORD total_bytes_read = 0;
    char buffer[BUFF_SIZE];
    memset(buffer, '\0', sizeof(buffer));
    FT_STATUS status;


    // ******************* READING DATA ***********************
    status = FT_Read(fthandle1, buffer, BUFFER_SIZE, &data_read);
    sample_period = 800;
    total_bytes_read = data_read;

    // ********** ERROR CHECKING ******************************
    if (status != FT_OK) {
        printf("status not OK %d\n", status);
        return 0;
    }

    // ******************* FINDING PACKET START POINT **********************************
    else {
        qreal raw_channel1, raw_channel2, voltage_channel1, voltage_channel2;
        int ch1_0, ch1_1, ch1_2, ch1_3;
        int ch2_0, ch2_1, ch2_2, ch2_3;
        int r1, r2, r3, sw;
        int marker = 0;
        int indexS;
        for (int i = 0; i < 1000; i++){
            if (buffer[i] == 's'){
                indexS = i;
//                std::cout << "found s index " << indexS << std::endl;
                marker = 1;
                break;
            }
        }

        // ************************** ERROR CHECKING **************************
        if (marker == 0){
            std::cout << "ERROR: DATA CORRUPTED" << std::endl;
            return 0;
        }

        // *********************** PROCESSING DATA **************************
        int points_total_each_channel = 0;
        int i = indexS;
        qreal counter = 0;
        s1.clear();
        s2.clear();
        while (i < total_bytes_read - 13){ // - 13 because we must have at least one packet of data left at the end
            if (buffer[i] == 's'){
//                std::cout << "saving data" << std::endl;
                i++;
                ch1_0 = buffer[i++] - '0';
                ch1_1 = buffer[i++] - '0';
                ch1_2 = buffer[i++] - '0';
                ch1_3 = buffer[i++] - '0';
                ch2_0 = buffer[i++] - '0';
                ch2_1 = buffer[i++] - '0';
                ch2_2 = buffer[i++] - '0';
                ch2_3 = buffer[i++] - '0';
                r1 = buffer[i++] - '0';
                r2 = buffer[i++] - '0';
                r3 = buffer[i++] - '0';
                sw = buffer[i++] - '0';

                if (channel_switch != sw){
                    channel_switch = sw;
                    std::cout << "channel_switch: " << channel_switch << std::endl;
                    set_channel_switch(sw);
                }

                if (r1 != rotary1.value(0)){
                    //                        std::cout << "rotary1: " << r1 << std::endl;
                    rotary1.pop_back();
                    rotary1.push_front(r1);
                    int last = rotary1.value(0) * 1000 + rotary1.value(1) * 100 + rotary1.value(2) * 10 + rotary1.value(3);
                    if (last == 3201 || last == 2013 || last == 0132 || last == 1320){
                        modifyTimeScale(1);
                    }
                    else if (last == 2310 || last == 3102 || last == 1023 || last == 0231){
                        modifyTimeScale(0);
                    }

                }
                if (r2 != rotary2.value(0)){
                    //                        std::cout << "rotary2: " << r2 << std::endl;
                    rotary2.pop_back();
                    rotary2.push_front(r2);
                    int last = rotary2.value(0) * 1000 + rotary2.value(1) * 100 + rotary2.value(2) * 10 + rotary2.value(3);
                    if (last == 3201 || last == 2013 || last == 0132 || last == 1320){
                        modifyVoltageScale(1);
                    }
                    else if (last == 2310 || last == 3102 || last == 1023 || last == 0231){
                        modifyVoltageScale(0);
                    }
                }
                if (r3 != rotary3.value(0)){
                    //                        std::cout << "rotary 3: " << r3 << std::endl;
                    rotary3.pop_back();
                    rotary3.push_front(r3);
                    int last = rotary3.value(0) * 1000 + rotary3.value(1) * 100 + rotary3.value(2) * 10 + rotary3.value(3);
                    if (last == 3201 || last == 2013 || last == 0132 || last == 1320){
                        changeTrigger(1);
                    }
                    else if (last == 2310 || last == 3102 || last == 1023 || last == 0231){
                        changeTrigger(0);
                    }

                }


                raw_channel1 = ch1_0 * 1000 + ch1_1 * 100 + ch1_2 * 10 + ch1_3;
                raw_channel2 = ch2_0 * 1000 + ch2_1 * 100 + ch2_2 * 10 + ch2_3;

                // converts raw adc value to voltage
                voltage_channel1 = 2 * (VCC/2 - (raw_channel1/4095)*VCC) + 0.054;
//                std::cout << "voltage channe 1 " << voltage_channel1 << std::endl;
                voltage_channel2 = 2 * (VCC/2 - (raw_channel2/4095)*VCC) + 0.01;
//               std::cout << "channel 2 " << voltage_channel2 << std::endl;
                counter++;
                points_total_each_channel++;

                QPointF point_s1((counter - 1) * sample_period / 1000, voltage_channel1);
                QPointF point_s2((counter - 1) * sample_period / 1000, voltage_channel2);
                if (s1.size() < MAX_DATA){
                    s1.append(point_s1);
                    //                    std::cout << "channel 1: " << point_s1.y() << std::endl;
                }
                if (s2.size() < MAX_DATA){
                    s2.append(point_s2);
                    //                    std::cout << "channel 2: " << point_s2.y() << std::endl;

                }
            }
            else {
                std::cout << "data corrupted index " << i << " " << buffer[i] << std::endl;
//                return 0;
                i++;
            }
        }
        //close device
        //        status = FT_Close(fthandle1);

    }
    return 0;
}
