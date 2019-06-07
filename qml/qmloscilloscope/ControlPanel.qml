/********************************************
 * Project: JD_oscilloscope
 * File Name: datasource.h
 * Modified  and Adapted by Tomas McMonigal
 * Original files provided by The Qt Company Ltd. under the terms
 * of the GNU Free Documentation License Version 1.3 published by the
 * Free Software Foundation.
 * Date Modified: 5/31/19
 * Description: Control Pannel for all the Buttons on the GUI
 * and the timer.
 ********************************************/

import QtQuick 2.1
import QtQuick.Layouts 1.0

ColumnLayout {
    property alias signal2Button: signal2Button
    property alias signal1Button: signal1Button
    spacing: 8
    Layout.fillHeight: true
    signal animationsEnabled(bool enabled)
    signal seriesTypeChanged(string type)
    signal refreshRateChanged(variant rate)
    signal signalTimeScaleChanged(int sampleCount)
    signal voltageScaleChanged1(int newBoundary)
    signal voltageScaleChanged2(int newBoundary)
    signal antialiasingEnabled(bool enabled)
    signal openGlChanged(bool enabled)
    signal timeAxisChanged(int xAxisRange);
    signal triggerButtonChanged(bool enabled)
    signal triggerButtonChanged2(bool enabled)
    signal signalTriggerOnOff();

    Text {
        text: "Controls"
        font.pointSize: 18
        color: "white"
    }

    MultiButton {
        id: signal1Button
        text: "Channel 1: "
        items: ["on", "off"]
        enabled: true
        currentSelection: 0
        // antialiasingEnabled needs to be changed to signal1Enabled
        onSelectionChanged: antialiasingEnabled(currentSelection == 1);
    }



    MultiButton {
        id: signal2Button
        text: "Channel 2: "
        items: ["on", "off"]
        currentSelection: 0
        // openGlChanged needs to be changed to signal2Enabled
        onSelectionChanged: {
            openGlChanged(currentSelection == 1);

        }

    }

    MultiButton {
        id: triggerButton
        text: "Trigger: "
        items: ["on", "off"]
        currentSelection: 0
        onSelectionChanged: {
            signalTriggerOnOff();
            triggerButtonChanged(currentSelection == 1)
        }
    }

//    MultiButton {
//        id: triggerButton2
//        text: "Trigger: "
//        items: ["on", "off"]
//        currentSelection: 0
//        onSelectionChanged: triggerButtonChanged2(currentSelection == 1)
//    }


    MultiButton {
        text: "Graph: "
        items: ["line", "scatter"]
        currentSelection: 0
        onSelectionChanged: seriesTypeChanged(items[currentSelection]);
    }

    MultiButton {
        id: sampleCountButton
        text: "Time Elapsed (ms): "
        items: ["10000", "1000", "100", "10"]
        currentSelection: 0
        // onSelectionChanged: timeAxisChanged(items[currentSelection]);

        onSelectionChanged: signalTimeScaleChanged(selection);
    }

    MultiButton {
        text: "Refresh rate(Hz): "
        items: ["1", "24", "60"]
        currentSelection: 2
        onSelectionChanged: refreshRateChanged(items[currentSelection]);
    }

    MultiButton {
        id: voltageScaleButton1
        text: "Voltage Signal 1: "
        items: ["5", "4", "3", "2", "1"]
        currentSelection: 0
        onSelectionChanged: voltageScaleChanged1(
                                voltageScaleButton1.items[voltageScaleButton1.currentSelection],
                                5,
                                selection);
    }

    MultiButton {
        id: voltageScaleButton2
        text: "Voltage Signal 2: "
        items: ["5", "4", "3", "2", "1"]
        currentSelection: 0
        onSelectionChanged: voltageScaleChanged2(
                                voltageScaleButton2.items[voltageScaleButton2.currentSelection],
                                5,
                                selection);
    }



}
