#ifndef QGCSTHREAD_H
#define QGCSTHREAD_H

#define sendFrequency 20 // in Hz; not quite exact

#include "../../../thread/GcsServerThread.h"
#include "../../../defs.h"
#include <qthread.h>
#include <QQuickItem>
#include <QString>
#include <QVector2D>

class updateChecker : public QThread
{
    Q_OBJECT
public:
    updateChecker(bool* link, bool* frame, bool* conn)
    {   linkStarted = link; newFrameQueued = frame;
        isConnected = conn; wasConnected = *conn;}
    ~updateChecker() {}

    virtual void run() {  while(1) {
        if(*linkStarted && *newFrameQueued)
            emit NewFrameFound();
        if(*isConnected != wasConnected) {
            emit connectionChanged();
            wasConnected = *isConnected; }
        usleep(50);
    }  }

private:
    bool* linkStarted;
    bool* newFrameQueued;
    bool* isConnected;
    bool wasConnected;
signals:
    void NewFrameFound();
    void connectionChanged();
};



class updateSender : public QThread
{
    Q_OBJECT
public:
    updateSender(GcsServerThread* gcsst, bool* conn)
    {   server = gcsst; connected = conn;}
    ~updateSender() {}

    virtual void run() {  while(1) {
        if(*connected)
            server->sendFrame();
        usleep(1000000/sendFrequency);
    }  }

private:
    GcsServerThread* server;
    bool* connected;
};



class QgcsThread : public QQuickItem
{
    Q_OBJECT

    // Autopilot Board Status Values
    Q_PROPERTY(int apstatus READ getAPStatus NOTIFY apstatusChanged)
    Q_PROPERTY(double voltage READ getVoltage NOTIFY voltageChanged)
    Q_PROPERTY(double cpuload READ getCpuLoad NOTIFY cpuLoadChanged)

    // Communication Link Status Values
    Q_PROPERTY(QString status      READ getStatus NOTIFY statusChanged)
    Q_PROPERTY(QString uartstatus  READ getUartStatus  NOTIFY uartstatusChanged)
    Q_PROPERTY(QString viconstatus READ getViconStatus NOTIFY viconstatusChanged)

    // Edison-generated Values
    Q_PROPERTY(QString currMode READ getCurrMode  NOTIFY currModeChanged)
    Q_PROPERTY(double state READ getState NOTIFY stateChange)
    Q_PROPERTY(double input READ getInput NOTIFY inputChange)
    Q_PROPERTY(double vicon READ getVicon NOTIFY viconChange)
    Q_PROPERTY(double imu   READ getImu   NOTIFY imuChange)
    Q_PROPERTY(double dmcs  READ getDMC   NOTIFY dmcChange)

    // Property values to update
    Q_PROPERTY(QString desMode READ getDesMode WRITE setDesMode)

    // Flags for updates
    Q_PROPERTY(bool gotNewFrame READ haveNewFrame NOTIFY newFrame)

    Q_DISABLE_COPY(QgcsThread)

public slots:
    void statusChangedSlot();
    void start();
    void updateValues();
    void stop();

    double getState(int i) const;
    double getRef(int i) const;
    double getInput(int i) const;
    double getVicon(int i) const;
    double getImu(int i)   const;
    int    getDMC(int i)   const;

    QString getAttCtrlState();
    QString getAltCtrlState();
    QString getPosCtrlState();
    QString getCompFailState();
    QString getSerIntState();
    QString getEmerModeState();
    QString getCalibErrState();
    QString getGyroCalErrState();
    QString getAccCalErrState();
    QString getMagStrErrState();
    QString getMagIncErrState();

    void setTrajectory(int i, double value);
    void setReference(int i, double value);
    void setMotor(int i, int value);

signals:
    // Autopilot status-related signals
    void apstatusChanged();
    void voltageChanged();
    void cpuLoadChanged();

    // Comm link-related signals
    void statusChanged();
    void uartstatusChanged();
    void viconstatusChanged();

    void newFrame();

    // Edison-related signals
    void currModeChanged();
    void stateChange();
    void inputChange();
    void viconChange();
    void imuChange();
    void dmcChange();

public:
    QgcsThread(QQuickItem *parent = 0);
    ~QgcsThread();

    // Read functions
    QString getStatus()      const;
    QString getUartStatus()  const;
    QString getViconStatus() const;
    QString getDesMode()     const;
    QString getCurrMode()    const;
    int getAPStatus()        const;
    double getVoltage()      const;
    double getCpuLoad()      const;
    bool haveNewFrame()      const;

    double getState()        const;
    double getInput()        const;
    double getVicon()        const;
    double getImu()          const;
    int    getDMC()          const;

    // Write functions
    void setDesMode(QString mode);
    void setCurrMode(QString mode);

private:
    // Original Class Definition
    GcsServerThread* gcsThread;

    // Qthread class definitions (used for looking for updates and sending data)
    updateChecker* checkThread;
    updateSender*  sendThread;

    // Transmitted Values
    double XqTrajDesired[POS_REFS];
    double XqRefDesired[STATES];
    unsigned char newDMCs[MOTORS];
    FlightMode desiredMode;

    // Received Values
    double Xq[STATES];
    double XqRef[STATES];
    double Uq[INPUTS];
    double XqVicon[VIC_STATES];
    double XqImu[IMU_STATES];
    unsigned char motorDMCs[MOTORS];
    short ap_status, ap_status_prev;
    double batt_voltage;
    double cpu_load;
    ConnStatus uart_status;
    ConnStatus vicon_status;
    FlightMode currentMode;

    // Other Values
    bool hasBeenStarted;
    bool* isconnected;
};

#endif // QGCSTHREAD_H

