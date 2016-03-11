/*
  DobotClient.h - Dobot client
  Copyright (c) 2015-2016 Yuejiang Technology Co., Ltd. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef DOBOTCLIENT_H
#define DOBOTCLIENT_H

#include <QObject>
#include <QThread>
#include <QVector>

class QTcpSocket;
class QTimer;

#define BUFFER_SIZE 10240

class DobotClient : public QObject
{
    Q_OBJECT
public:
    typedef struct tagFrame{
        float state;
        float axis;
        float x;
        float y;
        float z;
        float rHead;
        float isGrab;
        float startVelocity;
        float endVelocity;
        float maxVelocity;
    }Frame;

    enum MachineState{
        MachineIdle     = 0,
        Mouse           = 1,
        Axis            = 2,
        ReProc          = 3,
        Engrave         = 4,
        Vision          = 5,
        AxisSave        = 6,
        Linear          = 7,
        Hand            = 8,
        Parameter       = 9,
        JogParameter    = 10,
        LaserState      = 13,
        AngleCmd        = 15
    };

    enum AxisCtrlState {
        AxisIdle        = 0,
        APPressed       = 1,
        ANPressed       = 2,
        BPPressed       = 3,
        BNPressed       = 4,
        CPPressed       = 5,
        CNPressed       = 6,
        DPPressed       = 7,
        DNPressed       = 8,
        Grab            = 9,
        Release         = 10,
        GPPressed       = 11,
        GNPressed       = 12,
        LaserOn         = 13,
        LaserOff        = 14
    };

    // For end type
    enum EndType {
        Pump,
        Gripper,
        Laser
    };

    // For play back
    enum MotionStyle {
        JUMP,
        MOVJ,
        MOVL
    };

    typedef struct tagPlayBackPoint {
        MotionStyle motionStyle;
        bool isGrab;
        float baseAngle;
        float longArmAngle;
        float shortArmAngle;
        float pawAngle;
        float gripper;
        float pauseTime;
    }PlayBackPoint;
private:
    DobotClient(QObject *parent = 0);
    ~DobotClient();
public:
    static DobotClient *instance(void);
    void init(void);
    void start(void);

    // Jog functions
    void setJogMode(int jogMode);
    void setEndType(EndType endType);
    void addAxis1(void);
    void subAxis1(void);
    void addAxis2(void);
    void subAxis2(void);
    void addAxis3(void);
    void subAxis3(void);
    void addAxis4(void);
    void subAxis4(void);
    void addAxis5(void);
    void subAxis5(void);
    void enableGrab(bool isGrab);
    void enableLaser(bool isLaserOn);
    void stop(void);

    // Play back functions
    void playBack(MotionStyle motionStyle, bool isGrab,
                  float baseAngle, float longArmAngle, float shortArmAngle, float pawAngle,
                  float gripper, float pauseTime);

    // Just goto
    void gotoXYZ(MotionStyle motionStyle, bool isGrab,
                  float baseAngle, float longArmAngle, float shortArmAngle, float pawAngle,
                  float gripper, float pauseTime);

    // Configuration
    void configJogParams(float maxAxisSpeed, float axisAccSpeed,
                                 float maxServoSpeed, float servoAccSpeed,
                                 float maxLinearSpeed, float linearAccSpeed);
    void setVelocityRatio(float velocityRatio);

    void configPlayBackStaticParams(float maxAxisSpeed, float maxAxisAccSpeed,
                                      float maxServoSpeed, float maxServoAccSpeed,
                                      float maxLinearSpeed, float maxLinearAccSpeed,
                                      float pauseTime, float jumpHeight);

    // ConfigUserDefinedParams
    void configUserDefParams(const QVector<float> &params);

    void configPlayBackDynamicParams(float maxVelocityRatio, float maxAccRatio);

signals:
    void newFrame(const QVector<unsigned char>&frame);
private slots:
    void writeFrame(void);
    void onConnected(void);
    void onReadyRead(void);
private:
    QThread workerThread;
    QTcpSocket *tcpClient;
    QVector<unsigned char> readingFIFO;

    QTimer *timer;
    int jogMode;
    float velocityRatio;
    AxisCtrlState axisCtrlState;

    QVector<Frame> cmdList;
};

#endif // DOBOTCLIENT_H
