import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: statusItem
    width: 135
    height: 230

    property alias batteryVoltage: batteryVoltage.text
    property alias batteryLevel: batteryBar.value
    property alias cpuPercentage: cpuBar.value
    property alias cpuPercentageText: cpuLoad.text
    property alias serialText: serialStatus.text
    property alias serialColor: serialEnabled.color
    property alias currentMode: currentModeColor.text
    property alias currentModeColor: currentMode.color
    property alias desiredMode: desiredModeColor.text
    property alias desiredModeColor: desiredMode.color

    ColumnLayout {
        id: columnLayout
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        ProgressBar {
            id: batteryBar
            minimumValue: 9
            maximumValue: 12.4
            value: 10
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5

            Text {
                id: batteryVoltage
                text: qsTr("00.0 V")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            Text {
                id: batteryText
                text: qsTr("Quadrotor Battery")
                anchors.top: parent.top
                anchors.topMargin: 25
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 12
            }
        }

        ProgressBar {
            id: cpuBar
            value: 100
            maximumValue: 100
            Text {
                id: cpuLoad
                text: qsTr("100%")
                font.pixelSize: 12
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                id: cpuText
                text: qsTr("Autopilot CPU Load")
                anchors.topMargin: 25
                anchors.top: parent.top
                font.pixelSize: 12
                anchors.horizontalCenter: parent.horizontalCenter
            }
            anchors.left: parent.left
            anchors.topMargin: 50
            anchors.top: parent.top
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            anchors.right: parent.right
        }

        Rectangle {
            id: serialEnabled
            color: "#FF0000"
            radius: 6
            anchors.bottom: parent.top
            anchors.bottomMargin: -118
            border.width: 0
            anchors.top: parent.top
            anchors.topMargin: 95
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5

            Text {
                id: serialStatus
                width: 49
                height: 15
                text: qsTr("Disabled")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            Text {
                id: serialText
                text: qsTr("Serial Interface Status")
                anchors.top: parent.top
                anchors.topMargin: 25
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 12
            }

        }

        Rectangle {
            id: currentMode
            color: "#999999"
            radius: 6
            Text {
                id: currentModeColor
                width: 49
                height: 15
                text: qsTr("Off")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                id: currentModeText
                text: qsTr("Current Flight Mode")
                anchors.topMargin: 25
                anchors.top: parent.top
                font.pixelSize: 12
                anchors.horizontalCenter: parent.horizontalCenter
            }
            anchors.left: parent.left
            anchors.topMargin: 140
            anchors.bottom: parent.top
            anchors.top: parent.top
            anchors.leftMargin: 5
            anchors.bottomMargin: -163
            anchors.rightMargin: 5
            border.width: 0
            anchors.right: parent.right
        }

        Rectangle {
            id: desiredMode
            color: "#999999"
            radius: 6
            anchors.bottomMargin: -208
            border.width: 0
            anchors.right: parent.right
            anchors.topMargin: 185
            anchors.left: parent.left
            anchors.bottom: parent.top
            anchors.rightMargin: 5
            Text {
                id: desiredModeColor
                width: 49
                height: 15
                text: qsTr("Off")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                id: desiredModeText
                text: qsTr("Desired Flight Mode")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 25
                anchors.top: parent.top
                font.pixelSize: 12
            }
            anchors.top: parent.top
            anchors.leftMargin: 5
        }
    }
}
