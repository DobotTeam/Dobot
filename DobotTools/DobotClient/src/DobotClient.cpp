/*
  DobotClient.cpp - Dobot client
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

#include "DobotClient.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QDebug>

DobotClient::DobotClient(QObject *parent)
  : QObject(parent),
    tcpClient(0)
{
    qRegisterMetaType<QVector<unsigned char> >("QVector<unsigned char>");

    readingFIFO.clear();

    velocityRatio = 50;
    axisCtrlState = AxisIdle;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(writeFrame()));
    timer->setInterval(500);
}

DobotClient::~DobotClient()
{

}

DobotClient *DobotClient::instance(void)
{
    static DobotClient *instance = 0;
    if (instance == 0) {
        instance = new DobotClient();
    }
    return instance;
}

void DobotClient::init(void)
{
    tcpClient = new QTcpSocket(this);
    tcpClient->connectToHost(QHostAddress::LocalHost, 5555);
    connect(tcpClient, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void DobotClient::start(void)
{
    //workerThread.start();
    //moveToThread(&workerThread);
}

void DobotClient::setJogMode(int jogMode)
{
    this->jogMode = jogMode;
}

void DobotClient::setEndType(EndType endType)
{
    Frame frame;

    memset(&frame, 0, sizeof(frame));
    frame.state = Parameter;
    frame.axis = 4;
    frame.x = (float)endType;

    cmdList.push_back(frame);
}

void DobotClient::addAxis1(void)
{
    axisCtrlState = APPressed;
    timer->start();
    writeFrame();
}

void DobotClient::subAxis1(void)
{
    axisCtrlState = ANPressed;
    timer->start();
    writeFrame();
}

void DobotClient::addAxis2(void)
{
    axisCtrlState = BPPressed;
    timer->start();
    writeFrame();
}

void DobotClient::subAxis2(void)
{
    axisCtrlState = BNPressed;
    timer->start();
    writeFrame();
}

void DobotClient::addAxis3(void)
{
    axisCtrlState = CPPressed;
    timer->start();
    writeFrame();
}

void DobotClient::subAxis3(void)
{
    axisCtrlState = CNPressed;
    timer->start();
    writeFrame();
}

void DobotClient::addAxis4(void)
{
    axisCtrlState = DPPressed;
    timer->start();
    writeFrame();
}

void DobotClient::subAxis4(void)
{
    axisCtrlState = DNPressed;
    timer->start();
    writeFrame();
}

void DobotClient::addAxis5(void)
{
    axisCtrlState = GPPressed;
    timer->start();
    writeFrame();
}

void DobotClient::subAxis5(void)
{
    axisCtrlState = GNPressed;
    timer->start();
    writeFrame();
}

void DobotClient::enableGrab(bool isGrab)
{
    axisCtrlState = isGrab ? Grab : Release;
    timer->start();
    writeFrame();
}

void DobotClient::enableLaser(bool isLaserOn)
{
    axisCtrlState = isLaserOn ? LaserOn : LaserOff;
    timer->start();
    writeFrame();
}

void DobotClient::stop(void)
{
    timer->stop();
    cmdList.clear();

    Frame frame;

    memset(&frame, 0, sizeof(frame));
    frame.state = jogMode == 0 ? Axis : Linear;
    frame.axis = AxisIdle;

    cmdList.push_back(frame);
}

void DobotClient::writeFrame(void)
{
    Frame frame;

    memset(&frame, 0, sizeof(frame));
    frame.state = jogMode == 0 ? Axis : Linear;
    frame.axis = axisCtrlState;
    frame.startVelocity = velocityRatio;

    cmdList.push_back(frame);
}

void DobotClient::playBack(MotionStyle motionStyle, bool isGrab,
              float baseAngle, float longArmAngle, float shortArmAngle, float pawAngle,
              float gripper, float pauseTime)
{
    Frame frame;

    memset(&frame, 0, sizeof(frame));
    frame.state = AxisSave;
    frame.axis = 0;
    frame.x = baseAngle;
    frame.y = longArmAngle;
    frame.z = shortArmAngle;
    frame.rHead = pawAngle;
    frame.isGrab = isGrab;
    frame.startVelocity = motionStyle;
    frame.endVelocity = gripper;
    frame.maxVelocity = pauseTime;

    cmdList.push_back(frame);
}

void DobotClient::gotoXYZ(MotionStyle motionStyle, bool isGrab,
              float baseAngle, float longArmAngle, float shortArmAngle, float pawAngle,
              float gripper, float pauseTime)
{
    Frame frame;

    memset(&frame, 0, sizeof(frame));
    frame.state = ReProc;
    frame.axis = 0;
    frame.x = baseAngle;
    frame.y = longArmAngle;
    frame.z = shortArmAngle;
    frame.rHead = pawAngle;
    frame.isGrab = isGrab;
    frame.startVelocity = motionStyle;
    frame.endVelocity = gripper;
    frame.maxVelocity = pauseTime;

    cmdList.push_back(frame);
}

void DobotClient::configJogParams(float maxAxisSpeed, float axisAccSpeed,
                                      float maxServoSpeed, float servoAccSpeed,
                                      float maxLinearSpeed, float linearAccSpeed)
{
    Frame frame;

    memset(&frame, 0, sizeof(frame));
    frame.state = Parameter;
    frame.axis = 0;
    frame.x = maxAxisSpeed;
    frame.y = axisAccSpeed;
    frame.z = maxServoSpeed;
    frame.rHead = servoAccSpeed;
    frame.isGrab = maxLinearSpeed;
    frame.startVelocity = linearAccSpeed;
    frame.endVelocity = 0;
    frame.maxVelocity = 0;

    cmdList.push_back(frame);
}

void DobotClient::setVelocityRatio(float velocityRatio)
{
    velocityRatio = qBound(0.0f, velocityRatio, 100.0f);
    this->velocityRatio = velocityRatio;
}

void DobotClient::configPlayBackStaticParams(float maxAxisSpeed, float maxAxisAccSpeed,
                                  float maxServoSpeed, float maxServoAccSpeed,
                                  float maxLinearSpeed, float maxLinearAccSpeed,
                                  float pauseTime, float jumpHeight)
{
    Frame frame;

    memset(&frame, 0, sizeof(frame));
    frame.state = Parameter;
    frame.axis = 1;
    frame.x = maxAxisSpeed;
    frame.y = maxAxisAccSpeed;
    frame.z = maxServoSpeed;
    frame.rHead = maxServoAccSpeed;
    frame.isGrab = maxLinearSpeed;
    frame.startVelocity = maxLinearAccSpeed;
    frame.endVelocity = pauseTime;
    frame.maxVelocity = jumpHeight;

    cmdList.push_back(frame);
}

void DobotClient::configUserDefParams(const QVector<float> &params)
{
    Frame frame;

    memset(&frame, 0, sizeof(frame));
    frame.state = Parameter;
    frame.axis = 3;
    frame.x = params.at(0);
    frame.y = params.at(1);
    frame.z = params.at(2);
    frame.rHead = params.at(3);
    frame.isGrab = params.at(4);
    frame.startVelocity = params.at(5);
    frame.endVelocity = params.at(6);
    frame.maxVelocity = params.at(7);

    cmdList.push_back(frame);
}

void DobotClient::configPlayBackDynamicParams(float maxVelocityRatio, float maxAccRatio)
{
    Frame frame;

    memset(&frame, 0, sizeof(frame));
    frame.state = JogParameter;
    frame.axis = 0;
    frame.x = maxVelocityRatio;
    frame.y = maxAccRatio;

    cmdList.push_back(frame);
}

void DobotClient::onConnected(void)
{
    qDebug() << "Connected!";
}

void DobotClient::onReadyRead(void)
{
    const QByteArray array = tcpClient->readAll();

    foreach(char ch, array) {
        if (readingFIFO.count() == 42) {
            readingFIFO.removeFirst();
        }
        readingFIFO.push_back((unsigned char)ch);
        if (readingFIFO.count() == 42 and
            readingFIFO.first() == 0xa5 and
            readingFIFO.last() == 0x5a) {

            QVector<unsigned char> readingFrame;
            readingFrame.resize(40);
            unsigned char *dst = readingFrame.data();
            unsigned char *src = (unsigned char *)(&(readingFIFO.data()[1]));
            memcpy(dst, src, 40);
            readingFIFO.clear();
            emit newFrame(readingFrame);

            if (cmdList.count() != 0) {
                Frame frame = cmdList.first();
                quint8 buffer[42];

                buffer[0] = 0xa5;
                buffer[41] = 0x5a;
                memcpy(&buffer[1], &frame, sizeof(frame));
                tcpClient->write((const char *)buffer, sizeof(buffer));

                cmdList.removeFirst();
            }
        }
    }
}
