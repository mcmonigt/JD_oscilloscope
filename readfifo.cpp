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

#define MAX_DATA 10000
#define VCC 3.3
#define BUFFER_SIZE 130000
#define BUFF_SIZE 130000
#define MAX_PACKETS 10000
#define CLOCK_WISE 1
#define COUNTER_CLOCK_WISE 0


int ReadFifo::initialize_fifo(){
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

    // initializes rotary enconders
    for (int i = 0; i < 4; i++){
        rotary1.append(i);
        rotary2.append(i);
        rotary3.append(i);
    }

    // initializes channel switch to channel 1
    channel_switch = 0;

    // initialized trigget value to 0
//    triggerValue1 = 4;
//    triggerValue2 = 4;

    return 0;

}

int ReadFifo::readData_fifo(){
    while(1){
        // ******************************* INITIALIZATION OF VARIABLES ****************************************
//            std::cout << "entering read data fifo" << std::endl;
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

        status = FT_GetStatus(fthandle1, &RxBytes, &TxBytes, &EventDword);
//        status = FT_Read(fthandle1, buffer, RxBytes, &data_read);
        elapsedTimer.restart();
        status = FT_Read(fthandle1, buffer, BUFFER_SIZE, &data_read);
        sample_period = (elapsedTimer.nsecsElapsed())/(BUFFER_SIZE/13);
//        std::cout << "bytes read: " << RxBytes << std::endl;
        // weighted average - same way TCP throughput is calculated
        period_average = (period_average * num_samples * 0.875 + sample_period * 0.125 * num_samples)/(num_samples + 1);
        num_samples++;
        sample_period = period_average;
        total_bytes_read = BUFFER_SIZE;
//        std::cout << "sample period = " << sample_period << std::endl;

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
            unsigned int indexS;
            for (int i = 0; i < 1000; i++){
                if (buffer[i] == 's'){
                    indexS = i;
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

                    if (channel_switch != sw){
                        channel_switch = sw;
                        std::cout << "channel_switch: " << channel_switch << std::endl;
                        emit signal_channel_switch(sw);
                    }

                    if (r1 != rotary1.value(0)){
//                        std::cout << "rotary1: " << r1 << std::endl;
                        rotary1.pop_back();
                        rotary1.push_front(r1);
                        int last = rotary1.value(0) * 1000 + rotary1.value(1) * 100 + rotary1.value(2) * 10 + rotary1.value(3);
                        if (last == 3201 || last == 2013 || last == 0132 || last == 1320){
                            emit signal_rotary1(1);
                        }
                        else if (last == 2310 || last == 3102 || last == 1023 || last == 0231){
                            emit signal_rotary1(0);
                        }

                    }
                    if (r2 != rotary2.value(0)){
//                        std::cout << "rotary2: " << r2 << std::endl;
                        rotary2.pop_back();
                        rotary2.push_front(r2);
                        int last = rotary2.value(0) * 1000 + rotary2.value(1) * 100 + rotary2.value(2) * 10 + rotary2.value(3);
                        if (last == 3201 || last == 2013 || last == 0132 || last == 1320){
                            emit signal_rotary2(1);
                        }
                        else if (last == 2310 || last == 3102 || last == 1023 || last == 0231){
                            emit signal_rotary2(0);
                        }
                    }
                    if (r3 != rotary3.value(0)){
//                        std::cout << "rotary 3: " << r3 << std::endl;
                        rotary3.pop_back();
                        rotary3.push_front(r3);
                        int last = rotary3.value(0) * 1000 + rotary3.value(1) * 100 + rotary3.value(2) * 10 + rotary3.value(3);
                        if (last == 3201 || last == 2013 || last == 0132 || last == 1320){
                            emit signal_rotary3(1);
                        }
                        else if (last == 2310 || last == 3102 || last == 1023 || last == 0231){
                            emit signal_rotary3(0);
                        }

                    }


                    raw_channel1 = ch1_0 * 1000 + ch1_1 * 100 + ch1_2 * 10 + ch1_3;
                    raw_channel2 = ch2_0 * 1000 + ch2_1 * 100 + ch2_2 * 10 + ch2_3;

                    // converts raw adc value to voltage
                    voltage_channel1 = 2 * (VCC/2 - (raw_channel1/4095)*VCC) + 0.025;
                    voltage_channel2 = 2 * (VCC/2 - (raw_channel2/4095)*VCC) + 0.025;
                    counter++;
                    points_total_each_channel++;

                    // sets trigger to highest voltage value
//                    if (voltage_channel1 > triggerValue1){
//                        triggerValue1 = voltage_channel1;
//                    }
//                    if (voltage_channel2 > triggerValue2){
//                        triggerValue2 = voltage_channel2;
//                    }

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

        emit resultReady(s1, s2);
    }
    return 0;
}

int ReadFifo::get_sample_period(){
    DWORD RxBytes;	//bytes in rx  qeue
    DWORD TxBytes;	//bytes in tx queue
    DWORD EventDword;	//current event status state
    DWORD data_read; //num bytes read from device
    qreal total_bytes_read = 0;
    DWORD r_data_len;
    char data[65536];
    FT_STATUS status;
    while(elapsedTimer.nsecsElapsed() / 1000000000 < 10){
        status = FT_GetStatus(fthandle1, &RxBytes, &TxBytes, &EventDword);
        status = FT_Read(fthandle1, data, RxBytes, &data_read);
        total_bytes_read += data_read;
    }
    timeScale = elapsedTimer.nsecsElapsed() / total_bytes_read;
    std::cout << "timeScale: " << timeScale << std::endl;
    return 0;
}


