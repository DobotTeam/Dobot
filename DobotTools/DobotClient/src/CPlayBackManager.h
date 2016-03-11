/*
  CPlayBackManager.h - Playback manager
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

#ifndef CPLAYBACKMANAGER_H
#define CPLAYBACKMANAGER_H

#include <QObject>
#include "DobotClient.h"
#include <QVector>

class CPlayBackManager : public QObject
{
    Q_OBJECT
private:
    explicit CPlayBackManager(QObject *parent = 0);
    ~CPlayBackManager();
public:
    static CPlayBackManager *instance();
    void store(const QVector<DobotClient::PlayBackPoint> &playBackPoints);
    void restore(QVector<DobotClient::PlayBackPoint> &playBackPoints);

};

#endif // CPLAYBACKMANAGER_H
