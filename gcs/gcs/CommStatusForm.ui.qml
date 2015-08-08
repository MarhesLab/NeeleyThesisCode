import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: commStatusArea
    width: 135
    height: 180

    property alias gcsConnText: gcsConnStatus.text
    property alias gcsConnColor: gcsConn.color
    property alias apConnText: apConnStatus.text
    property alias apConnColor: apConn.color
    property alias viconConnText: viconConnStatus.text
    property alias viconConnColor: viconConn.color
    property alias gcsConnectButton: gcsConnectButton

    Rectangle {
        id: gcsConn
        x: 5
        y: 10
        height: 23
        color: "#ee0000"
        radius: 6
        anchors.top: parent.top
        anchors.topMargin: 5
        border.width: 0
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5

        Text {
            id: gcsConnLabel
            text: qsTr("Quadrotor <-> GCS")
            anchors.top: parent.top
            anchors.topMargin: 25
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
        }

        Text {
            id: gcsConnStatus
            text: qsTr("Disconnected")
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
        }
        }

    Rectangle {
        id: apConn
        x: 5
        y: 55
        height: 23
        color: "#ee0000"
        radius: 6
        anchors.top: parent.top
        anchors.topMargin: 50
        border.width: 0
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.rightMargin: 5
        Text {
            id: apConnLabel
            text: qsTr("Quadrotor <-> Autopilot")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 25
            anchors.top: parent.top
            font.pixelSize: 12
        }

        Text {
            id: apConnStatus
            text: qsTr("Disconnected")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12
        }
        anchors.leftMargin: 5
    }

    Rectangle {
        id: viconConn
        x: 5
        y: 100
        width: 125
        height: 23
        color: "#ee0000"
        radius: 6
        anchors.top: parent.top
        anchors.topMargin: 95
        border.width: 0
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.rightMargin: 5
        Text {
            id: viconConnLabel
            text: qsTr("Quadrotor <-> Vicon")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 25
            anchors.top: parent.top
            font.pixelSize: 12
        }

        Text {
            id: viconConnStatus
            text: qsTr("Disconnected")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12
        }
        anchors.leftMargin: 5
    }

    Button {
        id: gcsConnectButton
        width: 125
        text: qsTr("Start GCS Comm.")
        anchors.horizontalCenterOffset: 0
        anchors.top: parent.top
        anchors.topMargin: 140
        anchors.horizontalCenter: parent.horizontalCenter
    }
}

