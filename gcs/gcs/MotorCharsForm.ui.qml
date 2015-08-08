import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: mainItem
    width: 135
    height: 310

    property alias dmcColor:        dmcmodebar.color
    property alias dmcText:         dmcmodetext.text
    property alias motor1:          motorval1.text
    property alias motor2:          motorval2.text
    property alias motor3:          motorval3.text
    property alias motor4:          motorval4.text
    property alias motorChar1:      dmcval1.text
    property alias motorChar2:      dmcval2.text
    property alias motorChar3:      dmcval3.text
    property alias motorChar4:      dmcval4.text
    property alias motorBox1:       dmcval1
    property alias motorBox2:       dmcval2
    property alias motorBox3:       dmcval3
    property alias motorBox4:       dmcval4
    property alias motorCharIn1:    dmcspinbox1
    property alias motorCharIn2:    dmcspinbox2
    property alias motorCharIn3:    dmcspinbox3
    property alias motorCharIn4:    dmcspinbox4


    Image {
        id: motorMap
        height: 135
        antialiasing: true
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        source: "pictures/pic_motorlabelsbig.png"
    }

    Rectangle {
        id: dmcmodebar
        height: 23
        color: "#999999"
        radius: 6
        border.width: 0
        anchors.top: motorMap.bottom
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5

        Text {
            id: dmcmodetext
            text: qsTr("DMC Mode Off")
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
        }
    }

    Text {
        id: rpmlabel
        x: 41
        y: 166
        text: qsTr("RPM")
        anchors.bottom: motorval1.top
        anchors.bottomMargin: 5
        anchors.horizontalCenter: motorval1.horizontalCenter
        font.pixelSize: 12
    }

    Text {
        id: dmclabel
        x: 44
        y: 172
        text: qsTr("DMC")
        anchors.horizontalCenterOffset: 60
        anchors.horizontalCenter: rpmlabel.horizontalCenter
        font.pixelSize: 12
        anchors.bottom: motorval1.top
        anchors.bottomMargin: 5
    }

    Text {
        id: motorval1
        x: 25
        y: 280
        height: 20
        text: qsTr("1200")
        horizontalAlignment: Text.AlignHCenter
        anchors.right: parent.right
        anchors.rightMargin: 70
        verticalAlignment: Text.AlignVCenter
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.top: parent.top
        anchors.topMargin: 190
        font.pixelSize: 12

        Rectangle {
            id: motorvalbox1
            color: "#ffffff"
            radius: 5
            border.color: "#949494"
            border.width: 1
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

        SpinBox {
            id: dmcspinbox1
            y: 1
            width: 55
            visible: false
            anchors.left: parent.right
            anchors.leftMargin: 10
            maximumValue: 200
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: motorlabel1
            x: -20
            y: 0
            height: 20
            text: qsTr("1")
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: -12
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 12
        }

        Text {
            id: dmcval1
            x: 0
            y: 0
            text: qsTr("0")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            font.pixelSize: 12
            anchors.right: parent.right
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: 0
            anchors.top: parent.top
            Rectangle {
                id: dmcvalbox1
                color: "#ffffff"
                radius: 5
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                border.width: 1
                anchors.top: parent.top
                border.color: "#949494"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: -2
                anchors.bottomMargin: -2
            }
            anchors.rightMargin: -65
            anchors.left: parent.left
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: 60
        }
    }

    Text {
        id: motorval2
        x: 25
        y: 340
        height: 20
        text: qsTr("1200")
        anchors.left: motorval1.left
        anchors.topMargin: 30
        anchors.top: motorval1.top
        horizontalAlignment: Text.AlignHCenter
        anchors.leftMargin: 0
        font.pixelSize: 12
        anchors.rightMargin: 0
        verticalAlignment: Text.AlignVCenter
        anchors.right: motorval1.right

        Rectangle {
            id: motorvalbox2
            color: "#ffffff"
            radius: 5
            anchors.topMargin: -2
            anchors.left: parent.left
            anchors.leftMargin: -2
            anchors.right: parent.right
            z: -1
            anchors.top: parent.top
            anchors.rightMargin: -2
            border.width: 1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -2
            border.color: "#949494"
        }

        Text {
            id: motorlabel2
            x: -20
            y: 0
            height: 20
            text: qsTr("2")
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: -12
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
        }

        SpinBox {
            id: dmcspinbox2
            y: 1
            width: 55
            visible: false
            anchors.left: parent.right
            anchors.leftMargin: 10
            maximumValue: 200
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: dmcval2
            x: 0
            y: 0
            text: qsTr("0")
            font.pixelSize: 12
            anchors.right: parent.right
            anchors.topMargin: 0
            horizontalAlignment: Text.AlignHCenter
            anchors.top: parent.top
            Rectangle {
                id: dmcvalbox2
                color: "#ffffff"
                radius: 5
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                border.width: 1
                anchors.top: parent.top
                border.color: "#949494"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.bottomMargin: -2
                anchors.leftMargin: -2
            }
            anchors.rightMargin: -65
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 60
            verticalAlignment: Text.AlignVCenter
            anchors.bottomMargin: 0
        }
    }

    Text {
        id: motorval3
        x: 25
        y: 400
        height: 20
        text: qsTr("1200")
        anchors.left: motorval1.left
        anchors.topMargin: 60
        anchors.top: motorval1.top
        horizontalAlignment: Text.AlignHCenter
        anchors.leftMargin: 0
        font.pixelSize: 12
        anchors.rightMargin: 0
        verticalAlignment: Text.AlignVCenter
        anchors.right: motorval1.right

        Rectangle {
            id: motorvalbox3
            color: "#ffffff"
            radius: 5
            anchors.topMargin: -2
            anchors.left: parent.left
            anchors.leftMargin: -2
            anchors.right: parent.right
            z: -1
            anchors.top: parent.top
            anchors.rightMargin: -2
            border.width: 1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -2
            border.color: "#949494"
        }

        Text {
            id: motorlabel3
            x: -45
            y: -82
            height: 20
            text: qsTr("3")
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: -12
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
        }

        SpinBox {
            id: dmcspinbox3
            y: 1
            width: 55
            visible: false
            anchors.left: parent.right
            anchors.leftMargin: 10
            maximumValue: 200
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: dmcval3
            x: 0
            y: 0
            text: qsTr("0")
            font.pixelSize: 12
            anchors.right: parent.right
            anchors.topMargin: 0
            horizontalAlignment: Text.AlignHCenter
            anchors.top: parent.top
            Rectangle {
                id: dmcvalbox3
                color: "#ffffff"
                radius: 5
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                border.width: 1
                anchors.top: parent.top
                border.color: "#949494"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.bottomMargin: -2
                anchors.leftMargin: -2
            }
            anchors.rightMargin: -65
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 60
            verticalAlignment: Text.AlignVCenter
            anchors.bottomMargin: 0
        }
    }

    Text {
        id: motorval4
        x: 25
        y: 460
        height: 20
        text: qsTr("1200")
        anchors.left: motorval1.left
        anchors.topMargin: 90
        anchors.top: motorval1.top
        horizontalAlignment: Text.AlignHCenter
        anchors.leftMargin: 0
        font.pixelSize: 12
        anchors.rightMargin: 0
        verticalAlignment: Text.AlignVCenter
        anchors.right: motorval1.right

        Rectangle {
            id: motorvalbox4
            color: "#ffffff"
            radius: 5
            anchors.topMargin: -2
            anchors.left: parent.left
            anchors.leftMargin: -2
            anchors.right: parent.right
            z: -1
            anchors.top: parent.top
            anchors.rightMargin: -2
            border.width: 1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -2
            border.color: "#949494"
        }

        Text {
            id: motorlabel4
            x: -45
            y: -112
            height: 20
            text: qsTr("4")
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: -12
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
        }

        SpinBox {
            id: dmcspinbox4
            y: 1
            width: 55
            visible: false
            anchors.left: parent.right
            anchors.leftMargin: 10
            maximumValue: 200
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: dmcval4
            x: 0
            y: 0
            text: qsTr("0")
            font.pixelSize: 12
            anchors.right: parent.right
            anchors.topMargin: 0
            horizontalAlignment: Text.AlignHCenter
            anchors.top: parent.top
            Rectangle {
                id: dmcvalbox4
                color: "#ffffff"
                radius: 5
                z: -1
                anchors.right: parent.right
                anchors.topMargin: -2
                border.width: 1
                anchors.top: parent.top
                border.color: "#949494"
                anchors.rightMargin: -2
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.bottomMargin: -2
                anchors.leftMargin: -2
            }
            anchors.rightMargin: -65
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 60
            verticalAlignment: Text.AlignVCenter
            anchors.bottomMargin: 0
        }
    }



}

