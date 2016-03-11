/*
  CEEGManager.cpp - EEG manager
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

#include "CEEGManager.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>

CEEGManager::CEEGManager(QObject *parent)
  : QObject(parent),
    serialPort(0)
{

}

CEEGManager::~CEEGManager()
{

}

CEEGManager *CEEGManager::instance()
{
    static CEEGManager *instance = 0;

    if (instance == 0) {
        instance = new CEEGManager();
    }
    return instance;
}

void CEEGManager::init(void)
{
    // Init the Dobot interface
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    bool foundPortFlag = false;
    QString portName;
    int baudRate;

    foreach(QSerialPortInfo portInfo, availablePorts) {
        if (portInfo.description() == QString("Silicon Labs CP210x USB to UART Bridge") ||
            portInfo.description() == QString("CP210x USB to UART Bridge Controller") ||
            portInfo.description().contains("CP210x")) {
            foundPortFlag = true;
            portName = portInfo.portName();
            baudRate = 9600;
            qDebug() << portName;
            qDebug() << portInfo.description();
            qDebug() << portInfo.manufacturer();
            qDebug() << portInfo.portName();
            break;
        }
    }
    if (not foundPortFlag) {
        return;
    }
    serialPort = new QSerialPort(portName, this);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    if (serialPort->open(QIODevice::ReadWrite) == false) {
        return;
    }
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->setSingleShot(true);
    timer->start(100);
}

void CEEGManager::onReadyRead(void)
{
    QByteArray data = serialPort->readAll();
    int length = data.length();
    QVector<unsigned char> vector;

    vector.resize(length);
    memcpy(vector.data(), data.data(), length);

    readingFIFO += vector;
    while (readingFIFO.length() >= 36) {
        if (readingFIFO.at(0) != 0xAA or
            readingFIFO.at(1) != 0xAA) {
            readingFIFO.removeFirst();
            continue;
        }
        const unsigned char *frame = (const unsigned char *)readingFIFO.data();
        int length = (int)frame[2];
        const unsigned char *data = &frame[3];
        unsigned char acc = 0;
        for (int i = 0; i < length; i++) {
            acc += data[i];
        }
        acc = ~acc;
        QString str;
        for (int i = 0; i < 36; i++) {
            str += QString("%1 ").arg((unsigned char)frame[i], 2, 16, QChar('0'));
        }
        qDebug() << str;
        if (acc != data[length]) {
            readingFIFO.removeFirst();
            qDebug() << "Verify failed!!!!!!" << (unsigned int)acc << (unsigned int)data[length];
            continue;
        }
        EEGData eegData;

        eegData.poorSignal = frame[4];

        eegData.delta[0] = frame[7];
        eegData.delta[1] = frame[8];
        eegData.delta[2] = frame[9];

        eegData.theta[0] = frame[10];
        eegData.theta[1] = frame[11];
        eegData.theta[2] = frame[12];

        eegData.lowAlpha[0] = frame[13];
        eegData.lowAlpha[1] = frame[14];
        eegData.lowAlpha[2] = frame[15];

        eegData.highAlpha[0] = frame[16];
        eegData.highAlpha[1] = frame[17];
        eegData.highAlpha[2] = frame[18];

        eegData.lowBeta[0] = frame[19];
        eegData.lowBeta[1] = frame[20];
        eegData.lowBeta[2] = frame[21];

        eegData.highBeta[0] = frame[22];
        eegData.highBeta[1] = frame[23];
        eegData.highBeta[2] = frame[24];

        eegData.lowGamma[0] = frame[25];
        eegData.lowGamma[1] = frame[26];
        eegData.lowGamma[2] = frame[27];

        eegData.midGamma[0] = frame[28];
        eegData.midGamma[1] = frame[29];
        eegData.midGamma[2] = frame[30];

        eegData.attention = frame[32];
        eegData.mediation = frame[34];

        emit newFrame(eegData);

        readingFIFO.remove(0, 36);
    }
}

void CEEGManager::onTimeout(void)
{
    static int count = 0;
    QTimer *timer = (QTimer *)sender();

    switch (count) {
        case 0:
            serialPort->write("AT+ROLE1\r\n");
            timer->start(1000);
        break;

        case 1:
            serialPort->write("AT+INQ\r\n");
            timer->start(8000);
        break;

        case 2:
            serialPort->write("AT+CONN1\r\n");
        break;
    }
    count++;
}
