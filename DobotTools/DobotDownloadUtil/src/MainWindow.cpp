/*
  MainWindow.cpp - Dobot download utility main window
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
#include <QSerialPortInfo>
#include <QDebug>
#include "CSelectSerialDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(800, 500);

    textEdit = new QPlainTextEdit;
    textEdit->setFixedWidth(300);
    textEdit->setReadOnly(true);
    setCentralWidget(textEdit);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions(void)
{
    openAct = new QAction(QIcon(":/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    closeAct = new QAction(tr("Close"), this);
    closeAct->setShortcuts(QKeySequence::Close);
    closeAct->setStatusTip(tr("Close existing file"));
    closeAct->setEnabled(false);
    connect(closeAct, SIGNAL(triggered()), this, SLOT(closeFile()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    downloadAct = new QAction(QIcon(":/download.png"), tr("&Download"), this);
    downloadAct->setStatusTip(tr("Download the arduino code"));
    downloadAct->setEnabled(false);
    connect(downloadAct, SIGNAL(triggered()), this, SLOT(download()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus(void)
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    arduinoMenu = menuBar()->addMenu(tr("&Arduino"));
    arduinoMenu->addAction(downloadAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars(void)
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);

    arduinoToolBar = addToolBar(tr("Tool"));
    arduinoToolBar->addAction(downloadAct);
}

void MainWindow::createStatusBar(void)
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows(void)
{
    QDockWidget *dock = new QDockWidget(tr("Output"), this);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);

    outputEdit = new QPlainTextEdit(dock);
    outputEdit->setReadOnly(true);

    dock->setWidget(outputEdit);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());
}

void MainWindow::openFile(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open hex file..."),
                                                    "./", tr("Hex(*.hex)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly)) {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
        }
        this->fileName = fileName;
        closeAct->setEnabled(true);
        downloadAct->setEnabled(true);
        textEdit->clear();
        textEdit->setPlainText(file.readAll());
        statusBar()->showMessage(tr("File loaded"), 2000);
    }
}

void MainWindow::closeFile(void)
{
    fileName = QString("");
    closeAct->setEnabled(false);
    downloadAct->setEnabled(false);
    textEdit->clear();
}

void MainWindow::download(void)
{
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    bool foundDobot = false;
    QString portName;
    int baudRate;

    foreach(QSerialPortInfo portInfo, availablePorts) {
        if (portInfo.description() == QString("Arduino Mega 2560")) {
            foundDobot = true;
            portName = portInfo.portName();
            baudRate = 115200;
            qDebug() << portName;
            qDebug() << portInfo.description();
            qDebug() << portInfo.manufacturer();
            qDebug() << portInfo.portName();
            break;
        }
    }
    if (not foundDobot) {
        CSelectSerialDialog dlg;
        if (dlg.exec() == QDialog::Rejected) {
            qDebug() << "Download canceled!";
            return;
        }
        portName = dlg.getSelectedSerial();
        qDebug() << portName;
        baudRate = 115200;
    }
    closeAct->setEnabled(false);
    downloadAct->setEnabled(false);
    outputEdit->clear();

    QString program;
#ifdef QT_DEBUG
    program = QString("%1/../../DobotDownloadUtil/file/avrdude.exe").arg(qApp->applicationDirPath());
#else
    program = QString("%1/file/avrdude.exe").arg(qApp->applicationDirPath());
#endif
    QStringList arguments;

    arguments << QString("-v");
    arguments << QString("-patmega2560");
    arguments << QString("-cwiring");
    arguments << QString("-P%1").arg(portName);
    arguments << QString("-b%1").arg(baudRate);
    arguments << QString("-D");
    arguments << QString("-Uflash:w:%1:i").arg(fileName);

    avrdudeProcess = new QProcess(this);
    avrdudeProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(avrdudeProcess, SIGNAL(readyRead()), this, SLOT(onProcessReadyRead()));
    connect(avrdudeProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onProcessFinished(int,QProcess::ExitStatus)));
    avrdudeProcess->start(program, arguments);
}

void MainWindow::about(void)
{
   QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}

void MainWindow::onProcessReadyRead(void)
{
    outputEdit->insertPlainText(QString::fromLatin1(avrdudeProcess->readAll()));
    outputEdit->verticalScrollBar()->setValue(outputEdit->verticalScrollBar()->maximum());
}

void MainWindow::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus)
    closeAct->setEnabled(true);
    downloadAct->setEnabled(true);
    qDebug() << exitCode;
    avrdudeProcess->deleteLater();
}
