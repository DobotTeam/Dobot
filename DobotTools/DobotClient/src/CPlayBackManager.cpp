/*
  CPlayBackManager.cpp - Playback manager
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

#include "CPlayBackManager.h"
#include <QFile>
#include <QDomDocument>
#include <QApplication>
#include <QTextStream>

CPlayBackManager::CPlayBackManager(QObject *parent) : QObject(parent)
{

}

CPlayBackManager::~CPlayBackManager()
{

}

CPlayBackManager *CPlayBackManager::instance()
{
    static CPlayBackManager *instance = 0;

    if (instance == 0) {
        instance = new CPlayBackManager(0);
    }
    return instance;
}

void CPlayBackManager::store(const QVector<DobotClient::PlayBackPoint> &playBackPoints)
{
    QString fileName;
#ifdef QT_DEBUG
    fileName = QString("%1/../../DobotClient/file/PlayBackPoints.xml").arg(qApp->applicationDirPath());
#else
    fileName = QString("%1/file/PlayBackPoints.xml").arg(qApp->applicationDirPath());
#endif
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate) == false) {
        return;
    }
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version='1.0' encoding='UTF-8'");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("playBack");
    doc.appendChild(root);

    static const char *motionStyleLut[] = {"JUMP", "MOVJ", "MOVL"};
    foreach(DobotClient::PlayBackPoint playBackPoint, playBackPoints) {
        QDomElement point = doc.createElement("point");
        root.appendChild(point);

        QDomElement motionStyle = doc.createElement("motionStyle");
        QDomText _motionStyle = doc.createTextNode(motionStyleLut[playBackPoint.motionStyle]);
        motionStyle.appendChild(_motionStyle);
        point.appendChild(motionStyle);

        QDomElement isGrab = doc.createElement("isGrab");
        QDomText _isGrab = doc.createTextNode(playBackPoint.isGrab ? "true" : "false");
        isGrab.appendChild(_isGrab);
        point.appendChild(isGrab);

        QDomElement baseAngle = doc.createElement("x");
        QDomText _baseAngle = doc.createTextNode(QString("%1").arg(playBackPoint.baseAngle));
        baseAngle.appendChild(_baseAngle);
        point.appendChild(baseAngle);

        QDomElement longArmAngle = doc.createElement("y");
        QDomText _longArmAngle = doc.createTextNode(QString("%1").arg(playBackPoint.longArmAngle));
        longArmAngle.appendChild(_longArmAngle);
        point.appendChild(longArmAngle);

        QDomElement shortArmAngle = doc.createElement("z");
        QDomText _shortArmAngle = doc.createTextNode(QString("%1").arg(playBackPoint.shortArmAngle));
        shortArmAngle.appendChild(_shortArmAngle);
        point.appendChild(shortArmAngle);

        QDomElement pawAngle = doc.createElement("rHead");
        QDomText _pawAngle = doc.createTextNode(QString("%1").arg(playBackPoint.pawAngle));
        pawAngle.appendChild(_pawAngle);
        point.appendChild(pawAngle);

        QDomElement gripper = doc.createElement("gripper");
        QDomText _gripper = doc.createTextNode(QString("%1").arg(playBackPoint.gripper));
        gripper.appendChild(_gripper);
        point.appendChild(gripper);

        QDomElement pauseTime = doc.createElement("pauseTime");
        QDomText _pauseTime = doc.createTextNode(QString("%1").arg(playBackPoint.pauseTime));
        pauseTime.appendChild(_pauseTime);
        point.appendChild(pauseTime);
    }
    QTextStream out(&file);
    doc.save(out, 4);
}

void CPlayBackManager::restore(QVector<DobotClient::PlayBackPoint> &playBackPoints)
{
    QString fileName;
#ifdef QT_DEBUG
    fileName = QString("%1/../../DobotClient/file/PlayBackPoints.xml").arg(qApp->applicationDirPath());
#else
    fileName = QString("%1/file/PlayBackPoints.xml").arg(qApp->applicationDirPath());
#endif
    QFile file(fileName);

    playBackPoints.clear();

    if (file.open(QIODevice::ReadOnly) == false) {
        return;
    }
    QDomDocument doc;
    if (doc.setContent(&file) == false) {
        return;
    }
    QDomElement root = doc.documentElement();
    for (QDomElement child = root.firstChildElement("point"); !child.isNull(); child = child.nextSiblingElement("point")) {
        QDomElement motionStyle = child.firstChildElement("motionStyle");
        QString _motionStyle = motionStyle.text();
        QDomElement isGrab = child.firstChildElement("isGrab");
        QDomElement baseAngle = child.firstChildElement("x");
        QDomElement longArmAngle = child.firstChildElement("y");
        QDomElement shortArmAngle = child.firstChildElement("z");
        QDomElement pawAngle = child.firstChildElement("rHead");
        QDomElement gripper = child.firstChildElement("gripper");
        QDomElement pauseTime = child.firstChildElement("pauseTime");

        DobotClient::PlayBackPoint playBackPoint;
        memset(&playBackPoint, 0, sizeof(playBackPoint));
        if (_motionStyle == QString("JUMP")) {
            playBackPoint.motionStyle = DobotClient::JUMP;
        } else if (_motionStyle == QString("MOVJ")) {
            playBackPoint.motionStyle = DobotClient::MOVJ;
        } else if (_motionStyle == QString("MOVL")) {
            playBackPoint.motionStyle = DobotClient::MOVL;
        }
        playBackPoint.isGrab = isGrab.text() == QString("true");
        playBackPoint.baseAngle = baseAngle.text().toFloat();
        playBackPoint.longArmAngle = longArmAngle.text().toFloat();
        playBackPoint.shortArmAngle = shortArmAngle.text().toFloat();
        playBackPoint.pawAngle = pawAngle.text().toFloat();
        playBackPoint.gripper = gripper.text().toFloat();
        playBackPoint.pauseTime = pauseTime.text().toFloat();

        playBackPoints.push_back(playBackPoint);
    }
}
