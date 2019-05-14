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

    Text {
        text: "Controls"
        font.pointSize: 18
        color: "white"
    }

    MultiButton {
        id: signal1Button
        text: "Signal 1: "
        items: ["on", "off"]
        enabled: true
        currentSelection: 0
        // antialiasingEnabled needs to be changed to signal1Enabled
        onSelectionChanged: antialiasingEnabled(currentSelection == 1);
    }


    MultiButton {
        id: signal2Button
        text: "Signal 2: "
        items: ["on", "off"]
        currentSelection: 0
        // openGlChanged needs to be changed to signal2Enabled
        onSelectionChanged: openGlChanged(currentSelection == 1);
    }

    MultiButton {
        text: "Graph: "
        items: ["line", "scatter"]
        currentSelection: 0
        onSelectionChanged: seriesTypeChanged(items[currentSelection]);
    }

    MultiButton {
        id: sampleCountButton
        text: "Time Elapsed (ms): "
//        items: ["6", "128", "1024", "10000"]
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
        items: ["5", "10", "1"]
        currentSelection: 0
        onSelectionChanged: voltageScaleChanged1(
                                voltageScaleButton1.items[voltageScaleButton1.currentSelection],
                                5,
                                selection);
    }

    MultiButton {
        id: voltageScaleButton2
        text: "Voltage Signal 2: "
        items: ["5", "10", "1"]
        currentSelection: 0
        onSelectionChanged: voltageScaleChanged2(
                                voltageScaleButton2.items[voltageScaleButton2.currentSelection],
                                5,
                                selection);
    }

}
