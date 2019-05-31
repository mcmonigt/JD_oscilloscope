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
#include "readfifo.h"
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
#define VCC 3.3
#define BUFFER_SIZE 130000
#define BUFF_SIZE 130000
#define MAX_PACKETS 10000

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)


DataSource::DataSource(QQuickView *appViewer, QSerialPort *serial, QObject *parent) :
    QObject(parent),
    m_appViewer(appViewer),
    m_serialPort(serial),
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
    trigger1 = 4;
    trigger2 = 4;

    // starts time
    t.start();
    elapsedTimer.start();

    //maps memory for data structures to be able to be accessed by child process
//    *s1 = mmap(NULL, sizeof *s1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);


    channel_switch = 0;

    trigger1Set = 1;
    trigger2Set = 1;

    lastYValue1 = 0;
    lastYValue2 = 0;

    // reserves memory for data structures
    s1.reserve(MAX_DATA);
    s2.reserve(MAX_DATA);

    // creating new ReadFifo class
    ReadFifo *worker = new ReadFifo;
    // initializing the object as
    worker->moveToThread(&workerThread);
    //connecting the operate signal from DataSource to the readData function from ReadFifo
    QObject::connect(this, &DataSource::operate, worker, &ReadFifo::readData_fifo);
    // connecting the resultReady functoin from ReadFifo to the update_data function from DataSource
    QObject::connect(worker, &ReadFifo::resultReady, this, &DataSource::update_data);
    // connecting the connect the signal_rotary3 signal from ReadFifo to the modifyTimeScale function from DataSource
    QObject::connect(worker, &ReadFifo::signal_rotary1, this, &DataSource::modifyTimeScale);
    // connecting the signal_channel_switch signal from ReadFifoto the set_channel_switch function from DataSource
    QObject::connect(worker, &ReadFifo::signal_channel_switch, this, &DataSource::set_channel_switch);

    QObject::connect(worker, &ReadFifo::signal_rotary2, this, &DataSource::modifyVoltageScale);

    QObject::connect(worker, &ReadFifo::signal_rotary3, this, &DataSource::changeTrigger1);

    workerThread.start();
    emit operate();


    // tests time scaling implementation
//    testData();



//    QQmlEngine engine;
//    QQmlComponent component (&engine, "/home/mcmonigt/Applications/qt_applications/JD_oscilloscope/qml/qmloscilloscope/main.qml");
//    QObject *object = component.create();

//    QVariant returnedValue;
//    QVariant msg = "Hello from C++";
//    QMetaObject::invokeMethod(object, "myQmlFunction",
//        Q_RETURN_ARG(QVariant, returnedValue),
//        Q_ARG(QVariant, msg));

//    qDebug() << "QML function returned:" << returnedValue.toString();
}


//void DataSource::triggerChannel1Changed(qreal value){
//    std::cout << "setting trigger max value channel 1 to " << value << std::endl;
//    triggerValue1 = value;
//}

//void DataSource::triggetChannel2Changed(qreal value){
//    std::cout << "setting trigger max value channel 2 to " << value << std::endl;
//    triggerValue2 = value;
//}


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


void DataSource::checkTrigger(){
    if (trigger1Set == 0){
        if (maxValueChannel1 > trigger1){
            trigger1Set = 1;
        }
    }
    else if (trigger1Set == 1){
        if (trigger1 > maxValueChannel1){
            trigger1Set = 0;
        }
    }
}




void DataSource::modifyTimeScale(QVariant value){
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

void DataSource::changeTrigger1(QVariant value){
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


void DataSource::modifyVoltageScale(QVariant value){
    std::cout << "DataSource: modifyVoltageScale" << std::endl;
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


void DataSource::set_channel_switch(QVariant sw){
    if (sw == 0){
        channel_switch = 0;
        emit signalChannelChanged1();
    }
    else if (sw == 1){
        channel_switch = 1;
        emit signalChannelChanged2();
    }
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
    QMutexLocker m1(&m_Mutex);
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
//            std::cout << "reading data" << std::endl;
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
            QPointF first_element = s1.value(first_element_index);

            updateTrigger(first_element_index);

            // appends to points elements within window given with the new x-axis values for window to start from minX
            qreal maxValue = 0;
//            std::cout << "reading data" << std::endl;
            for (int i = first_element_index; i < s1.size(); i++){
                qreal x_axis = s1.value(i).x() - first_element.x() + minX;
//                qDebug() << "adding element x:" << x_axis << " y:" << s1.value(i).y();
                qreal y_axis = s1.value(i).y();
                if (y_axis > maxValue){
                    maxValue = y_axis;
                }
                QPointF element(x_axis, y_axis);
//                std::cout << "plotting point x: " << s1.value(i).x() << " y:" << s1.value(i).y() << std::endl;
                points.append(element);
//                printf("appending points\n");
            }
//            std::cout << "exiting reading data" << std::endl;
            // plots window
//            maxValueChannel1 = maxValue;
//            checkTrigger();
//            xySeries->replace(points);
            if (!trigger1Set){
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
//                qDebug() << "adding element x:" << x_axis << " y:" << s2.value(i).y();
                QPointF element(x_axis, s2.value(i).y());
                points.append(element);
            }
            // plots window
            if (!trigger2Set){
                xySeries->replace(points);
            }
            lastYValue2 = first_element.y();
//            xySeries->replace(points);
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

    // gets speed
//    char data[65536]; //buffer for incoming data
//    DWORD r_data_len = RxBytes;		//bytes to be read from device
//    memset(data, '\0', sizeof(data));
//    int counter = 0;
//    t.restart();
//    double total_data_read = 0;
//    while (t.elapsed() != 1000){
////        status = FT_Read(fthandle1, data, r_data_len, &data_read);
//        FT_Read(fthandle1, data, r_data_len, &data_read);
//        std::cout << "bytes read " << data_read << std::endl;
//        total_data_read += data_read;
//        counter++;
//    }
//    double
//    std::cout << "packets read : " << (total_data_read/13)/ << std::endl;

    elapsedTimer.restart();
    return 0;

}



int DataSource::readData_fifo(){
    while(1){
        // ******************************* INITIALIZATION OF VARIABLES ****************************************

        DWORD RxBytes;	//bytes in rx  qeue
        DWORD TxBytes;	//bytes in tx queue
        DWORD EventDword;	//current event status state
        DWORD data_read; //num bytes read from device
        DWORD total_bytes_read = 0;
        char buffer[BUFF_SIZE];
        memset(buffer, '\0', sizeof(buffer));
        FT_STATUS status;
        int count = 0;
        // ******************* READING DATA ***********************

        elapsedTimer.restart();
        status = FT_Read(fthandle1, buffer, BUFFER_SIZE, &data_read);
        sample_period = (elapsedTimer.nsecsElapsed())/(BUFFER_SIZE/13);
        period_average = (period_average * num_samples + sample_period)/(num_samples + 1);
        num_samples++;
        sample_period = period_average;
        total_bytes_read = BUFFER_SIZE;
        std::cout << "sample period = " << sample_period << std::endl;
        if (status != FT_OK) {
            printf("status not OK %d\n", status);
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
                if (buffer[i] == 's'){
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
            qreal counter = 0;
            s1.clear();
            s2.clear();
            while (i < total_bytes_read - 13){ // - 13 because we must have at least one packet of data left at the end
                if (buffer[i] == 's'){
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

                    raw_channel1 = ch1_0 * 1000 + ch1_1 * 100 + ch1_2 * 10 + ch1_3;
                    raw_channel2 = ch2_0 * 1000 + ch2_1 * 100 + ch2_2 * 10 + ch2_3;

                    // converts raw adc value to voltage
                    voltage_channel1 = 2 * (VCC/2 - (raw_channel1/4095)*VCC) + 0.025;
                    voltage_channel2 = 2 * (VCC/2 - (raw_channel2/4095)*VCC) + 0.025;
                    counter++;
                    points_total_each_channel++;

                    // adds data to their respective data structures (vectors)
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
                    i++;
                }
            }
            //close device
    //        status = FT_Close(fthandle1);

        }
        return 0;
    }

}

void DataSource::update_data(QVector<QPointF> vector1, QVector<QPointF> vector2){
    QMutexLocker m1(&m_Mutex);
//    std::cout << "entering update data" << std::endl;
//    std::cout << "writing data" << std::endl;

    s1.clear();
    s2.clear();
    s1 = vector1;
    s2 = vector2;
//    std::cout << "exiting writing data" << std::endl;
}

int DataSource::read_data(){
    std::cout << "entering read_data_funtion" << std::endl;
    // initializing variables for reading data
    DWORD RxBytes;	//bytes in rx  qeue
    DWORD TxBytes;	//bytes in tx queue
    DWORD EventDword;	//current event status state
    DWORD data_read; //num bytes read from device
    DWORD total_bytes_read = 0;
    char buffer[BUFFER_SIZE];
    char data[65336];
    memset(data, '\0', sizeof(data));
    memset(buffer, '\0', sizeof(buffer));
    DWORD r_data_len = RxBytes;		//bytes to be read from device
    FT_STATUS status;
    int count = 0;
    int total_bytes_left = BUFFER_SIZE;

    // fill buffer array (130000 bytes)
    while (total_bytes_read < BUFFER_SIZE){
        status = FT_GetStatus(fthandle1, &RxBytes, &TxBytes, &EventDword);
        status = FT_Read(fthandle1, data, r_data_len, &data_read);
        printf("data read %d\n", data_read);
        total_bytes_read += data_read;
        if (total_bytes_left - data_read <= 0){
            total_bytes_left = 0;
        }
        else {
            total_bytes_left -= data_read;
        }
        printf("total bytes left %d\n", total_bytes_left);
        size_t data_len;
        if (data_read <= total_bytes_left){
            data_len = data_read;
        }
        else if (total_bytes_left <= 0){
            data_len = 0;
        }
        else {
            data_len = total_bytes_left;
        }
        count += data_len;
        printf("data len: %d\n", data_len);
        strncat(buffer, data, data_len);
        memset(data, '\0', sizeof(data));
        return 0;
    }
    std::cout << "total bytes read " << count << std::endl;

    //total time divided in total amount of packets
    sample_period = (elapsedTimer.nsecsElapsed() - time1)/(total_bytes_read/13);
    std::cout << "sample period average for read call = " << sample_period << std::endl;
    if (status != FT_OK) {
        printf("status not OK %d\n", status);
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
            if (buffer[i] == 's'){
                indexS = i;
                std::cout << "found first s at index " << i << std::endl;
                marker = 1;
                break;
            }
        }
        if (marker == 0){
            std::cout << "ERROR: NO SERIAL DATA READ" << std::endl;
//            return 0;
        }
        int points_total_each_channel = 0;
        unsigned int i = indexS;
        qreal counter = 0;
        s1.clear();
        s2.clear();
        while (i < BUFFER_SIZE - 13){ // - 13 because we must have at least one packet of data left at the end
            if (buffer[i] == 's'){
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

                raw_channel1 = ch1_0 * 1000 + ch1_1 * 100 + ch1_2 * 10 + ch1_3;
                raw_channel2 = ch2_0 * 1000 + ch2_1 * 100 + ch2_2 * 10 + ch2_3;

                // converts raw adc value to voltage
                voltage_channel1 = 2 * (VCC/2 - (raw_channel1/4095)*VCC) + 0.025;
                voltage_channel2 = 2 * (VCC/2 - (raw_channel2/4095)*VCC) + 0.025;
                //                std::cout << "voltage channel1: " << voltage_channel1 << " voltage channel2: " << voltage_channel2 << std::endl;

                //                std::cout << voltage_channel1 << std::endl;
                //                std::cout << voltage_channel2 << std::endl;
                counter++;
                points_total_each_channel++;

                // adds data to their respective data structures (vectors)
                //                qreal t_elapsed = elapsedTimer.nsecsElapsed() / 1000; // microseconds
                //                std::cout << std::setprecision(20) << t_elapsed << std::endl;
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
                i++;
            }
        }

        //close device
        //        status = FT_Close(fthandle1);
        std::cout << "exiting read_data function" << std::endl;

        return 0;
    }
}
