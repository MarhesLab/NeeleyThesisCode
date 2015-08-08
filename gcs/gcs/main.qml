import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import com.wilneeleyisabadass.qmlcomponents 1.0

ApplicationWindow {
    id: applicationWindow1
    width: 1200
    height: 980
    title: qsTr("Quadrotor Ground Control System")
    color: "#ba122b"
    visible: true

    GcsThread {
        id: gcsSocketThread

        onStatusChanged: gcsCommLink.state = gcsSocketThread.status
        onUartstatusChanged: apCommLink.state = gcsSocketThread.uartstatus
        onViconstatusChanged: viconCommLink.state = gcsSocketThread.viconstatus

        onApstatusChanged: {
            attCtrl.state       = gcsSocketThread.getAttCtrlState();
            altCtrl.state       = gcsSocketThread.getAltCtrlState();
            posCtrl.state       = gcsSocketThread.getPosCtrlState();
            compFail.state      = gcsSocketThread.getCompFailState();
            serialStatus.state  = gcsSocketThread.getSerIntState();
            emerMode.state      = gcsSocketThread.getEmerModeState();
            calibErr.state      = gcsSocketThread.getCalibErrState();
            gyroCalErr.state    = gcsSocketThread.getGyroCalErrState();
            accCalErr.state     = gcsSocketThread.getAccCalErrState();
            magStrErr.state     = gcsSocketThread.getMagStrErrState();
            magIncErr.state     = gcsSocketThread.getMagIncErrState();
        }
        onVoltageChanged: {
            statusBar.batteryVoltage = qsTr("%1 V").arg(gcsSocketThread.voltage);
            statusBar.batteryLevel = gcsSocketThread.voltage;
        }
        onCpuLoadChanged: {
            statusBar.cpuPercentage = gcsSocketThread.cpuload;
            statusBar.cpuPercentageText = qsTr("%1%").arg(gcsSocketThread.cpuload);
        }

        onCurrModeChanged: currentFlightMode.state = gcsSocketThread.currMode;
        onStateChange: {
            stateDisplayBarForm.stateX  = qsTr("%1").arg(gcsSocketThread.getState(0));
            stateDisplayBarForm.stateY  = qsTr("%1").arg(gcsSocketThread.getState(1));
            stateDisplayBarForm.stateZ  = qsTr("%1").arg(gcsSocketThread.getState(2));
            stateDisplayBarForm.stateDX = qsTr("%1").arg(gcsSocketThread.getState(3));
            stateDisplayBarForm.stateDY = qsTr("%1").arg(gcsSocketThread.getState(4));
            stateDisplayBarForm.stateDZ = qsTr("%1").arg(gcsSocketThread.getState(5));
            stateDisplayBarForm.stateQ0 = qsTr("%1").arg(gcsSocketThread.getState(6));
            stateDisplayBarForm.stateQI = qsTr("%1").arg(gcsSocketThread.getState(7));
            stateDisplayBarForm.stateQJ = qsTr("%1").arg(gcsSocketThread.getState(8));
            stateDisplayBarForm.stateQK = qsTr("%1").arg(gcsSocketThread.getState(9));
            stateDisplayBarForm.stateWX = qsTr("%1").arg(gcsSocketThread.getState(10));
            stateDisplayBarForm.stateWY = qsTr("%1").arg(gcsSocketThread.getState(11));
            stateDisplayBarForm.stateWZ = qsTr("%1").arg(gcsSocketThread.getState(12));
        }
        onInputChange: {
            stateDisplayBarForm.inputT  = qsTr("%1").arg(gcsSocketThread.getInput(0));
            stateDisplayBarForm.inputtx = qsTr("%1").arg(gcsSocketThread.getInput(1));
            stateDisplayBarForm.inputty = qsTr("%1").arg(gcsSocketThread.getInput(2));
            stateDisplayBarForm.inputtz = qsTr("%1").arg(gcsSocketThread.getInput(3));
        }
        onViconChange: {
            stateDisplayBarForm.viconX  = qsTr("%1").arg(gcsSocketThread.getVicon(0));
            stateDisplayBarForm.viconY  = qsTr("%1").arg(gcsSocketThread.getVicon(1));
            stateDisplayBarForm.viconZ  = qsTr("%1").arg(gcsSocketThread.getVicon(2));
            stateDisplayBarForm.viconQ0 = qsTr("%1").arg(gcsSocketThread.getVicon(3));
            stateDisplayBarForm.viconQI = qsTr("%1").arg(gcsSocketThread.getVicon(4));
            stateDisplayBarForm.viconQJ = qsTr("%1").arg(gcsSocketThread.getVicon(5));
            stateDisplayBarForm.viconQK = qsTr("%1").arg(gcsSocketThread.getVicon(6));
        }
        onImuChange: {
            stateDisplayBarForm.imuDDX  = qsTr("%1").arg(gcsSocketThread.getImu(3));
            stateDisplayBarForm.imuDDY  = qsTr("%1").arg(gcsSocketThread.getImu(4));
            stateDisplayBarForm.imuDDZ  = qsTr("%1").arg(gcsSocketThread.getImu(5));
            stateDisplayBarForm.imuWX   = qsTr("%1").arg(gcsSocketThread.getImu(0));
            stateDisplayBarForm.imuWY   = qsTr("%1").arg(gcsSocketThread.getImu(1));
            stateDisplayBarForm.imuWZ   = qsTr("%1").arg(gcsSocketThread.getImu(2));
        }
        onDmcChange: {
            motorChars.motorChar1       = qsTr("%1").arg(gcsSocketThread.getDMC(0));
            motorChars.motorChar2       = qsTr("%1").arg(gcsSocketThread.getDMC(1));
            motorChars.motorChar3       = qsTr("%1").arg(gcsSocketThread.getDMC(2));
            motorChars.motorChar4       = qsTr("%1").arg(gcsSocketThread.getDMC(3));

            motorChars.motor1           = qsTr("%1").arg( (25 + (gcsSocketThread.getDMC(0) * 175)/200 )*43);
            motorChars.motor2           = qsTr("%1").arg( (25 + (gcsSocketThread.getDMC(1) * 175)/200 )*43);
            motorChars.motor3           = qsTr("%1").arg( (25 + (gcsSocketThread.getDMC(2) * 175)/200 )*43);
            motorChars.motor4           = qsTr("%1").arg( (25 + (gcsSocketThread.getDMC(3) * 175)/200 )*43);
        }
    }


    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
        Menu {
            title: qsTr("&Test Modes")
            MenuItem {
                text: qsTr("&Cycle Motors")
                onTriggered: desiredFlightMode.state = 'cycle'
            }
            MenuItem {
                text: qsTr("&Latency Test")
                onTriggered: desiredFlightMode.state = 'latency'
            }
            MenuItem {
                text: qsTr("&Direct Motor Control")
                onTriggered: desiredFlightMode.state = 'motorDMCs'
            }
            MenuItem {
                text: qsTr("&Attitude Control")
                onTriggered: desiredFlightMode.state = 'attCtrl'
            }
            MenuItem {
                text: qsTr("&Height Control")
                onTriggered: desiredFlightMode.state = 'altCtrl'
            }
            MenuItem {
                text: qsTr("&Position Control")
                onTriggered: desiredFlightMode.state = 'posCtrl'
            }
        }
        Menu {
            title: qsTr("&Diagnostic Info")
            MenuItem {
                text: qsTr("&State Info")
                onTriggered: stateDisplayBarForm.visible = !stateDisplayBarForm.visible;
            }
        }
    }




    MainForm {
        id: quadModeSelect
        width: 135
        height: 370
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.left: parent.left
        anchors.leftMargin: 30
        buttonOff.onClicked:        desiredFlightMode.state = 'off'
        buttonIdle.onClicked:       desiredFlightMode.state = 'idle'
        buttonTakeoff.onClicked:    desiredFlightMode.state = 'takeoff'
        buttonHover.onClicked:      desiredFlightMode.state = 'hover'
        buttonWaypoint.onClicked:   desiredFlightMode.state = 'waypoint'
        buttonVelocity.onClicked:   desiredFlightMode.state = 'velocity'
        buttonFigure8.onClicked:    desiredFlightMode.state = 'figure8'
        buttonSpecial.onClicked:    desiredFlightMode.state = 'special'
        buttonRef.onClicked:        desiredFlightMode.state = 'followRef'
        buttonHeadHome.onClicked:   desiredFlightMode.state = 'headHome'
        buttonLand.onClicked:       desiredFlightMode.state = 'land'

        Text {
            id: quadModeTitle
            x: 4
            width: 136
            height: 45
            color: "#000000"
            text: qsTr("Quadrotor Control and Flight Patterns")
            anchors.top: parent.top
            anchors.topMargin: -50
            anchors.horizontalCenter: parent.horizontalCenter
            scale: 1
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 15
        }

        Rectangle {
            id: quadModeBox
            color: "#ffffff"
            radius: 10
            anchors.right: parent.right
            anchors.rightMargin: -10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -10
            z: -1
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.top: parent.top
            anchors.topMargin: -60
            border.color: "#a4a3a7"
            border.width: 3
        }
    }


    StatusBarForm {
        id: statusBar
        width: 135
        height: 230
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.right: parent.right
        anchors.rightMargin: 30

        Text {
            id: statusBarTitle
            x: 4
            width: 136
            height: 45
            color: "#000000"
            text: qsTr("Autopilot Stats and Flight Mode")
            wrapMode: Text.WordWrap
            anchors.topMargin: -50
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 15
            scale: 1
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Rectangle {
            id: statusBarBox
            color: "#ffffff"
            radius: 10
            anchors.left: parent.left
            anchors.topMargin: -60
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.leftMargin: -10
            anchors.bottomMargin: -10
            anchors.rightMargin: -10
            border.width: 3
            anchors.right: parent.right
            border.color: "#a4a3a7"
            z: -1
        }
    }



    MessageDialog {
        id: messageDialog
        title: qsTr("Uh, hi?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

    MotorCharsForm {
        id: motorChars
        y: 486
        height: 310
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 105
        motorCharIn1.onValueChanged: gcsSocketThread.setMotor(0, motorCharIn1.value);
        motorCharIn2.onValueChanged: gcsSocketThread.setMotor(1, motorCharIn2.value);
        motorCharIn3.onValueChanged: gcsSocketThread.setMotor(2, motorCharIn3.value);
        motorCharIn4.onValueChanged: gcsSocketThread.setMotor(3, motorCharIn4.value);

        Text {
            id: motorCharsTitle
            x: 4
            width: 136
            height: 20
            color: "#000000"
            text: qsTr("Motor Speeds")
            wrapMode: Text.WordWrap
            anchors.topMargin: -25
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 15
            scale: 1
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Rectangle {
            id: motorCharsBox
            x: -10
            y: -110
            color: "#ffffff"
            radius: 10
            anchors.left: parent.left
            anchors.topMargin: -35
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.leftMargin: -10
            anchors.bottomMargin: -10
            anchors.rightMargin: -10
            border.width: 3
            anchors.right: parent.right
            border.color: "#a4a3a7"
            z: -1
        }

    }

    CommStatusForm {
        id: commStatusForm
        x: 1025
        height: 170
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 105
        anchors.right: parent.right
        anchors.rightMargin: 30
        gcsConnectButton.onClicked: gcsSocketThread.start();


        Rectangle {
            id: commStatusBox
            height: 270
            color: "#ffffff"
            radius: 10
            anchors.bottomMargin: -10
            border.width: 3
            border.color: "#a4a3a7"
            anchors.right: parent.right
            anchors.topMargin: -60
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.rightMargin: -10
            z: -1
            anchors.top: parent.top
            anchors.leftMargin: -9
        }

        Text {
            id: commStatusTitle
            x: 4
            width: 136
            height: 45
            color: "#000000"
            text: qsTr("Communication Link Statuses")
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.topMargin: -50
            anchors.top: parent.top
            wrapMode: Text.WordWrap
            font.pixelSize: 15
            scale: 1
            verticalAlignment: Text.AlignVCenter
        }
    }

    ApStatusBarForm {
        id: apStatusBarForm
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            id: statusBarBox1
            color: "#ffffff"
            radius: 10
            anchors.leftMargin: -10
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottomMargin: -10
            anchors.rightMargin: -10
            anchors.topMargin: -10
            border.width: 3
            z: -1
            anchors.bottom: parent.bottom
            border.color: "#a4a3a7"
        }
    }



    StateDisplayForm {
        id: stateDisplayBarForm
        x: 220
        y: 665
        height: 250
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 105
        visible: true

        Rectangle {
            id: statusBarBox2
            color: "#ffffff"
            radius: 10
            anchors.leftMargin: -10
            anchors.top: parent.top
            z: -1
            border.width: 3
            anchors.bottom: parent.bottom
            anchors.rightMargin: -10
            border.color: "#a4a3a7"
            anchors.right: parent.right
            anchors.topMargin: -10
            anchors.left: parent.left
            anchors.bottomMargin: -10
        }
    }

    Rectangle {
        id: logoBox
        width: 780
        height: 78
        color: "#ffffff"
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            id: logoBorder
            x: 62
            y: -10
            color: "#ffffff"
            radius: 10
            anchors.left: parent.left
            anchors.topMargin: -10
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.leftMargin: -10
            anchors.bottomMargin: -10
            anchors.rightMargin: -10
            border.width: 3
            anchors.right: parent.right
            border.color: "#a4a3a7"
            z: -1
        }

        Image {
            id: unmECELogo
            y: 0
            width: 635
            height: 78
            anchors.left: parent.left
            anchors.leftMargin: 0
            sourceSize.height: 96
            sourceSize.width: 780
            anchors.top: parent.top
            anchors.topMargin: 0
            antialiasing: true
            scale: 1
            source: "pictures/pic_UNM_ECE.png"
        }

        Image {
            id: marhesIcon
            width: 117
            height: 78
            anchors.right: parent.right
            anchors.rightMargin: 11
            anchors.top: parent.top
            anchors.topMargin: 0
            antialiasing: true
            fillMode: Image.Stretch
            source: "pictures/PsychicToasterSmall.png"
        }
    }

    WaypointAreaForm {
        id: waypointAreaForm
        x: 400
        width: 780
        height: 395
        anchors.top: parent.top
        anchors.topMargin: 150
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            id: waypointAreaBox
            color: "#ffffff"
            radius: 10
            anchors.bottomMargin: -10
            anchors.top: parent.top
            anchors.rightMargin: -10
            anchors.right: parent.right
            border.width: 3
            border.color: "#a4a3a7"
            anchors.topMargin: -10
            z: -1
            anchors.left: parent.left
            anchors.leftMargin: -10
            anchors.bottom: parent.bottom
        }
    }

    StateGroup {
        id: currentFlightMode
        states: [
            // Flight Modes
            State {name: "off"
                PropertyChanges {target: statusBar; currentMode: qsTr("Off");               currentModeColor: "#999999"}  },
            State {name: "idle"
                PropertyChanges {target: statusBar; currentMode: qsTr("Idle");              currentModeColor: "#00FF00"}  },
            State {name: "takeoff"
                PropertyChanges {target: statusBar; currentMode: qsTr("Taking Off");        currentModeColor: "#00FF00"}  },
            State {name: "hover"
                PropertyChanges {target: statusBar; currentMode: qsTr("Hovering");          currentModeColor: "#00FF00"}  },
            State {name: "waypoint"
                PropertyChanges {target: statusBar; currentMode: qsTr("To Waypoint");       currentModeColor: "#00FF00"}  },
            State {name: "velocity"
                PropertyChanges {target: statusBar; currentMode: qsTr("Velocity Control");  currentModeColor: "#00FF00"}  },
            State {name: "figure8"
                PropertyChanges {target: statusBar; currentMode: qsTr("Figure 8");          currentModeColor: "#00FF00"}  },
            State {name: "followRef"
                PropertyChanges {target: statusBar; currentMode: qsTr("Follow Reference");  currentModeColor: "#FFFF00"}  },
            State {name: "headHome"
                PropertyChanges {target: statusBar; currentMode: qsTr("Heading Home");      currentModeColor: "#00FF00"}  },
            State {name: "land"
                PropertyChanges {target: statusBar; currentMode: qsTr("Landing");           currentModeColor: "#00FF00"}  },
            State {name: "unsafe"
                PropertyChanges {target: statusBar; currentMode: qsTr("Unsafe");            currentModeColor: "#FF0000"}  },

            // Testing Modes
            State {name: "cycle"
                PropertyChanges {target: statusBar; currentMode: qsTr("Cycle Motors");      currentModeColor: "#0099FF"}  },
            State {name: "latency"
                PropertyChanges {target: statusBar; currentMode: qsTr("Latency Testing");   currentModeColor: "#0099FF"}  },
            State {name: "motorDMCs"
                PropertyChanges {target: statusBar; currentMode: qsTr("Direct Motor Ctrl"); currentModeColor: "#0099FF"}  },
            State {name: "attCtrl"
                PropertyChanges {target: statusBar; currentMode: qsTr("Attitude Control");  currentModeColor: "#0099FF"}  },
            State {name: "altCtrl"
                PropertyChanges {target: statusBar; currentMode: qsTr("Height Control");    currentModeColor: "#0099FF"} },
            State {name: "posCtrl"
                PropertyChanges {target: statusBar; currentMode: qsTr("Position Control");  currentModeColor: "#0099FF"} },

            // Undefined
            State {name: "undefined"
                PropertyChanges {target: statusBar; currentMode: qsTr("!!! UNDEFINED !!!"); currentModeColor: "#FF0000"}  }

        ]
    }

    StateGroup {
        id: desiredFlightMode
        states: [
            // Flight Modes
            State {name: "off"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Off");               desiredModeColor: "#999999"}
                PropertyChanges {target: gcsSocketThread; desMode: "off"} },
            State {name: "idle"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Idle");              desiredModeColor: "#00FF00"}
                PropertyChanges {target: gcsSocketThread; desMode: "idle"} },
            State {name: "takeoff"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Taking Off");        desiredModeColor: "#00FF00"}
                PropertyChanges {target: gcsSocketThread; desMode: "takeoff"} },
            State {name: "hover"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Hovering");          desiredModeColor: "#00FF00"}
                PropertyChanges {target: gcsSocketThread; desMode: "hover"} },
            State {name: "waypoint"
                PropertyChanges {target: statusBar; desiredMode: qsTr("To Waypoint");       desiredModeColor: "#00FF00"}
                PropertyChanges {target: gcsSocketThread; desMode: "waypoint"} },
            State {name: "velocity"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Velocity Control");  desiredModeColor: "#00FF00"}
                PropertyChanges {target: gcsSocketThread; desMode: "velocity"} },
            State {name: "figure8"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Figure 8");          desiredModeColor: "#00FF00"}
                PropertyChanges {target: gcsSocketThread; desMode: "figure8"} },
            State {name: "special"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Special");           desiredModeColor: "#00FF00"}
                PropertyChanges {target: gcsSocketThread; desMode: "special"} },
            State {name: "followRef"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Follow Reference");  desiredModeColor: "#FFFF00"}
                PropertyChanges {target: gcsSocketThread; desMode: "followRef"} },
            State {name: "headHome"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Heading Home");      desiredModeColor: "#00FF00"}
                PropertyChanges {target: gcsSocketThread; desMode: "headHome"} },
            State {name: "land"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Landing");           desiredModeColor: "#00FF00"}
                PropertyChanges {target: gcsSocketThread; desMode: "land"} },
            State {name: "unsafe"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Unsafe");            desiredModeColor: "#FF0000"}
                PropertyChanges {target: gcsSocketThread; desMode: "unsafe"} },

            // Testing Modes
            State {name: "cycle"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Cycle Motors");      desiredModeColor: "#0099FF"}
                PropertyChanges {target: gcsSocketThread; desMode: "cycle"} },
            State {name: "latency"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Latency Testing");   desiredModeColor: "#0099FF"}
                PropertyChanges {target: gcsSocketThread; desMode: "latency"} },
            State {name: "motorDMCs"
                PropertyChanges {target: dmcMode; state: "on" }
                PropertyChanges {target: statusBar; desiredMode: qsTr("Direct Motor Ctrl"); desiredModeColor: "#0099FF"}
                PropertyChanges {target: gcsSocketThread; desMode: "motorDMCs"} },
            State {name: "attCtrl"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Attitude Control");  desiredModeColor: "#0099FF"}
                PropertyChanges {target: gcsSocketThread; desMode: "attCtrl"} },
            State {name: "altCtrl"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Height Control");    desiredModeColor: "#0099FF"}
                PropertyChanges {target: gcsSocketThread; desMode: "altCtrl"} },
            State {name: "posCtrl"
                PropertyChanges {target: statusBar; desiredMode: qsTr("Position Control");  desiredModeColor: "#0099FF"}
                PropertyChanges {target: gcsSocketThread; desMode: "posCtrl"} },

            // Undefined
            State {name: "undefined"
                PropertyChanges {target: statusBar; desiredMode: qsTr("!!! UNDEFINED !!!"); desiredModeColor: "#FF0000"} }

        ]
    }

    StateGroup {
        id: gcsCommLink
        states: [
            State {name: "disconnected"
                PropertyChanges {target: commStatusForm;    gcsConnText: qsTr("Disconnected");      gcsConnColor: "#FF0000"} },
            State {name: "searching"
                PropertyChanges {target: commStatusForm;    gcsConnText: qsTr("Searching...");      gcsConnColor: "#FFFF00"} },
            State {name: "connected"
                PropertyChanges {target: commStatusForm;    gcsConnText: qsTr("Connected");         gcsConnColor: "#00FF00"} }
        ]
    }

    StateGroup {
        id: apCommLink
        states: [
            State {name: "disconnected"
                PropertyChanges {target: commStatusForm;    apConnText: qsTr("Disconnected");       apConnColor: "#FF0000"} },
            State {name: "searching"
                PropertyChanges {target: commStatusForm;    apConnText: qsTr("Searching...");       apConnColor: "#FFFF00"} },
            State {name: "connected"
                PropertyChanges {target: commStatusForm;    apConnText: qsTr("Connected");          apConnColor: "#00FF00"} }
        ]
    }

    StateGroup {
        id: viconCommLink
        states: [
            State {name: "disconnected"
                PropertyChanges {target: commStatusForm;    viconConnText: qsTr("Disconnected");    viconConnColor: "#FF0000"} },
            State {name: "searching"
                PropertyChanges {target: commStatusForm;    viconConnText: qsTr("Searching...");    viconConnColor: "#FFFF00"} },
            State {name: "connected"
                PropertyChanges {target: commStatusForm;    viconConnText: qsTr("Connected");       viconConnColor: "#00FF00"} }
        ]
    }

    StateGroup {
        id: serialStatus
        states: [
            State {name: "on"
                PropertyChanges {target: statusBar;         serialText: qsTr("Enabled");            serialColor: "#00FF00"} },
            State {name: "off"
                PropertyChanges {target: statusBar;         serialText: qsTr("Disabled");           serialColor: "#FF0000"} }
        ]
    }

    StateGroup {    // attitude control activated: state = on
        id: attCtrl
        states: [
            State {name: "on"
                PropertyChanges {target: apStatusBarForm;   attCtrlText: qsTr("Att. Ctrl. On");     attCtrlColor: "#00FF00"} },
            State {name: "off"
                PropertyChanges {target: apStatusBarForm;   attCtrlText: qsTr("Att. Ctrl. Off");    attCtrlColor: "#FF0000"} }
        ]
    }

    StateGroup {    // altitude/height control activated: state = on
        id: altCtrl
        states: [
            State {name: "on"
                PropertyChanges {target: apStatusBarForm;   altCtrlText: qsTr("Alt. Ctrl. On");     altCtrlColor: "#00FF00"} },
            State {name: "off"
                PropertyChanges {target: apStatusBarForm;   altCtrlText: qsTr("Alt. Ctrl. Off");    altCtrlColor: "#FF0000"} }
        ]
    }

    StateGroup {    // position control activated: state = on
        id: posCtrl
        states: [
            State {name: "on"
                PropertyChanges {target: apStatusBarForm;   posCtrlText: qsTr("Pos. Ctrl. On");     posCtrlColor: "#00FF00"} },
            State {name: "off"
                PropertyChanges {target: apStatusBarForm;   posCtrlText: qsTr("Pos. Ctrl. Off");    posCtrlColor: "#FF0000"} }
        ]
    }

    StateGroup {    // compass failure: state = on
        id: compFail
        states: [
            State {name: "on"
                PropertyChanges {target: apStatusBarForm;   compFailText: qsTr("Compass Bad");      compFailColor: "#FF0000"} },
            State {name: "off"
                PropertyChanges {target: apStatusBarForm;   compFailText: qsTr("Compass Good");     compFailColor: "#00FF00"} }
        ]
    }

    StateGroup {    // emergency mode activated: state = on
        id: emerMode
        states: [
            State {name: "on"
                PropertyChanges {target: apStatusBarForm;   emerModeText: qsTr("Emergency Mode");   emerModeColor: "#FF0000"} },
            State {name: "off"
                PropertyChanges {target: apStatusBarForm;   emerModeText: qsTr("Normal Mode");      emerModeColor: "#00FF00"} }
        ]
    }

    StateGroup {    // calibration error: state = on
        id: calibErr
        states: [
            State {name: "on"
                PropertyChanges {target: apStatusBarForm;   calibErrText: qsTr("Calibration Err."); calibErrColor: "#FF0000"} },
            State {name: "off"
                PropertyChanges {target: apStatusBarForm;   calibErrText: qsTr("Calibration Good"); calibErrColor: "#00FF00"} }
        ]
    }

    StateGroup {    // gyroscope calibration error: state = on
        id: gyroCalErr
        states: [
            State {name: "on"
                PropertyChanges {target: apStatusBarForm;   gyroCalErrText: qsTr("Gyro Cal. Err."); gyroCalErrColor: "#FF0000"} },
            State {name: "off"
                PropertyChanges {target: apStatusBarForm;   gyroCalErrText: qsTr("Gyro Cal. Good"); gyroCalErrColor: "#00FF00"} }
        ]
    }

    StateGroup {    // accelerometer calibration error: state = on
        id: accCalErr
        states: [
            State {name: "on"
                PropertyChanges {target: apStatusBarForm;   accCalErrText: qsTr("Acc. Cal. Err.");  accCalErrColor: "#FF0000"} },
            State {name: "off"
                PropertyChanges {target: apStatusBarForm;   accCalErrText: qsTr("Acc. Cal. Good");  accCalErrColor: "#00FF00"} }
        ]
    }

    StateGroup {    // magnetic field strength error: state = on
        id: magStrErr
        states: [
            State {name: "on"
                PropertyChanges {target: apStatusBarForm;   magStrErrText: qsTr("Mag. Str. Err.");  magStrErrColor: "#FF0000"} },
            State {name: "off"
                PropertyChanges {target: apStatusBarForm;   magStrErrText: qsTr("Mag. Str. Good");  magStrErrColor: "#00FF00"} }
        ]
    }

    StateGroup {    // magnetic inclination error: state = on
        id: magIncErr
        states: [
            State {name: "on"
                PropertyChanges {target: apStatusBarForm;   magIncErrText: qsTr("Mag. Str. Err.");  magIncErrColor: "#FF0000"} },
            State {name: "off"
                PropertyChanges {target: apStatusBarForm;   magIncErrText: qsTr("Mag. Str. Good");  magIncErrColor: "#00FF00"} }
        ]
    }

    StateGroup {
        id: dmcMode
        states: [
            State {name: "off"
                PropertyChanges {target: motorChars;                dmcColor: "#999999"}
                PropertyChanges {target: motorChars;                dmcText: qsTr("DMC Mode Off")}
                PropertyChanges {target: motorChars.motorBox1;      visible: true   }
                PropertyChanges {target: motorChars.motorBox2;      visible: true   }
                PropertyChanges {target: motorChars.motorBox3;      visible: true   }
                PropertyChanges {target: motorChars.motorBox4;      visible: true   }
                PropertyChanges {target: motorChars.motorCharIn1;   visible: false  }
                PropertyChanges {target: motorChars.motorCharIn2;   visible: false  }
                PropertyChanges {target: motorChars.motorCharIn3;   visible: false  }
                PropertyChanges {target: motorChars.motorCharIn4;   visible: false  }
            },
            State {name: "on"
                PropertyChanges {target: motorChars;                dmcColor: "#00FF00"}
                PropertyChanges {target: motorChars;                dmcText: qsTr("DMC Mode On")}
                PropertyChanges {target: motorChars.motorBox1;      visible: false }
                PropertyChanges {target: motorChars.motorBox2;      visible: false }
                PropertyChanges {target: motorChars.motorBox3;      visible: false }
                PropertyChanges {target: motorChars.motorBox4;      visible: false }
                PropertyChanges {target: motorChars.motorCharIn1;   visible: true  }
                PropertyChanges {target: motorChars.motorCharIn2;   visible: true  }
                PropertyChanges {target: motorChars.motorCharIn3;   visible: true  }
                PropertyChanges {target: motorChars.motorCharIn4;   visible: true  }
            }
        ]
    }



}
