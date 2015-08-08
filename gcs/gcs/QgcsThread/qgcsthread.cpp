#include <iostream>
#include <bitset>   // for printing in binary
#include "qgcsthread.h"

using namespace std;

// Constructor
QgcsThread::QgcsThread(QQuickItem *parent):
    QQuickItem(parent)
{
    // By default, QQuickItem does not draw anything. If you subclass
    // QQuickItem to create a visual item, you will need to uncomment the
    // following line and re-implement updatePaintNode()

     // setFlag(ItemHasContents, true);

    // Boolean Value Initialization
    hasBeenStarted = false;
    emit statusChanged();

    // Thread Class Initialization
    gcsThread = new GcsServerThread((double *)XqTrajDesired, (double *)XqRefDesired, (unsigned char *)newDMCs, &desiredMode);
    gcsThread->initialize();
    isconnected = &(gcsThread->connected);

    // Qthread Class Initialization
    checkThread = new updateChecker(&hasBeenStarted, &(gcsThread->updated), isconnected);
    QObject::connect(checkThread, SIGNAL(NewFrameFound()), this, SLOT(updateValues()));
    QObject::connect(checkThread, SIGNAL(connectionChanged()), this, SLOT(statusChangedSlot()));
    checkThread->start();

    sendThread = new updateSender(gcsThread, isconnected);
    sendThread->start();

    // Transmit Value Initialization
    for(int i = 0; i < POS_REFS; i++)
        XqTrajDesired[i] = 0;
    for(int i = 0; i < STATES; i++)
        XqRefDesired[i] = 0;
    for(int i = 0; i < MOTORS; i++)
        newDMCs[i] = 0;
    XqRefDesired[Q0] = 1;
    desiredMode = QUAD_OFF;

    ap_status_prev = 0x0000;

    std::cout << "Finished the constructor!" << endl;
}



// Destructor
QgcsThread::~QgcsThread()
{
    gcsThread->killThread();
    checkThread->quit();
}



// Function Calls
void QgcsThread::statusChangedSlot()
{
    emit statusChanged();
}

void QgcsThread::start()
{
    hasBeenStarted = true;
    emit statusChanged();
    cout << "Starting..." << endl;
    gcsThread->listenAndAccept();
    emit statusChanged();
    gcsThread->startThread();
    cout << "Started!" << endl;
}

void QgcsThread::updateValues()
{
    //cout << "updating values..." << endl;
    // Try to load the new frame
    if(gcsThread->gotFrame())
    {
        // Update values
        gcsThread->getStates(Xq);
        gcsThread->getReferences(XqRef);
        gcsThread->getInputs(Uq);
        gcsThread->getVicon(XqVicon);
        gcsThread->getImu(XqImu);
        gcsThread->getDMCs(motorDMCs);
        ap_status = gcsThread->getAutopilotStatus();
        batt_voltage = gcsThread->getVolts();
        cpu_load = gcsThread->getCPU();
        uart_status = gcsThread->getSerialStatus();
        vicon_status = gcsThread->getViconStatus();
        currentMode = gcsThread->getMode();

        // Emit appropriate signals
        if(ap_status != ap_status_prev) {
            //cout << "ap_status changed! " << (bitset<16>)ap_status << endl;
            emit apstatusChanged();
            ap_status_prev = ap_status;
        }
        emit voltageChanged();
        emit cpuLoadChanged();

        emit uartstatusChanged();
        emit viconstatusChanged();

        emit currModeChanged();
        emit stateChange();
        emit inputChange();
        emit viconChange();
        emit imuChange();
        emit dmcChange();

        //cout << "updated!" << endl;
    }
}

void QgcsThread::stop()
{
    gcsThread->startShutdown();
}


// Read Functions
QString QgcsThread::getStatus() const
{
    if(!hasBeenStarted)   return "disconnected";
    else if(!*isconnected) return "searching";
    else                  return "connected";
}

QString QgcsThread::getDesMode() const
{
    switch(desiredMode) {
        case QUAD_OFF:          return "off";
        case QUAD_IDLE:         return "idle";
        case QUAD_CYCLE:        return "cycle";
        case QUAD_LATENCY_TEST: return "latency";
        case QUAD_DMC:          return "motorDMCs";
        default:                return "undefined";
    }
    return "undefined";
}

QString QgcsThread::getCurrMode() const
{
    switch(currentMode) {
        case QUAD_OFF:          return "off";
        case QUAD_IDLE:         return "idle";
        case QUAD_RAMPUP:       return "rampup";
        case QUAD_TAKEOFF:      return "takeoff";
        case QUAD_HOVER:        return "hover";
        case QUAD_WAYPOINT:     return "waypoint";
        case QUAD_VELOCITY:     return "velocity";
        case QUAD_FIGURE_8:     return "figure8";
        case QUAD_SPECIAL:      return "special";
        case QUAD_FOLLOW_REF:   return "followRef";
        case QUAD_HEAD_HOME:    return "headHome";
        case QUAD_LAND:         return "land";
        case QUAD_UNSAFE:       return "unsafe";

        case QUAD_CYCLE:        return "cycle";
        case QUAD_LATENCY_TEST: return "latency";
        case QUAD_DMC:          return "motorDMCs";
        case QUAD_ATT_CTRL:     return "attCtrl";
        case QUAD_POS_CTRL:     return "posCtrl";
        case QUAD_HEIGHT_CTRL:  return "altCtrl";
        default:                return "undefined";
    }
    return "undefined";
}

QString QgcsThread::getUartStatus() const
{
    switch(uart_status) {
        case CONN_DISCONNECTED: return "disconnected";
        case CONN_CONNECTING:   return "searching";
        case CONN_CONNECTED:    return "connected";
        default:                return "undefined";
    }
    return "undefined";
}

QString QgcsThread::getViconStatus() const
{
    switch(vicon_status) {
        case CONN_DISCONNECTED: return "disconnected";
        case CONN_CONNECTING:   return "searching";
        case CONN_CONNECTED:    return "connected";
        default:                return "undefined";
    }
    return "undefined";
}

int    QgcsThread::getAPStatus()   const {return (int)ap_status;}
double QgcsThread::getVoltage()    const {return batt_voltage;}
double QgcsThread::getCpuLoad()    const {return cpu_load;}
bool   QgcsThread::haveNewFrame()  const {return gcsThread->updated;}
double QgcsThread::getState(int i) const {return Xq[i];}
double QgcsThread::getRef(int i)   const {return XqRef[i];}
double QgcsThread::getInput(int i) const {return Uq[i];}
double QgcsThread::getVicon(int i) const {return XqVicon[i];}
double QgcsThread::getImu(int i)   const {return XqImu[i];}
int    QgcsThread::getDMC(int i)   const {return motorDMCs[i];}

double QgcsThread::getState()      const {return getState(0);}
double QgcsThread::getInput()      const {return getInput(0);}
double QgcsThread::getVicon()      const {return getVicon(0);}
double QgcsThread::getImu()        const {return getImu(0);}
int    QgcsThread::getDMC()        const {return getDMC(0);}


QString QgcsThread::getAttCtrlState()
{   if(ap_status & 0x0001)   return "on";
    return "off";}
QString QgcsThread::getAltCtrlState()
{   if(ap_status & 0x0002)   return "on";
    return "off";}
QString QgcsThread::getPosCtrlState()
{   if(ap_status & 0x0004)   return "on";
    return "off";}
QString QgcsThread::getCompFailState()
{   if(ap_status & 0x0010)   return "on";
    return "off";}
QString QgcsThread::getSerIntState()
{   if(ap_status & 0x0020)   return "on";
    return "off";}
QString QgcsThread::getEmerModeState()
{   if(ap_status & 0x0080)   return "on";
    return "off";}
QString QgcsThread::getCalibErrState()
{   if(ap_status & 0x0100)   return "on";
    return "off";}
QString QgcsThread::getGyroCalErrState()
{   if(ap_status & 0x0200)   return "on";
    return "off";}
QString QgcsThread::getAccCalErrState()
{   if(ap_status & 0x0400)   return "on";
    return "off";}
QString QgcsThread::getMagStrErrState()
{   if(ap_status & 0x4000)   return "on";
    return "off";}
QString QgcsThread::getMagIncErrState()
{   if(ap_status & 0x8000)   return "on";
    return "off";}


// Write Functions
void QgcsThread::setDesMode(QString mode)
{
    if     (mode == "off")       desiredMode = QUAD_OFF;
    else if(mode == "idle")      desiredMode = QUAD_IDLE;
    else if(mode == "takeoff")   desiredMode = QUAD_TAKEOFF;
    else if(mode == "hover")     desiredMode = QUAD_HOVER;
    else if(mode == "waypoint")  desiredMode = QUAD_WAYPOINT;
    else if(mode == "velocity")  desiredMode = QUAD_VELOCITY;
    else if(mode == "figure8")   desiredMode = QUAD_FIGURE_8;
    else if(mode == "special")   desiredMode = QUAD_SPECIAL;
    else if(mode == "followRef") desiredMode = QUAD_FOLLOW_REF;
    else if(mode == "headHome")  desiredMode = QUAD_HEAD_HOME;
    else if(mode == "land")      desiredMode = QUAD_LAND;
    else if(mode == "unsafe")    desiredMode = QUAD_UNSAFE;

    else if(mode == "cycle")     desiredMode = QUAD_CYCLE;
    else if(mode == "latency")   desiredMode = QUAD_LATENCY_TEST;
    else if(mode == "motorDMCs") desiredMode = QUAD_DMC;
    else if(mode == "attCtrl")   desiredMode = QUAD_ATT_CTRL;
    else if(mode == "altCtrl")   desiredMode = QUAD_HEIGHT_CTRL;
    else if(mode == "posCtrl")   desiredMode = QUAD_POS_CTRL;
    else                         desiredMode = QUAD_UNDEFINED;
}

void QgcsThread::setTrajectory(int i, double value)
    {XqTrajDesired[i] = value;}
void QgcsThread::setReference(int i, double value)
    {XqRefDesired[i] = value;}
void QgcsThread::setMotor(int i, int value)
    {newDMCs[i] = value;}
