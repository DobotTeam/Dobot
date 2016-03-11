/*
  COptions.cpp - Dobot server configuration files
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
  : QObject(parent),
    baudrate(QString(256000))
{
    loadConfig();
}

COptions::~COptions()
{

}

void COptions::loadConfig(void)
{
    QString fileName;
#ifdef QT_DEBUG
    fileName = QString("%1/../../DobotServer/file/config.json").arg(qApp->applicationDirPath());
#else
    fileName = QString("%1/file/config.json").arg(qApp->applicationDirPath());
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
        obj["baudrate"] = QJsonValue(baudrate);
        QJsonDocument saveDoc(obj);
        file.write(saveDoc.toJson());
        file.close();
        file.open(QIODevice::ReadOnly);
    }
    QJsonDocument loadDoc(QJsonDocument::fromJson(file.readAll()));
    const QJsonObject &obj = loadDoc.object();
    baudrate = obj["baudrate"].toString();
    if (baudrate == QString("")) {
        baudrate = QString("9600");
    }
    file.close();
}

void COptions::saveConfig(void)
{
    QString fileName;
#ifdef QT_DEBUG
    fileName = QString("%1/../../DobotServer/file/config.json").arg(qApp->applicationDirPath());
#else
    fileName = QString("%1/file/config.json").arg(qApp->applicationDirPath());
#endif
    fileName.arg(qApp->applicationDirPath());
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly) == false) {
        qCritical() << "Failed to open file!";
        return;
    }
    QJsonObject obj;
    obj["baudrate"] = QJsonValue(baudrate);

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

void COptions::setBaudrate(const QString &baudrate)
{
    this->baudrate = baudrate;
    saveConfig();
}

QString COptions::getBaudrate(void)
{
    return baudrate;
}
