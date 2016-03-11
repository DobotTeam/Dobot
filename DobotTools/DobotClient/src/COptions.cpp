/*
  COptions.cpp - Dobot client configuration files
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

#include "COptions.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

COptions::COptions(QObject *parent)
  : QObject(parent)
{
    jogParams.jointVelocity = 15;
    jogParams.jointAcceleration = 50;
    jogParams.servoVelocity = 30;
    jogParams.servoAcceleration = 10;
    jogParams.linearVelocity = 40;
    jogParams.linearAcceleration = 40;
    jogParams.velocityRatio = 50;

    playBackParams.jointVelocity = 200;
    playBackParams.jointAcceleration = 200;
    playBackParams.servoVelocity = 200;
    playBackParams.servoAcceleration = 200;
    playBackParams.linearVelocity = 800;
    playBackParams.linearAcceleration = 1000;
    playBackParams.pauseTime = 100;
    playBackParams.jumpHeight = 20;

    playBackParams.velocityRatio = 30;
    playBackParams.accelerationRatio = 30;

    loadJogParams();
    loadPlayBackParams();
}

COptions::~COptions()
{

}

void COptions::loadJogParams(void)
{
    QString fileName;
#ifdef QT_DEBUG
    fileName = QString("%1/../../DobotClient/file/JogParams.json").arg(qApp->applicationDirPath());
#else
    fileName = QString("%1/file/JogParams.json").arg(qApp->applicationDirPath());
#endif
    qDebug() << fileName;
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly) == false) {
        if (file.open(QIODevice::WriteOnly) == false) {
            qCritical() << QString("Failed to open %1").arg(fileName);
            return;
        }
        qDebug() << "Resetting config file";
        // Init the structure
        QJsonObject obj;
        obj["jointVelocity"] = QJsonValue(jogParams.jointVelocity);
        obj["jointAcceleration"] = QJsonValue(jogParams.jointAcceleration);
        obj["servoVelocity"] = QJsonValue(jogParams.servoVelocity);
        obj["servoAcceleration"] = QJsonValue(jogParams.servoAcceleration);
        obj["linearVelocity"] = QJsonValue(jogParams.linearVelocity);
        obj["linearAcceleration"] = QJsonValue(jogParams.linearAcceleration);
        obj["velocityRatio"] = QJsonValue(jogParams.velocityRatio);

        QJsonDocument saveDoc(obj);
        file.write(saveDoc.toJson());
        file.close();
        file.open(QIODevice::ReadOnly);
    }
    QJsonDocument loadDoc(QJsonDocument::fromJson(file.readAll()));
    const QJsonObject &obj = loadDoc.object();

    jogParams.jointVelocity = obj["jointVelocity"].toDouble();
    jogParams.jointAcceleration = obj["jointAcceleration"].toInt();
    jogParams.servoVelocity = obj["servoVelocity"].toDouble();
    jogParams.servoAcceleration = obj["servoAcceleration"].toDouble();
    jogParams.linearVelocity = obj["linearVelocity"].toDouble();
    jogParams.linearAcceleration = obj["linearAcceleration"].toDouble();
    jogParams.velocityRatio = obj["velocityRatio"].toDouble();

    file.close();
}

void COptions::saveJogParams(void)
{
    QString fileName;
#ifdef QT_DEBUG
    fileName = QString("%1/../../DobotClient/file/JogParams.json").arg(qApp->applicationDirPath());
#else
    fileName = QString("%1/file/JogParams.json").arg(qApp->applicationDirPath());
#endif
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly) == false) {
        qCritical() << "Failed to open file!";
        return;
    }
    QJsonObject obj;
    obj["jointVelocity"] = QJsonValue(jogParams.jointVelocity);
    obj["jointAcceleration"] = QJsonValue(jogParams.jointAcceleration);
    obj["servoVelocity"] = QJsonValue(jogParams.servoVelocity);
    obj["servoAcceleration"] = QJsonValue(jogParams.servoAcceleration);
    obj["linearVelocity"] = QJsonValue(jogParams.linearVelocity);
    obj["linearAcceleration"] = QJsonValue(jogParams.linearAcceleration);
    obj["velocityRatio"] = QJsonValue(jogParams.velocityRatio);

    QJsonDocument saveDoc(obj);
    file.write(saveDoc.toJson());
    file.close();
}

void COptions::loadPlayBackParams(void)
{
    QString fileName;
#ifdef QT_DEBUG
    fileName = QString("%1/../../DobotClient/file/PlayBackParams.json").arg(qApp->applicationDirPath());
#else
    fileName = QString("%1/file/PlayBackParams.json").arg(qApp->applicationDirPath());
#endif
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly) == false) {
        if (file.open(QIODevice::WriteOnly) == false) {
            qCritical() << QString("Failed to open %1").arg(fileName);
            return;
        }
        qDebug() << "Resetting config file";
        // Init the structure
        QJsonObject obj;
        obj["jointVelocity"] = QJsonValue(playBackParams.jointVelocity);
        obj["jointAcceleration"] = QJsonValue(playBackParams.jointAcceleration);
        obj["servoVelocity"] = QJsonValue(playBackParams.servoVelocity);
        obj["servoAcceleration"] = QJsonValue(playBackParams.servoAcceleration);
        obj["linearVelocity"] = QJsonValue(playBackParams.linearVelocity);
        obj["linearAcceleration"] = QJsonValue(playBackParams.linearAcceleration);
        obj["pauseTime"] = QJsonValue(playBackParams.pauseTime);
        obj["jumpHeight"] = QJsonValue(playBackParams.jumpHeight);
        obj["velocityRatio"] = QJsonValue(playBackParams.velocityRatio);
        obj["accelerationRatio"] = QJsonValue(playBackParams.accelerationRatio);

        QJsonDocument saveDoc(obj);
        file.write(saveDoc.toJson());
        file.close();
        file.open(QIODevice::ReadOnly);
    }
    QJsonDocument loadDoc(QJsonDocument::fromJson(file.readAll()));
    const QJsonObject &obj = loadDoc.object();

    playBackParams.jointVelocity = obj["jointVelocity"].toDouble();
    playBackParams.jointAcceleration = obj["jointAcceleration"].toDouble();
    playBackParams.servoVelocity = obj["servoVelocity"].toDouble();
    playBackParams.servoAcceleration = obj["servoAcceleration"].toDouble();
    playBackParams.linearVelocity = obj["linearVelocity"].toDouble();
    playBackParams.linearAcceleration = obj["linearAcceleration"].toDouble();
    playBackParams.pauseTime = obj["pauseTime"].toDouble();
    playBackParams.jumpHeight = obj["jumpHeight"].toDouble();
    playBackParams.velocityRatio = obj["velocityRatio"].toDouble();
    playBackParams.accelerationRatio = obj["accelerationRatio"].toDouble();

    file.close();
}

void COptions::savePlayBackParams(void)
{
    QString fileName;
#ifdef QT_DEBUG
    fileName = QString("%1/../../DobotClient/file/PlayBackParams.json").arg(qApp->applicationDirPath());
#else
    fileName = QString("%1/file/PlayBackParams.json").arg(qApp->applicationDirPath());
#endif
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly) == false) {
        qCritical() << "Failed to open file!";
        return;
    }
    QJsonObject obj;
    obj["jointVelocity"] = QJsonValue(playBackParams.jointVelocity);
    obj["jointAcceleration"] = QJsonValue(playBackParams.jointAcceleration);
    obj["servoVelocity"] = QJsonValue(playBackParams.servoVelocity);
    obj["servoAcceleration"] = QJsonValue(playBackParams.servoAcceleration);
    obj["linearVelocity"] = QJsonValue(playBackParams.linearVelocity);
    obj["linearAcceleration"] = QJsonValue(playBackParams.linearAcceleration);
    obj["pauseTime"] = QJsonValue(playBackParams.pauseTime);
    obj["jumpHeight"] = QJsonValue(playBackParams.jumpHeight);
    obj["velocityRatio"] = QJsonValue(playBackParams.velocityRatio);
    obj["accelerationRatio"] = QJsonValue(playBackParams.accelerationRatio);

    QJsonDocument saveDoc(obj);
    file.write(saveDoc.toJson());
    file.close();
}

COptions *COptions::instance(void)
{
    static COptions *instance = 0;

    if (instance == 0) {
        instance = new COptions();
    }
    return instance;
}

void COptions::setJogParams(JogParams *jogParams)
{
    float velocityRatio = this->jogParams.velocityRatio;
    this->jogParams = *jogParams;
    this->jogParams.velocityRatio = velocityRatio;

    saveJogParams();
}

void COptions::setJogVelocityRatio(float velocityRatio)
{
    this->jogParams.velocityRatio = velocityRatio;

    saveJogParams();
}

void COptions::getJogParams(JogParams *jogParams)
{
    *jogParams = this->jogParams;
}

void COptions::setPlayBackParams(PlayBackParams *playBackParams)
{
    float velocityRatio = this->playBackParams.velocityRatio;
    float accelerationRatio = this->playBackParams.accelerationRatio;

    this->playBackParams = *playBackParams;
    this->playBackParams.velocityRatio = velocityRatio;
    this->playBackParams.accelerationRatio = accelerationRatio;

    savePlayBackParams();
}

void COptions::setPlayBackVelocityParams(float velocityRatio)
{
    this->playBackParams.velocityRatio = velocityRatio;

    savePlayBackParams();
}

void COptions::setPlayBackAccelerationRatio(float accelerationRatio)
{
    this->playBackParams.accelerationRatio = accelerationRatio;

    savePlayBackParams();
}

void COptions::getPlayBackParams(PlayBackParams *playBackParams)
{
    *playBackParams = this->playBackParams;
}
