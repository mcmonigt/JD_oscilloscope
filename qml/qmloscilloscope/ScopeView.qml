/********************************************
 * Project: JD_oscilloscope
 * File Name: datasource.h
 * Modified  and Adapted by Tomas McMonigal
 * Original files provided by The Qt Company Ltd. under the terms
 * of the GNU Free Documentation License Version 1.3 published by the
 * Free Software Foundation.
 * Date Modified: 5/31/19
 * Description: qml file that controls the functionality of the elements of the graph.
 ********************************************/

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

            var series3 = chartView.createSeries(chartView.SeriesTypeLine, "Trigger",
                                                axisX, axisY1);
            series3.useOpenGL = charView.openGl


        } else {
            var series1 = chartView.createSeries(ChartView.SeriesTypeScatter, "Channel 1",
                                                 axisX, axisY1);
            series1.markerSize = 3;
            series1.borderColor = "transparent";
            series1.useOpenGL = chartView.openGL

            var series2 = chartView.createSeries(ChartView.SeriesTypeScatter, "Channel 2",
                                                 axisX2, axisY2);
            series2.markerSize = 3;
            series2.borderColor = "transparent";
            series2.useOpenGL = chartView.openGL

            var series3 = chartView.createSeries(chartView.SeriesTypeLine, "Trigger",
                                                axisX, axisY1);
            series3.useOpenGL = charView.openGl
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

}
