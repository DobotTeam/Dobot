/*
  DobotServer.cpp - Dobot server
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

#include "DobotServer.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "COptions.h"
#include "qdevicewatcher.h"

DobotServer::DobotServer(QObject *parent)
  : QObject(parent)
{
    watcher = new QDeviceWatcher;
    watcher->appendEventReceiver(this);
    watcher->start();
}

DobotServer::~DobotServer()
{

}

bool DobotServer::isDobotOnline(QString &portName)
{
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    bool foundDobot = false;

    foreach(QSerialPortInfo portInfo, availablePorts) {
        if (portInfo.description().contains("Arduino")) {
            foundDobot = true;
            portName = portInfo.portName();
            break;
        }
    }
    emit onlineStatusUpdated(foundDobot);
    return foundDobot;
}

bool DobotServer::isDobotOnline(void)
{
    QString portName;

    return isDobotOnline(portName);
}

bool DobotServer::connectDobot(const QString &portName)
{
    const int baudRate = COptions::instance()->getBaudrate().toInt();

    dobot = new QSerialPort(portName, this);
    dobot->setBaudRate(baudRate);
    dobot->setDataBits(QSerialPort::Data8);
    dobot->setParity(QSerialPort::NoParity);
    dobot->setStopBits(QSerialPort::OneStop);
    if (dobot->open(QIODevice::ReadWrite) == false) {
        dobot->deleteLater();
        dobot = 0;
        return false;
    }
    connect(dobot, SIGNAL(readyRead()), this, SLOT(onDobotReadyRead()));
    return true;
}

DobotServer *DobotServer::instance(void)
{
    static DobotServer *instance = 0;

    if (instance == 0) {
        instance = new DobotServer();
    }
    return instance;
}

bool DobotServer::init(void)
{
    // Init the dobot server!
    tcpClient = 0;
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(onNewClientConnection()));
    if (tcpServer->listen(QHostAddress::LocalHost, 5555) == false) {
        return false;
    }
    do {
        QString portName;

        if (isDobotOnline(portName) == false) {
            break;
        }
        connectDobot(portName);
    } while (0);
    return true;
}

void DobotServer::start(void)
{
    workerThread.start();
    moveToThread(&workerThread);
}

void DobotServer::onDobotReadyRead(void)
{
    QByteArray data = dobot->readAll();

    // Currently we only support one tcp client
    if (tcpClient) {
        tcpClient->write(data);
    }
}

void DobotServer::onNewClientConnection(void)
{
    tcpClient = tcpServer->nextPendingConnection();
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(onClientReadyRead()));
}

void DobotServer::onClientReadyRead(void)
{
    QByteArray data = tcpClient->readAll();

    if (dobot) {
        dobot->write(data);
    }
}

bool DobotServer::event(QEvent *event)
{
    if (event->type() != QDeviceChangeEvent::registeredType()) {
        return false;
    }
    QDeviceChangeEvent *deviceChangeEvent = (QDeviceChangeEvent *)event;
    switch (deviceChangeEvent->action()) {
        case QDeviceChangeEvent::Add:
            qDebug() << "Device added!";
            do {
                // Only reconnect if dobot is null
                if (dobot) {
                    break;
                }
                QString portName;
                if (isDobotOnline(portName) == false) {
                    break;
                }
                if (connectDobot(portName) == false) {
                    break;
                }
            } while (0);
        break;

        case QDeviceChangeEvent::Remove:
            qDebug() << "Device removed!";
            do {
                // Delete dobot if it's offline
                QString portName;
                if (isDobotOnline(portName) == true) {
                    break;
                }
                dobot->deleteLater();
                dobot = 0;
            } while (0);
        break;
        default:
        break;
    }
    return true;
}
