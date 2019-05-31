/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtCharts 2.1

//![1]
ChartView {
    signal signalTimeScaleChanged2(int sampleCount)

    id: chartView
    animationOptions: ChartView.NoAnimation
    theme: ChartView.ChartThemeDark
    property bool openGL: true
    property bool openGLSupported: true
    property int xAxisScaleValue: 1000

    onOpenGLChanged: {
        if (openGLSupported) {
            series("Channel 1").useOpenGL = openGL;
            series("Channel 2").useOpenGL = openGL;
        }
    }
    Component.onCompleted: {
        if (!series("Channel 1").useOpenGL) {
            openGLSupported = false
            openGL = false
        }
//        if (!series("Channel 2").useOpenGl){
//            openGLSupported = false
//            openGL = false
//        }
    }

    ValueAxis {
        id: axisY1
        min: -5
        max: 5
        tickCount: 11
        titleText: "Voltage Channel 1"
    }

    ValueAxis {
        id: axisY2
        min: -5
        max: 5
        tickCount: 11
        titleText: "Voltage Channel 2"
    }

    ValueAxis {
        id: axisX
        min: -5000
        max: 5000
        tickCount: 11
        titleText: "Time Channel 1 (us)"
//        labelsVisible: false
    }
    ValueAxis {
        id: axisX2
        min: -5000
        max: 5000
        tickCount: 11
        titleText: "Time Channel 2 (us)"
    }

//    ValueAxis {
//        id: timeScale
//        min: 0
//        max: 10000
//        labelsVisible: true
//    }

    LineSeries {
        id: lineSeries1
        name: "Channel 1"
        axisX: axisX
        axisY: axisY1
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: lineSeries2
        name: "Channel 2"
        axisXTop: axisX2
        axisYRight: axisY2
        useOpenGL: chartView.openGL
    }
    LineSeries {
        id: trigger
        name: "Trigger"
        axisX: axisX
        axisY: axisY1
        useOpenGL: chartView.openGL
    }
//    LineSeries {
//        id: trigger2
//        name: "Trigger Channel 2"
//        axisXTop: axisX2
//        axisYRight: axisY2
//        useOpenGL: chartView.openGL
//    }

//![1]

    //![2]
    Timer {
        id: refreshTimer
        interval: 1 / 60 * 1000 // 60 Hz
        running: true
        repeat: true
        onTriggered: {
//            console.log("timer triggered");
//            dataSource.readData_fifo();
//            dataSource.read_data();
            dataSource.update(chartView.series(0), 1);
            dataSource.update(chartView.series(1), 2);
            dataSource.update(chartView.series(2), 4);
//            dataSource.update(chartView.series(3), 4);
//            updateTimeAxis();
        }
    }
//    Timer {
//        id : readDataTimer
//        interval: 0
//        running: false
//        repeat: true
//        onTriggered: {
//            dataSource.read_data();
//        }
//    }
    //![2]

    //![3]
    function changeSeriesType(type) {
        chartView.removeAllSeries();

        // Create two new series of the correct type. Axis x is the same for both of the series,
        // but the series have their own y-axes to make it possible to control the y-offset
        // of the "signal sources".
        if (type == "line") {
            var series1 = chartView.createSeries(ChartView.SeriesTypeLine, "Channel 1",
                                                 axisX, axisY1);
            series1.useOpenGL = chartView.openGL

            var series2 = chartView.createSeries(ChartView.SeriesTypeLine, "Channel 2",
                                                 axisX2, axisY2);
            series2.useOpenGL = chartView.openGL


        } else {
            var series1 = chartView.createSeries(ChartView.SeriesTypeScatter, "Channel 1",
                                                 axisX, axisY1);
            series1.markerSize = 2;
            series1.borderColor = "transparent";
            series1.useOpenGL = chartView.openGL

            var series2 = chartView.createSeries(ChartView.SeriesTypeScatter, "Channel 2",
                                                 axisX2, axisY2);
            series2.markerSize = 2;
            series2.borderColor = "transparent";
            series2.useOpenGL = chartView.openGL
        }
    }

    function createAxis(min, max) {
        // The following creates a ValueAxis object that can be then set as a x or y axis for a series
        return Qt.createQmlObject("import QtQuick 2.0; import QtCharts 2.0; ValueAxis { min: "
                                  + min + "; max: " + max + " }", chartView);
    }
    //![3]

    function setAnimations(enabled) {
        if (enabled)
            chartView.animationOptions = ChartView.SeriesAnimations;
        else
            chartView.animationOptions = ChartView.NoAnimation;
    }

    function changeRefreshRate(rate) {
        refreshTimer.interval = 1 / Number(rate) * 1000;
    }
    function changeVoltageScale1(newBoundary) {
//        console.log("changing voltage scale 1");
        axisY1.min = -newBoundary;
        axisY1.max = newBoundary;
    }
    function changeVoltageScale2(newBoundary){
        axisY2.min = -newBoundary;
        axisY2.max = newBoundary;
    }
    function changeAxisX2(value){
        axisX2.max = value;
        axisX2.min = -value;
    }

    function signal1Visible(enabled){
        if (enabled)
            series("Channel 1").visible = false;
        else
            series("Channel 1").visible = true;
    }

    function triggerVisible(enabled){
        if (enabled)
            series("Trigger").visible = false;
        else
            series("Trigger").visible = true;
    }

    function triggerVisible2(enabled){
        if (enabled)
            series("Trigger Channel 2").visible = false;
        else
            series("Trigger Channel 2").visible = true;
    }

    function signal2Visible(enabled){
        if (enabled)
            series("Channel 2").visible = false;
        else
            series("Channel 2").visible = true;
   }
    function xAxisChanged(xAxisRange){

        dummy.max = xAxisRange;

    }
    function updateTimeAxis(){
        axisX.max = dataSource.timeElapsed();
//        axisX.min = dataSource.timeElapsed() - dummy.max;
    }

}
