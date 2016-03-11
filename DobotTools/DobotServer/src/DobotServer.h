/*
  DobotServer.h - Dobot server
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

#ifndef DOBOTSERVER_H
#define DOBOTSERVER_H

#include <QObject>
#include <QThread>

class QSerialPort;
class QTcpServer;
class QTcpSocket;

class QDeviceWatcher;

class DobotServer : public QObject
{
    Q_OBJECT
private:
    explicit DobotServer(QObject *parent = 0);
    ~DobotServer();
    bool isDobotOnline(QString &portName);
    bool isDobotOnline(void);
    bool connectDobot(const QString &portName);
signals:
    void onlineStatusUpdated(bool isOnline);
public:
    static DobotServer *instance(void);
    bool init(void);
    void start(void);
private slots:
    void onDobotReadyRead(void);
    void onNewClientConnection(void);
    void onClientReadyRead(void);
protected:
    bool event(QEvent *event);
private:
    QThread workerThread;
    QSerialPort *dobot;
    QTcpServer *tcpServer;
    QTcpSocket *tcpClient;

    QDeviceWatcher *watcher;
};

#endif // DOBOTSERVER_H
