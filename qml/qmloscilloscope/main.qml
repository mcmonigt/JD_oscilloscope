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

        onSignalTimeScaleChanged: {
            dataSource.changeTimeScale(sampleCount);
            scopeView.axisX().max = sampleCount/2;
            scopeView.axisX().min = -sampleCount/2;
        }

        onSeriesTypeChanged: scopeView.changeSeriesType(type);
        onRefreshRateChanged: {
            scopeView.changeRefreshRate(rate);
            scopeView.changeVoltageScale(rate);
         }
        onVoltageScaleChanged1: scopeView.changeVoltageScale1(newBoundary);
        onVoltageScaleChanged2: scopeView.changeVoltageScale2(newBoundary);
        // onAntialiasingEnabled name needs to be changed to onSingal1Enabled
        onAntialiasingEnabled: scopeView.signal1Visible(enabled);
            //scopeView.antialiasing = enabled;
        // onOpenGlChanged needs to be changed to onSignal2Enabled
        onOpenGlChanged: {
            scopeView.signal2Visible(enabled);
            //scopeView.openGL = enabled;
        }
        onTimeAxisChanged:
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
