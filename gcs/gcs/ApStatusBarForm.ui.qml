import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: apStatBarMain
    width: 1140
    height: 33

    // Color definitions
    property alias attCtrlColor:    attCtrlBox.color
    property alias altCtrlColor:    altCtrlBox.color
    property alias posCtrlColor:    posCtrlBox.color
    property alias compFailColor:   compFailBox.color
    property alias emerModeColor:   emerModeBox.color
    property alias calibErrColor:   calibErrBox.color
    property alias gyroCalErrColor: gyroCalErrBox.color
    property alias accCalErrColor:  accCalErrBox.color
    property alias magStrErrColor:  magStrErrBox.color
    property alias magIncErrColor:  magIncErrBox.color


    // Text definitions
    property alias attCtrlText:     attCtrlText.text
    property alias altCtrlText:     altCtrlText.text
    property alias posCtrlText:     posCtrlText.text
    property alias compFailText:    compFailText.text
    property alias emerModeText:    emerModeText.text
    property alias calibErrText:    calibErrText.text
    property alias gyroCalErrText:  gyroCalErrText.text
    property alias accCalErrText:   accCalErrText.text
    property alias magStrErrText:   magStrErrText.text
    property alias magIncErrText:   magIncErrText.text


    Rectangle {
        id: attCtrlBox
        width: 95
        height: 23
        color: "#ff0000"
        radius: 6
        border.width: 0
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter

        Text {
            id: attCtrlText
            text: qsTr("Att. Ctrl. Off")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12
        }
    }

    Rectangle {
        id: altCtrlBox
        x: -2
        y: 9
        width: 95
        height: 23
        color: "#ff0000"
        radius: 6
        anchors.leftMargin: 105
        Text {
            id: altCtrlText
            text: qsTr("Alt. Ctrl. Off")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        border.width: 0
    }

    Rectangle {
        id: posCtrlBox
        x: -3
        y: 15
        width: 95
        height: 23
        color: "#ff0000"
        radius: 6
        anchors.leftMargin: 205
        Text {
            id: posCtrlText
            text: qsTr("Pos. Ctrl. Off")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        border.width: 0
    }

    Rectangle {
        id: compFailBox
        x: 4
        y: 24
        width: 95
        height: 23
        color: "#00ff00"
        radius: 6
        anchors.leftMargin: 305
        Text {
            id: compFailText
            text: qsTr("Compass Good")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        border.width: 0
    }

    Rectangle {
        id: emerModeBox
        x: -2
        y: 29
        width: 95
        height: 23
        color: "#00ff00"
        radius: 6
        anchors.leftMargin: 405
        Text {
            id: emerModeText
            text: qsTr("Normal Mode")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        border.width: 0
    }

Text {
    id: titleText
    x: 605
    y: 5
    width: 153
    height: 33
    text: qsTr("Autopilot Status")
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    wrapMode: Text.WordWrap
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    font.pixelSize: 15
}

Rectangle {
    id: calibErrBox
        x: -9
        y: 23
        width: 95
        height: 23
        color: "#00ff00"
        radius: 6
        anchors.right: parent.right
        anchors.rightMargin: 405
        Text {
            id: calibErrText
            text: qsTr("Calibration Good")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }
        anchors.verticalCenter: parent.verticalCenter
        border.width: 0
    }

    Rectangle {
        id: gyroCalErrBox
        x: -14
        y: 19
        width: 95
        height: 23
        color: "#00ff00"
        radius: 6
        anchors.right: parent.right
        anchors.rightMargin: 305
        Text {
            id: gyroCalErrText
            text: qsTr("Gyro Cal. Good")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }
        anchors.verticalCenter: parent.verticalCenter
        border.width: 0
    }

    Rectangle {
        id: accCalErrBox
        x: -23
        y: 22
        width: 95
        height: 23
        color: "#00ff00"
        radius: 6
        anchors.right: parent.right
        anchors.rightMargin: 205
        Text {
            id: accCalErrText
            text: qsTr("Acc. Cal. Good")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }
        anchors.verticalCenter: parent.verticalCenter
        border.width: 0
    }

    Rectangle {
        id: magStrErrBox
        x: -21
        y: 27
        width: 95
        height: 23
        color: "#00ff00"
        radius: 6
        anchors.right: parent.right
        anchors.rightMargin: 105
        Text {
            id: magStrErrText
            text: qsTr("Mag. Str. Good")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }
        anchors.verticalCenter: parent.verticalCenter
        border.width: 0
    }

    Rectangle {
        id: magIncErrBox
        x: -18
        y: 20
        width: 95
        height: 23
        color: "#00ff00"
        radius: 6
        anchors.right: parent.right
        anchors.rightMargin: 5
        Text {
            id: magIncErrText
            text: qsTr("Mag. Str. Good")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }
        anchors.verticalCenter: parent.verticalCenter
        border.width: 0
    }
}

