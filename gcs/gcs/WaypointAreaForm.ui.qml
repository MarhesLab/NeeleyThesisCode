import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: item1
    width: 780
    height: 390


    MouseArea {
        id: waypointArea
        width: 510
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter

        Image {
            id: background
            anchors.fill: parent
            source: "pictures/pic_background.png"
        }
    }

    Rectangle {
        id: altitudeBox
        width: 120
        color: "#ffffff"
        radius: 6
        border.color: "#999999"
        border.width: 1
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 3
        anchors.top: parent.top
        anchors.topMargin: 5

        Slider {
            id: altitudeSlider
            stepSize: 0.25
            updateValueWhileDragging: true
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.top: parent.top
            anchors.topMargin: 62
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 48
            antialiasing: true
            value: 0.75
            maximumValue: 2
            minimumValue: 0.25
            tickmarksEnabled: true
            orientation: Qt.Vertical

            Label {
                id: altitudeRef
                x: -21
                text: qsTr("Ref.")
                anchors.top: parent.top
                anchors.topMargin: -23
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Text {
            id: altitudeTitle
            text: qsTr("Altitude")
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 15
        }

        ProgressBar {
            id: altitudeBar
            x: 84
            height: 250
            value: 0.75
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 65
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 14
            maximumValue: 2
            orientation: 90

            Label {
                id: altitudeAct
                x: 0
                text: qsTr("Act.")
                anchors.top: parent.top
                anchors.topMargin: -26
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Column {
            id: altitudeLabels
            width: 37
            anchors.horizontalCenterOffset: 1
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 22
            anchors.top: parent.top
            anchors.topMargin: 61
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5

            Label {
                id: label200m
                text: qsTr("2.00 m")
                font.pointSize: 9
            }

            Label {
                id: label175m
                text: qsTr("1.75 m")
                font.pointSize: 9
            }

            Label {
                id: label150m
                text: qsTr("1.50 m")
                font.pointSize: 9
            }

            Label {
                id: label125m
                text: qsTr("1.25 m")
                font.pointSize: 9
            }

            Label {
                id: label100m
                text: qsTr("1.00 m")
                font.pointSize: 9
            }

            Label {
                id: label075m
                text: qsTr("0.75 m")
                font.pointSize: 9
            }

            Label {
                id: label050m
                text: qsTr("0.50 m")
                font.pointSize: 9
            }

            Label {
                id: label025m
                text: qsTr("0.25 m")
                font.pointSize: 9
            }

            Label {
                id: label000m
                text: qsTr("0.00 m")
                font.pointSize: 9
            }
        }
    }

    Rectangle {
        id: headingBox
        x: -1
        y: 2
        width: 120
        color: "#ffffff"
        radius: 6
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        border.width: 1
        anchors.bottomMargin: 3
        Slider {
            id: headingSlider
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            stepSize: 45
            maximumValue: 180
            orientation: Qt.Vertical
            value: 0
            minimumValue: -180
            anchors.top: parent.top
            tickmarksEnabled: true
            anchors.bottomMargin: 17
            Label {
                id: headingRef
                x: -21
                text: qsTr("Ref.")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: -23
            }
            antialiasing: true
            anchors.topMargin: 62
        }

        Text {
            id: headingTitle
            text: qsTr("Heading")
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 15
            anchors.top: parent.top
            anchors.topMargin: 10
        }

        ProgressBar {
            id: headingBar
            x: 84
            minimumValue: -180
            anchors.bottom: parent.bottom
            maximumValue: 180
            orientation: 90
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.bottomMargin: 20
            Label {
                id: headingAct
                x: 0
                text: qsTr("Act.")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: -27
            }
            anchors.topMargin: 66
        }

        Column {
            id: headingLabels
            width: 37
            anchors.horizontalCenterOffset: 2
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 22
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.bottomMargin: 5
            Label {
                id: label180d
                text: qsTr("+180°")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 9
            }

            Label {
                id: label135d
                text: qsTr("+135°")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 9
            }

            Label {
                id: label90d
                text: qsTr("+90°")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 9
            }

            Label {
                id: label45d
                text: qsTr("+45°")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 9
            }

            Label {
                id: label0d
                text: qsTr("0°")
                anchors.horizontalCenter: parent.horizontalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 9
            }

            Label {
                id: labeln45d
                text: qsTr("-45°")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 9
            }

            Label {
                id: labeln90d
                text: qsTr("-90°")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 9
            }

            Label {
                id: labeln135d
                text: qsTr("-135°")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 9
            }

            Label {
                id: labeln180d
                text: qsTr("-180°")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 9
            }
            anchors.topMargin: 61
        }
        anchors.topMargin: 5
        border.color: "#999999"
    }
}

