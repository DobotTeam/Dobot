/*
  main.cpp - Dobot server main file
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

#include "MainWindow.h"
#include <QApplication>
#include <QMessageBox>
#include "DobotServer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    // The tray icon changes with the dobot connecting status
    QObject::connect(DobotServer::instance(), SIGNAL(onlineStatusUpdated(bool)), &w, SLOT(onOnlineStatusUpdated(bool)));

    if (DobotServer::instance()->init() == false) {
        QMessageBox::critical(0, "Error", "TCP port is not available!\r\nServer will exit!");
        exit(-2);
    } else {
        DobotServer::instance()->start();
    }
    return a.exec();
}
