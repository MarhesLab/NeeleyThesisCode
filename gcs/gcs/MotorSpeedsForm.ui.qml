import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Rectangle {
    width: 95
    height: 120

    property alias speed1:   motor1.text
    property alias speed2:   motor2.text
    property alias speed3:   motor3.text
    property alias speed4:   motor4.text

    Text {
        id: motor1
        y: 9
        width: 60
        height: 20
        text: qsTr("0.000")
        Rectangle {
            id: motor1frame
            color: "#ffffff"
            radius: 6
            anchors.leftMargin: -2
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            border.color: "#a4a3a7"
            anchors.right: parent.right
            anchors.rightMargin: -2
            anchors.left: parent.left
            anchors.bottomMargin: -2
            z: -1
            anchors.topMargin: -2
        }

        Text {
            id: motor1label
            text: qsTr("M1")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenterOffset: -45
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 15
            anchors.bottomMargin: 0
            anchors.topMargin: 0
        }
        anchors.leftMargin: 30
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.left: parent.left
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14
        anchors.topMargin: 5
    }

    Text {
        id: motor2
        x: 7
        y: 9
        width: 60
        height: 20
        text: qsTr("0.000")
        Rectangle {
            id: motor2frame
            color: "#ffffff"
            radius: 6
            anchors.leftMargin: -2
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            border.color: "#a4a3a7"
            anchors.right: parent.right
            anchors.rightMargin: -2
            anchors.left: parent.left
            anchors.bottomMargin: -2
            z: -1
            anchors.topMargin: -2
        }

        Text {
            id: motor2label
            text: qsTr("M2")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenterOffset: -45
            anchors.bottomMargin: 0
            font.pixelSize: 15
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 0
        }
        anchors.leftMargin: 30
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.left: parent.left
        font.pixelSize: 14
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 35
    }

    Text {
        id: motor3
        x: 5
        y: 2
        width: 60
        height: 20
        text: qsTr("0.000")
        Rectangle {
            id: motor3frame
            color: "#ffffff"
            radius: 6
            anchors.leftMargin: -2
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            border.color: "#a4a3a7"
            anchors.right: parent.right
            anchors.rightMargin: -2
            anchors.left: parent.left
            anchors.bottomMargin: -2
            z: -1
            anchors.topMargin: -2
        }

        Text {
            id: motor3label
            text: qsTr("M3")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenterOffset: -45
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 15
            anchors.bottomMargin: 0
            anchors.topMargin: 0
        }
        anchors.leftMargin: 30
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.left: parent.left
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14
        anchors.topMargin: 65
    }

    Text {
        id: motor4
        x: 15
        y: 9
        width: 60
        height: 20
        text: qsTr("0.000")
        Rectangle {
            id: motor4frame
            color: "#ffffff"
            radius: 6
            anchors.leftMargin: -2
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            border.color: "#a4a3a7"
            anchors.right: parent.right
            anchors.rightMargin: -2
            anchors.left: parent.left
            anchors.bottomMargin: -2
            z: -1
            anchors.topMargin: -2
        }

        Text {
            id: motor4label
            text: qsTr("M4")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenterOffset: -45
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 15
            anchors.bottomMargin: 0
            anchors.topMargin: 0
        }
        anchors.leftMargin: 30
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.left: parent.left
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 14
        anchors.topMargin: 95
    }
}
