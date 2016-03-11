/*
  MainWindow.cpp - Dobot server main window
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
#include <QtWidgets>

#include <QApplication>
#include <QProcess>
#include <QDebug>

#include "COptions.h"
#include "COptionsDialog.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
    QMenu *trayIconMenu = new QMenu(this);
    trayIconMenu->setObjectName("trayIconMenu");

    QAction *optionsAct = new QAction("Options...", this);
    connect(optionsAct, SIGNAL(triggered()), this, SLOT(onOptionsActTriggered()));
    trayIconMenu->addAction(optionsAct);

    trayIconMenu->addSeparator();

    QAction *quitAct = new QAction("Exit", this);
    connect(quitAct, SIGNAL(triggered()), this, SLOT(onQuitActTriggered()));
    trayIconMenu->addAction(quitAct);

    QIcon icon(":/Dobot_online.png");
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setObjectName("trayIcon");
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("Dobot server");
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initLayout(void)
{

}

void MainWindow::onOptionsActTriggered(void)
{
    // Open the configuration dialog
    COptionsDialog dlg(COptions::instance()->getBaudrate(), this);
    if (dlg.exec() == QDialog::Accepted) {
        COptions::instance()->setBaudrate(dlg.getBaudrate());
    }
    QMessageBox::information(this, "Warning", "DobotServer's gonna restart!\n You should at the same time restart DobotClient!");
}

void MainWindow::onQuitActTriggered(void)
{
    qApp->quit();
}

void MainWindow::onOnlineStatusUpdated(bool isOnline)
{
    // The tray icon changes with the dobot connecting status
    QSystemTrayIcon *trayIcon = findChild<QSystemTrayIcon *>("trayIcon");

    if (trayIcon == 0) {
        return;
    }
    if (isOnline) {
        trayIcon->setIcon(QIcon(":/Dobot_online.png"));
    } else {
        trayIcon->setIcon(QIcon(":/Dobot_offline.png"));
    }
}
