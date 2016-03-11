/*
  COptions.h - Dobot client configuration files
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

#ifndef COPTIONS
#define COPTIONS

#include <QObject>

class COptions : public QObject
{
    Q_OBJECT
public:
    typedef struct tagJogParams {
        float jointVelocity;
        float jointAcceleration;
        float servoVelocity;
        float servoAcceleration;
        float linearVelocity;
        float linearAcceleration;

        float velocityRatio;
    }JogParams;
    typedef struct tagPlayBackParams {
        float jointVelocity;
        float jointAcceleration;
        float servoVelocity;
        float servoAcceleration;
        float linearVelocity;
        float linearAcceleration;
        float pauseTime;
        float jumpHeight;

        float velocityRatio;
        float accelerationRatio;
    }PlayBackParams;
private:
    explicit COptions(QObject *parent = 0);
    ~COptions();
    void loadJogParams(void);
    void saveJogParams(void);
    void loadPlayBackParams(void);
    void savePlayBackParams(void);
public:
    static COptions *instance(void);

    void setJogParams(JogParams *jogParams);
    void setJogVelocityRatio(float velocityRatio);

    void getJogParams(JogParams *jogParams);

    void setPlayBackParams(PlayBackParams *playBackParams);
    void setPlayBackVelocityParams(float velocityRatio);
    void setPlayBackAccelerationRatio(float accelerationRatio);

    void getPlayBackParams(PlayBackParams *playBackParams);
private:
    JogParams jogParams;
    PlayBackParams playBackParams;
};

#endif // COPTIONS
