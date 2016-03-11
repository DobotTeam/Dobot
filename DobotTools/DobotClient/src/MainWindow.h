/*
  MainWindow.h - Dobot client main window
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include "DobotClient.h"
#include "CEEGManager.h"

class QTableWidget;
class CEEGCurveDialog;

#define PLAYBACK_ENABLE    1

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void initJogUI(QWidget *widget);
    void initPlayBackUI(QWidget *widget);
    void insertPlayBackRow(const DobotClient::PlayBackPoint &playBackPoint);
    void getPlayBackPoints(QVector<DobotClient::PlayBackPoint> &playBackPoints);
private slots:
    void onJogModeChanged(int index);
    void onJogCtrlBtnPressed(int index);
    void onJogCtrlBtnReleased(void);
    void onJogVelocityRatioChanged(int value);

    void onEndTypeChanged(int index);
    void onGrabToggled(bool checked);
    void onLaserToggled(bool checked);
    void onJogConfigBtnClicked(void);
    void onUserDefBtnClicked(void);
    void onEEGBtnClicked(void);
    void onEEGCurveDlgFinished(void);
    void onEEGNewFrame(const CEEGManager::EEGData &eegData);

    void insertPlayBackRow(void);
    void removePlayBackRow(void);
    void onPlayBackRatioChanged(void);

    void onSendAllBtnClicked(bool force = false);
    void onStopBtnClicked(void);

    void onPlayBackConfigBtnClicked(void);

    void onNewFrame(const QVector<unsigned char> &frame);
protected:
    void keyPressEvent(QKeyEvent *keyEvent);
private:
    QWidget *widget[2];
    CEEGCurveDialog *eegCurveDlg;
};

#endif // MAINWINDOW_H
