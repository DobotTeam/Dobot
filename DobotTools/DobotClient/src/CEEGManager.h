/*
  CEEGManager.h - EEG manager
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

#ifndef CEEGMANAGER_H
#define CEEGMANAGER_H

#include <QObject>
#include <QVector>

class QSerialPort;

class CEEGManager : public QObject
{
    Q_OBJECT
public:
    typedef struct tagEEGData {
        unsigned char poorSignal;
        unsigned char delta[3];
        unsigned char theta[3];
        unsigned char lowAlpha[3];
        unsigned char highAlpha[3];
        unsigned char lowBeta[3];
        unsigned char highBeta[3];
        unsigned char lowGamma[3];
        unsigned char midGamma[3];
        unsigned char attention;
        unsigned char mediation;
    }EEGData;

private:
    explicit CEEGManager(QObject *parent = 0);
    ~CEEGManager();
public:
    static CEEGManager *instance();
    void init(void);
private slots:
    void onReadyRead(void);
    void onTimeout(void);
signals:
    void newFrame(const CEEGManager::EEGData &eegData);
private:
    QSerialPort *serialPort;
    QVector<unsigned char> readingFIFO;
};

#endif // CEEGMANAGER_H
