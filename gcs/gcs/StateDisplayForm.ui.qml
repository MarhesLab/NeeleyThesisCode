import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: item1
    width: 780
    height: 250

    // State Aliases
    property alias stateX:   stateX.text
    property alias stateY:   stateY.text
    property alias stateZ:   stateZ.text
    property alias stateDX:  stateDX.text
    property alias stateDY:  stateDY.text
    property alias stateDZ:  stateDZ.text
    property alias stateQ0:  stateQ0.text
    property alias stateQI:  stateQI.text
    property alias stateQJ:  stateQJ.text
    property alias stateQK:  stateQK.text
    property alias stateWX:  stateWX.text
    property alias stateWY:  stateWY.text
    property alias stateWZ:  stateWZ.text

    // Vicon Aliases
    property alias viconX:   viconX.text
    property alias viconY:   viconY.text
    property alias viconZ:   viconZ.text
    property alias viconQ0:  viconQ0.text
    property alias viconQI:  viconQI.text
    property alias viconQJ:  viconQJ.text
    property alias viconQK:  viconQK.text

    // IMU Aliases
    property alias imuDDX:   imuDDX.text
    property alias imuDDY:   imuDDY.text
    property alias imuDDZ:   imuDDZ.text
    property alias imuWX:    imuWX.text
    property alias imuWY:    imuWY.text
    property alias imuWZ:    imuWZ.text

    // Input Aliases
    property alias inputT:   inputT.text
    property alias inputtx:  inputTx.text
    property alias inputty:  inputTy.text
    property alias inputtz:  inputTz.text

    // Reference Aliases
    property alias refX:     refX.text
    property alias refY:     refY.text
    property alias refZ:     refZ.text
    property alias refDX:    refDX.text
    property alias refDY:    refDY.text
    property alias refDZ:    refDZ.text
    property alias refQ0:    refQ0.text
    property alias refQI:    refQI.text
    property alias refQJ:    refQJ.text
    property alias refQK:    refQK.text
    property alias refWX:    refWX.text
    property alias refWY:    refWY.text
    property alias refWZ:    refWZ.text


    Rectangle {
        id: stateBox
        width: 210
        height: 240
        color: "#ffffff"
        radius: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        border.color: "#a4a3a7"
        border.width: 1

        Text {
            id: stateTitle
            text: qsTr("States")
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 15
        }

        Text {
            id: stateX
            x: 25
            y: 5
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.top: parent.top
            anchors.topMargin: 30
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 14

            Rectangle {
                id: frame1
                color: "#ffffff"
                radius: 6
                border.color: "#a4a3a7"
                z: -1
                anchors.right: parent.right
                anchors.rightMargin: -2
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -2
                anchors.top: parent.top
                anchors.topMargin: -2
            }

            Text {
                id: stateXlabel
                text: qsTr("X")
                anchors.horizontalCenterOffset: -45
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 15
            }
        }


        Text {
            id: stateY
            x: 19
            y: 8
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: stateX.horizontalCenter
            anchors.top: stateX.top
            horizontalAlignment: Text.AlignHCenter
            Rectangle {
                id: frame2
                color: "#ffffff"
                radius: 6
                anchors.leftMargin: -2
                anchors.top: parent.top
                z: -1
                anchors.bottom: parent.bottom
                anchors.rightMargin: -2
                border.color: "#a4a3a7"
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.left: parent.left
                anchors.bottomMargin: -2
            }

            Text {
                id: stateYlabel
                x: -2
                y: 9
                text: qsTr("Y")
                anchors.top: parent.top
                verticalAlignment: Text.AlignVCenter
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 0
                anchors.horizontalCenterOffset: -45
                font.pixelSize: 15
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 30
            font.pixelSize: 14
        }

        Text {
            id: stateZ
            x: 17
            y: 9
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: stateX.horizontalCenter
            anchors.top: stateX.top
            horizontalAlignment: Text.AlignHCenter
            Rectangle {
                id: frame3
                color: "#ffffff"
                radius: 6
                anchors.leftMargin: -2
                anchors.top: parent.top
                z: -1
                anchors.bottom: parent.bottom
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.left: parent.left
                anchors.bottomMargin: -2
            }

            Text {
                id: stateZlabel
                x: 0
                y: -2
                text: qsTr("Z")
                anchors.top: parent.top
                verticalAlignment: Text.AlignVCenter
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 0
                anchors.horizontalCenterOffset: -45
                font.pixelSize: 15
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 60
            font.pixelSize: 14
        }

    Text {
        id: stateDX
        x: 14
        y: 2
        width: 60
        height: 20
        text: qsTr("0.000")
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        horizontalAlignment: Text.AlignHCenter
        Rectangle {
            id: frame4
            color: "#ffffff"
            radius: 6
            anchors.leftMargin: -2
            anchors.top: parent.top
            z: -1
            anchors.bottom: parent.bottom
            anchors.rightMargin: -2
            border.color: "#a4a3a7"
            anchors.right: parent.right
            anchors.topMargin: -2
            anchors.left: parent.left
            anchors.bottomMargin: -2
        }

        Text {
            id: stateDXlabel
            x: 7
            y: 7
            text: qsTr("dX")
            anchors.top: parent.top
            verticalAlignment: Text.AlignVCenter
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 0
            anchors.horizontalCenterOffset: -50
            font.pixelSize: 15
            anchors.bottomMargin: 0
        }
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 30
        font.pixelSize: 14
    }

    Text {
        id: stateDY
        x: 9
        y: 8
        width: 60
        height: 20
        text: qsTr("0.000")
        anchors.horizontalCenter: stateDX.horizontalCenter
        anchors.top: stateDX.top
        horizontalAlignment: Text.AlignHCenter
        Rectangle {
            id: frame5
            color: "#ffffff"
            radius: 6
            anchors.leftMargin: -2
            anchors.top: parent.top
            z: -1
            anchors.bottom: parent.bottom
            anchors.rightMargin: -2
            border.color: "#a4a3a7"
            anchors.right: parent.right
            anchors.topMargin: -2
            anchors.left: parent.left
            anchors.bottomMargin: -2
        }

        Text {
            id: stateDYlabel
            x: -9
            y: 6
            text: qsTr("dY")
            anchors.top: parent.top
            verticalAlignment: Text.AlignVCenter
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 0
            anchors.horizontalCenterOffset: -50
            font.pixelSize: 15
            anchors.bottomMargin: 0
        }
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 30
        font.pixelSize: 14
    }

    Text {
        id: stateDZ
        x: 0
        y: 5
        width: 60
        height: 20
        text: qsTr("0.000")
        anchors.horizontalCenter: stateDX.horizontalCenter
        anchors.top: stateDX.top
        horizontalAlignment: Text.AlignHCenter
        Rectangle {
            id: frame6
            color: "#ffffff"
            radius: 6
            anchors.leftMargin: -2
            anchors.top: parent.top
            z: -1
            anchors.bottom: parent.bottom
            border.color: "#a4a3a7"
            anchors.rightMargin: -2
            anchors.right: parent.right
            anchors.topMargin: -2
            anchors.left: parent.left
            anchors.bottomMargin: -2
        }

        Text {
            id: stateDZlabel
            x: -9
            y: 6
            text: qsTr("dZ")
            anchors.top: parent.top
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            anchors.horizontalCenterOffset: -50
            anchors.bottomMargin: 0
            font.pixelSize: 15
        }
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 60
        font.pixelSize: 14
    }

    Text {
        id: stateQ0
        y: 0
        width: 60
        height: 20
        text: qsTr("0.000")
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        horizontalAlignment: Text.AlignHCenter
        Rectangle {
            id: frame7
            color: "#ffffff"
            radius: 6
            anchors.leftMargin: -2
            anchors.top: parent.top
            z: -1
            anchors.bottom: parent.bottom
            anchors.rightMargin: -2
            border.color: "#a4a3a7"
            anchors.right: parent.right
            anchors.topMargin: -2
            anchors.left: parent.left
            anchors.bottomMargin: -2
        }

        Text {
            id: stateQ0label
            text: qsTr("Q0")
            anchors.top: parent.top
            verticalAlignment: Text.AlignVCenter
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 0
            anchors.horizontalCenterOffset: -45
            font.pixelSize: 15
            anchors.bottomMargin: 0
        }
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 120
        font.pixelSize: 14
    }

    Text {
        id: stateQI
        x: 705
        y: 25
        width: 60
        height: 20
        text: qsTr("0.000")
        anchors.horizontalCenter: stateQ0.horizontalCenter
        font.pixelSize: 14
        horizontalAlignment: Text.AlignHCenter
        anchors.topMargin: 30
        anchors.top: stateQ0.top
        Rectangle {
            id: frame41
            color: "#ffffff"
            radius: 6
            z: -1
            anchors.right: parent.right
            anchors.topMargin: -2
            anchors.top: parent.top
            border.color: "#a4a3a7"
            anchors.rightMargin: -2
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: -2
            anchors.bottomMargin: -2
        }

        Text {
            id: stateQIlabel
            text: qsTr("Qi")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 15
            anchors.horizontalCenterOffset: -45
            anchors.topMargin: 0
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            verticalAlignment: Text.AlignVCenter
            anchors.bottomMargin: 0
        }
        verticalAlignment: Text.AlignVCenter
    }


    Text {
        id: stateQJ
        x: 21
        y: 9
        width: 60
        height: 20
        text: qsTr("0.000")
        anchors.horizontalCenter: stateQ0.horizontalCenter
        anchors.top: stateQ0.top
        horizontalAlignment: Text.AlignHCenter
        Rectangle {
            id: frame8
            color: "#ffffff"
            radius: 6
            anchors.leftMargin: -2
            anchors.top: parent.top
            z: -1
            anchors.bottom: parent.bottom
            border.color: "#a4a3a7"
            anchors.rightMargin: -2
            anchors.right: parent.right
            anchors.topMargin: -2
            anchors.left: parent.left
            anchors.bottomMargin: -2
        }

        Text {
            id: stateQJlabel
            text: qsTr("Qj")
            anchors.top: parent.top
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            anchors.horizontalCenterOffset: -45
            anchors.bottomMargin: 0
            font.pixelSize: 15
        }
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 60
        font.pixelSize: 14
    }

    Text {
        id: stateQK
        x: 26
        y: -4
        width: 60
        height: 20
        text: qsTr("0.000")
        anchors.horizontalCenter: stateQ0.horizontalCenter
        anchors.top: stateQ0.top
        horizontalAlignment: Text.AlignHCenter
        Rectangle {
            id: frame9
            color: "#ffffff"
            radius: 6
            anchors.leftMargin: -2
            anchors.top: parent.top
            z: -1
            anchors.bottom: parent.bottom
            border.color: "#a4a3a7"
            anchors.rightMargin: -2
            anchors.right: parent.right
            anchors.topMargin: -2
            anchors.left: parent.left
            anchors.bottomMargin: -2
        }

        Text {
            id: stateQKlabel
            text: qsTr("Qk")
            anchors.top: parent.top
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            anchors.horizontalCenterOffset: -45
            anchors.bottomMargin: 0
            font.pixelSize: 15
        }
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 90
        font.pixelSize: 14
    }

    Text {
        id: stateWX
        x: 18
        y: 0
        width: 60
        height: 20
        text: qsTr("0.000")
        anchors.top: parent.top
        horizontalAlignment: Text.AlignHCenter
        Rectangle {
            id: frame10
            color: "#ffffff"
            radius: 6
            anchors.leftMargin: -2
            anchors.top: parent.top
            z: -1
            anchors.bottom: parent.bottom
            border.color: "#a4a3a7"
            anchors.rightMargin: -2
            anchors.right: parent.right
            anchors.topMargin: -2
            anchors.left: parent.left
            anchors.bottomMargin: -2
        }

        Text {
            id: stateWXlabel
            text: qsTr("Wx")
            anchors.top: parent.top
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            anchors.horizontalCenterOffset: -50
            anchors.bottomMargin: 0
            font.pixelSize: 15
        }
        verticalAlignment: Text.AlignVCenter
        anchors.rightMargin: 10
        anchors.right: parent.right
        anchors.topMargin: 120
        font.pixelSize: 14
    }

    Text {
        id: stateWY
        x: 23
        y: 9
        width: 60
        height: 20
        text: qsTr("0.000")
        anchors.horizontalCenter: stateWX.horizontalCenter
        anchors.top: stateWX.top
        horizontalAlignment: Text.AlignHCenter
        Rectangle {
            id: frame11
            color: "#ffffff"
            radius: 6
            anchors.leftMargin: -2
            anchors.top: parent.top
            z: -1
            anchors.bottom: parent.bottom
            anchors.rightMargin: -2
            border.color: "#a4a3a7"
            anchors.right: parent.right
            anchors.topMargin: -2
            anchors.left: parent.left
            anchors.bottomMargin: -2
        }

        Text {
            id: stateWYlabel
            text: qsTr("Wy")
            anchors.top: parent.top
            verticalAlignment: Text.AlignVCenter
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 0
            anchors.horizontalCenterOffset: -50
            font.pixelSize: 15
            anchors.bottomMargin: 0
        }
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 30
        font.pixelSize: 14
    }

    Text {
        id: stateWZ
        x: 30
        y: 7
        width: 60
        height: 20
        text: qsTr("0.000")
        anchors.top: stateWX.top
        horizontalAlignment: Text.AlignHCenter
        Rectangle {
            id: frame12
            color: "#ffffff"
            radius: 6
            anchors.leftMargin: -2
            anchors.top: parent.top
            z: -1
            anchors.bottom: parent.bottom
            border.color: "#a4a3a7"
            anchors.rightMargin: -2
            anchors.right: parent.right
            anchors.topMargin: -2
            anchors.left: parent.left
            anchors.bottomMargin: -2
        }

        Text {
            id: stateWZlabel
            text: qsTr("Wz")
            anchors.top: parent.top
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            anchors.horizontalCenterOffset: -50
            anchors.bottomMargin: 0
            font.pixelSize: 15
        }
        verticalAlignment: Text.AlignVCenter
        anchors.rightMargin: 10
        anchors.right: parent.right
        anchors.topMargin: 60
        font.pixelSize: 14
    }

    }

    Rectangle {
        id: viconBox
        x: 7
        y: -7
        width: 100
        height: 240
        color: "#ffffff"
        radius: 5
        anchors.leftMargin: 225
        anchors.top: parent.top
        Text {
            id: viconTitle
            text: qsTr("Vicon")
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 5
            font.pixelSize: 15
        }

        Text {
            id: viconX
            x: 28
            y: 8
            width: 60
            height: 20
            text: qsTr("0.000")
            font.pixelSize: 14
            anchors.topMargin: 30
            horizontalAlignment: Text.AlignHCenter
            anchors.top: parent.top
            Rectangle {
                id: frame42
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: viconXlabel
                text: qsTr("X")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                verticalAlignment: Text.AlignVCenter
            }
            anchors.left: parent.left
            anchors.leftMargin: 30
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: viconY
            x: 22
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: viconX.horizontalCenter
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 30
            anchors.top: viconX.top
            Rectangle {
                id: frame43
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: viconYlabel
                x: -2
                y: 9
                text: qsTr("Y")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: viconZ
            x: 20
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: viconX.horizontalCenter
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 60
            anchors.top: viconX.top
            Rectangle {
                id: frame44
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: viconZlabel
                x: 0
                y: -2
                text: qsTr("Z")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: viconQ0
            y: 0
            width: 60
            height: 20
            text: qsTr("0.000")
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 120
            anchors.top: parent.top
            Rectangle {
                id: frame13
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: viconQ0label
                text: qsTr("Q0")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            anchors.left: parent.left
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: 30
        }

        Text {
            id: viconQI
            x: 705
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: viconQ0.horizontalCenter
            font.pixelSize: 14
            anchors.topMargin: 30
            horizontalAlignment: Text.AlignHCenter
            anchors.top: viconQ0.top
            Rectangle {
                id: frame45
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.bottomMargin: -2
                anchors.leftMargin: -2
            }

            Text {
                id: viconQIlabel
                text: qsTr("Qi")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                verticalAlignment: Text.AlignVCenter
            }
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: viconQJ
            x: 21
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: viconQ0.horizontalCenter
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 60
            anchors.top: viconQ0.top
            Rectangle {
                id: frame14
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: viconQJlabel
                text: qsTr("Qj")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: viconQK
            x: 26
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: viconQ0.horizontalCenter
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 90
            anchors.top: viconQ0.top
            Rectangle {
                id: frame15
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: viconQKlabel
                text: qsTr("Qk")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
        }

        border.width: 1
        border.color: "#a4a3a7"
        anchors.topMargin: 5
        anchors.left: parent.left
    }

    Rectangle {
        id: imuBox
        x: -1
        y: -11
        width: 110
        height: 210
        color: "#ffffff"
        radius: 6
        anchors.leftMargin: 335
        anchors.top: parent.top
        Text {
            id: imuTitle
            text: qsTr("IMU")
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 5
            font.pixelSize: 15
        }

        Text {
            id: imuDDX
            x: 25
            y: 5
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.leftMargin: 40
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            Rectangle {
                id: frame16
                color: "#ffffff"
                radius: 6
                anchors.leftMargin: -2
                anchors.top: parent.top
                z: -1
                anchors.bottom: parent.bottom
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.left: parent.left
                anchors.bottomMargin: -2
            }

            Text {
                id: imuDDXlabel
                text: qsTr("ddX")
                anchors.top: parent.top
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.topMargin: 0
                anchors.horizontalCenterOffset: -50
                anchors.bottomMargin: 0
                font.pixelSize: 15
            }
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 30
            anchors.left: parent.left
            font.pixelSize: 14
        }

        Text {
            id: imuDDY
            x: 19
            y: 8
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.leftMargin: 40
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            Rectangle {
                id: frame17
                color: "#ffffff"
                radius: 6
                anchors.leftMargin: -2
                anchors.top: parent.top
                z: -1
                anchors.bottom: parent.bottom
                anchors.rightMargin: -2
                border.color: "#a4a3a7"
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.left: parent.left
                anchors.bottomMargin: -2
            }

            Text {
                id: imuDDYval
                x: -2
                y: 9
                text: qsTr("ddY")
                anchors.top: parent.top
                verticalAlignment: Text.AlignVCenter
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 0
                anchors.horizontalCenterOffset: -50
                font.pixelSize: 15
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 60
            anchors.left: parent.left
            font.pixelSize: 14
        }

        Text {
            id: imuDDZ
            x: 17
            y: 9
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.leftMargin: 40
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            Rectangle {
                id: frame18
                color: "#ffffff"
                radius: 6
                anchors.leftMargin: -2
                anchors.top: parent.top
                z: -1
                anchors.bottom: parent.bottom
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.left: parent.left
                anchors.bottomMargin: -2
            }

            Text {
                id: imuDDZlabel
                x: 0
                y: -2
                text: qsTr("ddZ")
                anchors.top: parent.top
                verticalAlignment: Text.AlignVCenter
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 0
                anchors.horizontalCenterOffset: -50
                font.pixelSize: 15
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 90
            anchors.left: parent.left
            font.pixelSize: 14
        }

        Text {
            id: imuWX
            y: 0
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.leftMargin: 40
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            Rectangle {
                id: frame22
                color: "#ffffff"
                radius: 6
                anchors.leftMargin: -2
                anchors.top: parent.top
                z: -1
                anchors.bottom: parent.bottom
                anchors.rightMargin: -2
                border.color: "#a4a3a7"
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.left: parent.left
                anchors.bottomMargin: -2
            }

            Text {
                id: imuWXlabel
                text: qsTr("Wx")
                anchors.top: parent.top
                verticalAlignment: Text.AlignVCenter
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 0
                anchors.horizontalCenterOffset: -50
                font.pixelSize: 15
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 120
            anchors.left: parent.left
            font.pixelSize: 14
        }

        Text {
            id: imuWY
            y: 9
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.leftMargin: 40
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            Rectangle {
                id: frame23
                color: "#ffffff"
                radius: 6
                anchors.leftMargin: -2
                anchors.top: parent.top
                z: -1
                anchors.bottom: parent.bottom
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.left: parent.left
                anchors.bottomMargin: -2
            }

            Text {
                id: imuWYlabel
                text: qsTr("Wy")
                anchors.top: parent.top
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.topMargin: 0
                anchors.horizontalCenterOffset: -50
                anchors.bottomMargin: 0
                font.pixelSize: 15
            }
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 150
            anchors.left: parent.left
            font.pixelSize: 14
        }

        Text {
            id: imuWZ
            y: -4
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.leftMargin: 40
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            Rectangle {
                id: frame24
                color: "#ffffff"
                radius: 6
                anchors.leftMargin: -2
                anchors.top: parent.top
                z: -1
                anchors.bottom: parent.bottom
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.left: parent.left
                anchors.bottomMargin: -2
            }

            Text {
                id: imuWZlabel
                text: qsTr("Wz")
                anchors.top: parent.top
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.topMargin: 0
                anchors.horizontalCenterOffset: -50
                anchors.bottomMargin: 0
                font.pixelSize: 15
            }
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 180
            anchors.left: parent.left
            font.pixelSize: 14
        }
        border.width: 1
        border.color: "#a4a3a7"
        anchors.topMargin: 5
        anchors.left: parent.left
    }

    Rectangle {
        id: inputBox
        x: 10
        y: -12
        width: 100
        height: 150
        color: "#ffffff"
        radius: 6
        anchors.leftMargin: 455
        anchors.top: parent.top
        Text {
            id: inputTitle
            text: qsTr("Inputs")
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 5
            font.pixelSize: 15
        }

        Text {
            id: inputT
            x: 25
            y: 5
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.leftMargin: 30
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            Rectangle {
                id: frame25
                color: "#ffffff"
                radius: 6
                anchors.leftMargin: -2
                anchors.top: parent.top
                z: -1
                anchors.bottom: parent.bottom
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.left: parent.left
                anchors.bottomMargin: -2
            }

            Text {
                id: inputTlabel
                text: qsTr("T")
                anchors.top: parent.top
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.topMargin: 0
                anchors.horizontalCenterOffset: -45
                anchors.bottomMargin: 0
                font.pixelSize: 15
            }
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 30
            anchors.left: parent.left
            font.pixelSize: 14
        }

        Text {
            id: inputTx
            y: 0
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.leftMargin: 30
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            Rectangle {
                id: frame28
                color: "#ffffff"
                radius: 6
                anchors.leftMargin: -2
                anchors.top: parent.top
                z: -1
                anchors.bottom: parent.bottom
                anchors.rightMargin: -2
                border.color: "#a4a3a7"
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.left: parent.left
                anchors.bottomMargin: -2
            }

            Text {
                id: inputTxlabel
                text: qsTr("τx")
                anchors.top: parent.top
                verticalAlignment: Text.AlignVCenter
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 0
                anchors.horizontalCenterOffset: -45
                font.pixelSize: 15
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 60
            anchors.left: parent.left
            font.pixelSize: 14
        }

        Text {
            id: inputTy
            y: 9
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.leftMargin: 30
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            Rectangle {
                id: frame29
                color: "#ffffff"
                radius: 6
                anchors.leftMargin: -2
                anchors.top: parent.top
                z: -1
                anchors.bottom: parent.bottom
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.left: parent.left
                anchors.bottomMargin: -2
            }

            Text {
                id: inputTylabel
                text: qsTr("τy")
                anchors.top: parent.top
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.topMargin: 0
                anchors.horizontalCenterOffset: -45
                anchors.bottomMargin: 0
                font.pixelSize: 15
            }
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 90
            anchors.left: parent.left
            font.pixelSize: 14
        }

        Text {
            id: inputTz
            y: -4
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.leftMargin: 30
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            Rectangle {
                id: frame30
                color: "#ffffff"
                radius: 6
                anchors.leftMargin: -2
                anchors.top: parent.top
                z: -1
                anchors.bottom: parent.bottom
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.left: parent.left
                anchors.bottomMargin: -2
            }

            Text {
                id: inputTzlabel
                text: qsTr("τz")
                anchors.top: parent.top
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.topMargin: 0
                anchors.horizontalCenterOffset: -45
                anchors.bottomMargin: 0
                font.pixelSize: 15
            }
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 120
            anchors.left: parent.left
            font.pixelSize: 14
        }
        border.width: 1
        border.color: "#a4a3a7"
        anchors.topMargin: 5
        anchors.left: parent.left
    }

    Rectangle {
        id: refBox
        x: -5
        y: 1
        width: 210
        height: 240
        color: "#ffffff"
        radius: 5
        anchors.topMargin: 5
        border.width: 1
        anchors.top: parent.top
        border.color: "#a4a3a7"
        Text {
            id: refTitle
            text: qsTr("Reference")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 15
            anchors.topMargin: 5
            anchors.top: parent.top
        }

        Text {
            id: refX
            x: 25
            y: 5
            width: 60
            height: 20
            text: qsTr("0.000")
            font.pixelSize: 14
            anchors.topMargin: 30
            horizontalAlignment: Text.AlignHCenter
            anchors.top: parent.top
            Rectangle {
                id: frame19
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: refXlabel
                text: qsTr("X")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                verticalAlignment: Text.AlignVCenter
            }
            anchors.left: parent.left
            anchors.leftMargin: 30
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: refY
            x: 19
            y: 8
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: refX.horizontalCenter
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 30
            anchors.top: refX.top
            Rectangle {
                id: frame20
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: refYlabel
                x: -2
                y: 9
                text: qsTr("Y")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: refZ
            x: 17
            y: 9
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: refX.horizontalCenter
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 60
            anchors.top: refX.top
            Rectangle {
                id: frame21
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: refZlabel
                x: 0
                y: -2
                text: qsTr("Z")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: refDX
            x: 14
            y: 2
            width: 60
            height: 20
            text: qsTr("0.000")
            font.pixelSize: 14
            anchors.right: parent.right
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 30
            anchors.top: parent.top
            Rectangle {
                id: frame46
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: refDXlabel
                x: 7
                y: 7
                text: qsTr("dX")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -50
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            anchors.rightMargin: 10
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: refDY
            x: 9
            y: 8
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: refDX.horizontalCenter
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 30
            anchors.top: refDX.top
            Rectangle {
                id: frame47
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: refDYlabel
                x: -9
                y: 6
                text: qsTr("dY")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -50
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: refDZ
            x: 0
            y: 5
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: refDX.horizontalCenter
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 60
            anchors.top: refDX.top
            Rectangle {
                id: frame48
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: refDZlabel
                x: -9
                y: 6
                text: qsTr("dZ")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -50
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: refQ0
            y: 0
            width: 60
            height: 20
            text: qsTr("0.000")
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 120
            anchors.top: parent.top
            Rectangle {
                id: frame49
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: refQ0label
                text: qsTr("Q0")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            anchors.left: parent.left
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: 30
        }

        Text {
            id: refQI
            x: 705
            y: 25
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: refQ0.horizontalCenter
            font.pixelSize: 14
            anchors.topMargin: 30
            horizontalAlignment: Text.AlignHCenter
            anchors.top: refQ0.top
            Rectangle {
                id: frame50
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.bottomMargin: -2
                anchors.leftMargin: -2
            }

            Text {
                id: refQIlabel
                text: qsTr("Qi")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                verticalAlignment: Text.AlignVCenter
            }
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: refQJ
            x: 21
            y: 9
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: refQ0.horizontalCenter
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 60
            anchors.top: refQ0.top
            Rectangle {
                id: frame51
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: refQJlabel
                text: qsTr("Qj")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: refQK
            x: 26
            y: -4
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: refQ0.horizontalCenter
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 90
            anchors.top: refQ0.top
            Rectangle {
                id: frame52
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: refQKlabel
                text: qsTr("Qk")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -45
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: refWX
            x: 18
            y: 0
            width: 60
            height: 20
            text: qsTr("0.000")
            font.pixelSize: 14
            anchors.right: parent.right
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 120
            anchors.top: parent.top
            Rectangle {
                id: frame53
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: refWXlabel
                text: qsTr("Wx")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -50
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            anchors.rightMargin: 10
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: refWY
            x: 23
            y: 9
            width: 60
            height: 20
            text: qsTr("0.000")
            anchors.horizontalCenter: refWX.horizontalCenter
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 30
            anchors.top: refWX.top
            Rectangle {
                id: frame54
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: refWYlabel
                text: qsTr("Wy")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -50
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: refWZ
            x: 30
            y: 7
            width: 60
            height: 20
            text: qsTr("0.000")
            font.pixelSize: 14
            anchors.right: parent.right
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 60
            anchors.top: refWX.top
            Rectangle {
                id: frame55
                color: "#ffffff"
                radius: 6
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                anchors.top: parent.top
                border.color: "#a4a3a7"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }

            Text {
                id: refWZlabel
                text: qsTr("Wz")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 15
                anchors.horizontalCenterOffset: -50
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
            }
            anchors.rightMargin: 10
            verticalAlignment: Text.AlignVCenter
        }
        anchors.left: parent.left
        anchors.leftMargin: 565
    }
}

