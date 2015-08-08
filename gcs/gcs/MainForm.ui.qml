import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: item1
    width: 135
    height: 330
    transformOrigin: Item.Center

    property alias buttonOff:       buttonOff
    property alias buttonIdle:      buttonIdle
    property alias buttonTakeoff:   buttonTakeoff
    property alias buttonHover:     buttonHover
    property alias buttonWaypoint:  buttonWaypoint
    property alias buttonVelocity:  buttonVelocity
    property alias buttonFigure8:   buttonFigure8
    property alias buttonSpecial:   buttonSpecial
    property alias buttonRef:       buttonRef
    property alias buttonHeadHome:  buttonHeadHome
    property alias buttonLand:      buttonLand

    ColumnLayout {
        id: columnLayout1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        transformOrigin: Item.TopLeft
        spacing: 6

        Button {
            id: buttonOff
            width: 120
            text: "Off/Kill"
            Layout.fillWidth: true
        }

        Button {
            id: buttonIdle
            width: 120
            text: qsTr("Idle")
            Layout.fillWidth: true
        }

        Button {
            id: buttonTakeoff
            width: 120
            text: qsTr("Takeoff")
            Layout.fillWidth: true
        }

        Button {
            id: buttonHover
            width: 120
            text: qsTr("Hover")
            Layout.fillWidth: true
            checkable: false
        }

        Button {
            id: buttonWaypoint
            width: 120
            text: qsTr("Waypoint")
            Layout.fillWidth: true
        }

        Button {
            id: buttonVelocity
            width: 120
            text: qsTr("Velocity Control")
            Layout.fillWidth: true
        }

        Button {
            id: buttonFigure8
            width: 120
            text: qsTr("Figure 8")
            Layout.fillWidth: true
        }

        Button {
            id: buttonSpecial
            width: 120
            text: qsTr("Special")
            Layout.fillWidth: true
        }

        Button {
            id: buttonRef
            width: 120
            text: qsTr("Follow Reference")
            Layout.fillWidth: true
        }

        Button {
            id: buttonHeadHome
            width: 120
            text: qsTr("Head Home")
            Layout.fillWidth: true
        }

        Button {
            id: buttonLand
            width: 120
            text: qsTr("Land")
            Layout.fillWidth: true
        }
    }
}
