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


QT_BEGIN_NAMESPACE
class QQuickView;
//class QXYSeries;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class DataSource : public QObject
{
    Q_OBJECT
public:
    explicit DataSource(QQuickView *appViewer,QSerialPort *serial, QObject *parent = 0);

Q_SIGNALS:

public slots:
    void generateData(int type, int rowCount, int colCount);
    void update(QAbstractSeries *series, int series_num);
    void readData(QAbstractSeries *series);
    int timeElapsed();
    void setupSerial(QSerialPort *serial);
    void changeTimeScale(int time_scale);
    void testData();
    int readData_fifo();
    int initialize_fifo();

private:
    // Serial members
    QSerialPort *m_serialPort = nullptr;
    QByteArray m_readData;
    QTextStream m_standardOutput;
    QTime t;
    FT_HANDLE fthandle1;

    QQuickView *m_appViewer;
    QList<QVector<QPointF> > m_data1;
    QList<QVector<QPointF> > m_data2;
//    QVector<QPointF> points;
//    QXYSeries *series1;
//    QXYSeries *series2;
    int m_index;

    // data structure members
    QVector<QPointF> s1;
    QVector<QPointF> s2;
    int timeScale;
    QElapsedTimer elapsedTimer;
};

#endif // DATASOURCE_H
