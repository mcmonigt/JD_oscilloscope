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

//![1]
Item {
//    signal callTimeScaleChange(int value)

    Connections {
        target: dataSource

        onSignalTimeScale1_10000: {
            scopeView.axisX().max = 5000;
            scopeView.axisX().min = -5000;
        }
        onSignalTimeScale1_1000 : {
            scopeView.axisX().max = 500;
            scopeView.axisX().min = -500;
        }
        onSignalTimeScale1_100 : {
            scopeView.axisX().max = 50;
            scopeView.axisX().min = -50;
        }
        onSignalTimeScale1_10 : {
            scopeView.axisX().max = 5;
            scopeView.axisX().min = -5;
        }

        onSignalTimeScale2_10000: {
            scopeView.changeAxisX2(5000);
//            scopeView.axisX2().min = - 5000;
        }
        onSignalTimeScale2_1000: {
            scopeView.changeAxisX2(500);
//            scopeView.axisX2().max = 500;
//            scopeView.axisX2().min = - 500;
        }
        onSignalTimeScale2_100: {
            scopeView.changeAxisX2(50);
//            scopeView.axisX2().max = 50;
//            scopeView.axisX2().min = - 50;
        }
        onSignalTimeScale2_10: {
            scopeView.changeAxisX2(5);
//            scopeView.axisX2().max = 5;
//            scopeView.axisX2().min = - 5;
        }

        onSignalVoltageScale1_1: scopeView.changeVoltageScale1(1);
        onSignalVoltageScale1_2: scopeView.changeVoltageScale1(2);
        onSignalVoltageScale1_3: scopeView.changeVoltageScale1(3);
        onSignalVoltageScale1_4: scopeView.changeVoltageScale1(4);
        onSignalVoltageScale1_5: scopeView.changeVoltageScale1(5);

        onSignalVoltageScale2_1: scopeView.changeVoltageScale2(1);
        onSignalVoltageScale2_2: scopeView.changeVoltageScale2(2);
        onSignalVoltageScale2_3: scopeView.changeVoltageScale2(3);
        onSignalVoltageScale2_4: scopeView.changeVoltageScale2(4);
        onSignalVoltageScale2_5: scopeView.changeVoltageScale2(5);
    }


    function modifyTimeScale(value){
        console.log("main.qml: modifyTimeScale")
        scopeView.axisX.max = 50;
        scopeView.axisX.min = -50;
        dataSource.changeTimeScale(value);
//        callTimeScaleChange(value);
        return "main.qml: changed time scale"
    }
    function myQmlFunction(msg) {
          console.log("Got message:", msg)
          return "some return value"
      }

    id: main
    width: 600
    height: 400

    ControlPanel {
        id: controlPanel
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
//![1]

        function changeTimeScale(newValue){
            dataSource.changeTimeScale(newValue);
            scopeView.axisX().max = newValue/2;
            scopeView.axisX().min = -newValue/2;
        }

        onSignalTimeScaleChanged: {
            console.log("onSignalTimeScale");
            console.log(scopeView.axisX().max);
            dataSource.changeTimeScale(sampleCount);
            scopeView.axisX().max = sampleCount/2;
            scopeView.axisX().min = -sampleCount/2;
        }


        onSeriesTypeChanged: scopeView.changeSeriesType(type);
        onRefreshRateChanged: {
            scopeView.changeRefreshRate(rate);
//            scopeView.changeVoltageScale(rate);
         }
        onVoltageScaleChanged1: {
            scopeView.changeVoltageScale1(newBoundary);
            dataSource.setVoltageScale1(newBoundary);
        }

        onVoltageScaleChanged2: {
            scopeView.changeVoltageScale2(newBoundary);
            dataSource.setVoltageScale2(newBoundary);
        }

        onTriggerButtonChanged: scopeView.triggerVisible(enabled)
        onTriggerButtonChanged2: scopeView.triggerVisible2(enabled)

        // onAntialiasingEnabled name needs to be changed to onSingal1Enabled
        onAntialiasingEnabled: scopeView.signal1Visible(enabled);
            //scopeView.antialiasing = enabled;
        // onOpenGlChanged needs to be changed to onSignal2Enabled
        onOpenGlChanged: {
            scopeView.signal2Visible(enabled);
            //scopeView.openGL = enabled;
        }
        onTimeAxisChanged:
//            console.log("changing time scale");
            scopeView.axisX().max = sampleCount;

//            scopeView.xAxisChanged(xAxisRange);
    }

//![2]
    ScopeView {
        id: scopeView
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: controlPanel.right
        height: main.height
        property string property0: "none.none"


        onOpenGLSupportedChanged: {
            if (!openGLSupported) {
                controlPanel.openGLButton.enabled = false
                controlPanel.openGLButton.currentSelection = 0
            }
        }
    }
//![2]

}
