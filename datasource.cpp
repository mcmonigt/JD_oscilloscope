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
//#include <fstream>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DataSource::DataSource(QQuickView *appViewer, QObject *parent) :
    QObject(parent),
    m_appViewer(appViewer),
    m_index(-1)
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    generateData(0, 5, 1024);
//    readData();
}

void DataSource::update(QAbstractSeries *series)
{
    readData();
//    if (series) {
//        QXYSeries *xySeries = static_cast<QXYSeries *>(series);
//        m_index++;
//        if (m_index > m_data.count() - 1)
//            m_index = 0;

//        QVector<QPointF> points = m_data.at(m_index);
//        // Use replace instead of clear + append, it's optimized for performance
//        xySeries->replace(points);
//    }
}

void DataSource::generateData(int type, int rowCount, int colCount)
{
    readData();
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
    // clear previous data
    m_data.clear();

    // append new data

    QFile inputFile(QString("/home/tomas/Documents/spring2019/ece342/Oscilloscope/Code/data_signal1.txt"));
    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << " file opened";
        QTextStream in(&inputFile);
        qreal x(0);
        qreal y(0);
        QVector<QPointF> points;
        points.reserve(1024);
        while(!in.atEnd()){
            in >> x >> y;

//            std::cout << "x: " << x << " y: " << y << std::endl;
            if (x != 0 && y != 0){
                points.append(QPointF(x, y));
            }
//            points.append(QPointF(x, y));


        }
        m_data.append(points);
        inputFile.close();
    }
    else {
        qDebug() << " Could not open file for writing";
    }


//    QFile inputFile(QString("home/tomas/Documents/spring2019/ece342/data_signal1"));
//    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)){
//        QTextStream in(&inputFile);
//        qreal x(0);
//        qreal y(0);
//        QVector<QPointF> points;
//        points.reserve(1024);
//        while(!in.atEnd()){
//            in >> x >> y;
//            points.append(QPointF(x, y));
//        }
//        m_data.append(points);
//        inputFile.close();
//    }
//    else {
//        qDebug() << " Could not open file for writing";
//    }

}
