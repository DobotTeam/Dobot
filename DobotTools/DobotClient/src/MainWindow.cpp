/*
  MainWindow.cpp - Dobot client main window
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
#include <QDebug>
#include "DobotClient.h"
#include "CConfigJogDialog.h"
#include "CConfigPlayBackDialog.h"
#include "CConfigUserDefDialog.h"
#include "CPlayBackManager.h"
#include "COptions.h"
#include <QProcess>
#include <QApplication>
#include "CEEGCurveDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{
#ifndef QT_DEBUG
    // Start the process
    QProcess *process = new QProcess(this);
    process->setProgram(QString("%1/DobotServer.exe").arg(qApp->applicationDirPath()));
    process->start();

    connect(qApp, SIGNAL(aboutToQuit()), process, SLOT(kill()));
#endif

    widget[0] = new QWidget();
    widget[1] = new QWidget();
    widget[0]->setFixedSize(1300, 401);
    widget[1]->setFixedSize(1300, 401);
    initJogUI(widget[0]);
    initPlayBackUI(widget[1]);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(widget[0]);
    layout->addWidget(widget[1]);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

    QPushButton *button = widget[0]->findChild<QPushButton *>("moreBtn");
    connect(button, SIGNAL(toggled(bool)), widget[1], SLOT(setVisible(bool)));

    connect(DobotClient::instance(), SIGNAL(newFrame(QVector<unsigned char>)),
            this, SLOT(onNewFrame(QVector<unsigned char>)));

    // Init the parameters!
    CConfigJogDialog jogParams;
    jogParams.onSendBtnClicked();
    CConfigPlayBackDialog playBackParams;
    playBackParams.onSendBtnClicked();

    // Init the parameters
    eegCurveDlg = 0;

    // Init the EEG manager
    CEEGManager::instance()->init();

    connect(CEEGManager::instance(), SIGNAL(newFrame(CEEGManager::EEGData)),
            this, SLOT(onEEGNewFrame(CEEGManager::EEGData)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::initJogUI(QWidget *widget)
{
    QGroupBox *group = new QGroupBox("Teach", widget);
    group->move(10, 10);
    group->resize(1280, 380);
    group->setStyleSheet("QGroupBox{border:1px solid #303030;border-radius:5px;}");

    int baseX = 10 + 10, baseY = 10 + 30;
    QFont font(QFont("微软雅黑", 8));

    QGroupBox *jogGroup = new QGroupBox("JogMode", widget);
    jogGroup->move(baseX + 10, baseY);
    jogGroup->resize(680 + 65, 320);
    jogGroup->setStyleSheet("QGroupBox{border:1px solid #303030;border-radius:5px;}");

    QComboBox *jogMode = new QComboBox(widget);
    jogMode->setObjectName("jogMode");
    jogMode->move(baseX + 50, baseY + 30);
    jogMode->resize(180, 50);
    jogMode->addItem("Axis");
    jogMode->addItem("Linear");

    // The buttons
    QPushButton *baseAngleAddBtn = new QPushButton(widget);
    baseAngleAddBtn->setObjectName("baseAngleAddBtn");
    baseAngleAddBtn->move(baseX + 300, baseY + 30);
    baseAngleAddBtn->resize(180, 50);

    QPushButton *baseAngleSubBtn = new QPushButton(widget);
    baseAngleSubBtn->setObjectName("baseAngleSubBtn");
    baseAngleSubBtn->move(baseX + 520, baseY + 30);
    baseAngleSubBtn->resize(180, 50);

    QPushButton *longArmAddBtn = new QPushButton(widget);
    longArmAddBtn->setObjectName("longArmAddBtn");
    longArmAddBtn->move(baseX + 300, baseY + 90);
    longArmAddBtn->resize(180, 50);

    QPushButton *longArmSubBtn = new QPushButton(widget);
    longArmSubBtn->setObjectName("longArmSubBtn");
    longArmSubBtn->move(baseX + 520, baseY + 90);
    longArmSubBtn->resize(180, 50);

    QPushButton *shortArmAddBtn = new QPushButton(widget);
    shortArmAddBtn->setObjectName("shortArmAddBtn");
    shortArmAddBtn->move(baseX + 300, baseY + 150);
    shortArmAddBtn->resize(180, 50);

    QPushButton *shortArmSubBtn = new QPushButton(widget);
    shortArmSubBtn->setObjectName("shortArmSubBtn");
    shortArmSubBtn->move(baseX + 520, baseY + 150);
    shortArmSubBtn->resize(180, 50);

    QPushButton *rHeadAddBtn = new QPushButton(widget);
    rHeadAddBtn->setObjectName("rHeadAddBtn");
    rHeadAddBtn->move(baseX + 300, baseY + 210);
    rHeadAddBtn->resize(180, 50);

    QPushButton *rHeadSubBtn = new QPushButton(widget);
    rHeadSubBtn->setObjectName("rHeadSubBtn");
    rHeadSubBtn->move(baseX + 520, baseY + 210);
    rHeadSubBtn->resize(180, 50);

    COptions::JogParams jogParams;
    COptions::instance()->getJogParams(&jogParams);

    QLabel *label = new QLabel(widget);
    label->move(baseX + 300, baseY + 270);
    label->resize(120, 30);
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label->setText("VelocityRatio");

    QSlider *slider = new QSlider(widget);
    slider->move(baseX + 430, baseY + 270);
    slider->resize(180, 30);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(1, 100);
    slider->setValue((int)jogParams.velocityRatio);

    QSpinBox *spinBox = new QSpinBox(widget);
    spinBox->move(baseX + 620, baseY + 270);
    spinBox->resize(80, 30);
    spinBox->setRange(1, 100);
    spinBox->setValue((int)jogParams.velocityRatio);

    connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
    connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onJogVelocityRatioChanged(int)));

    // End type
    QGroupBox *endTypeGroup = new QGroupBox("EndType", widget);
    endTypeGroup->move(baseX + 775, baseY);
    endTypeGroup->resize(330, 320);
    endTypeGroup->setStyleSheet("QGroupBox{border:1px solid #303030;border-radius:5px;}");

    QComboBox *endType = new QComboBox(widget);
    endType->setObjectName("endType");
    endType->move(baseX + 855, baseY + 30);
    endType->resize(180, 50);
    endType->addItem("Pump");
    endType->addItem("Gripper");
    endType->addItem("Laser");

    // Grip
    QCheckBox *grab = new QCheckBox(widget);
    grab->setObjectName("grab");
    grab->move(baseX + 855, baseY + 100);
    grab->resize(180, 20);
    grab->setText("Grab");
    grab->setFont(font);
    grab->setChecked(false);

    // Laser
    QCheckBox *laser = new QCheckBox(widget);
    laser->setObjectName("laser");
    laser->move(baseX + 855, baseY + 130);
    laser->resize(180, 20);
    laser->setText("Laser");
    laser->setFont(font);
    laser->setChecked(false);

    QPushButton *gripperAddBtn = new QPushButton(widget);
    gripperAddBtn->setObjectName("gripperAddBtn");
    gripperAddBtn->move(baseX + 855, baseY + 170);
    gripperAddBtn->resize(180, 50);
    gripperAddBtn->setText("Gripper+");

    QPushButton *gripperSubBtn = new QPushButton(widget);
    gripperSubBtn->setObjectName("gripperSubBtn");
    gripperSubBtn->move(baseX + 855, baseY + 240);
    gripperSubBtn->resize(180, 50);
    gripperSubBtn->setText("Gripper-");

    // Config button
    QPushButton *configBtn = new QPushButton(widget);
    configBtn->setObjectName("configBtn");
    configBtn->move(baseX + 1125, baseY);
    configBtn->resize(130, 50);
    configBtn->setFont(font);
    configBtn->setText("Config...");

    // UserDef button
    QPushButton *userDefBtn = new QPushButton(widget);
    userDefBtn->setObjectName("userDefBtn");
    userDefBtn->move(baseX + 1125, baseY + 60);
    userDefBtn->resize(130, 50);
    userDefBtn->setFont(font);
    userDefBtn->setText("UserDef...");

    // EEG button
    QPushButton *eegBtn = new QPushButton(widget);
    eegBtn->setObjectName("eegBtn");
    eegBtn->move(baseX + 1125, baseY + 120);
    eegBtn->resize(130, 50);
    eegBtn->setFont(font);
    eegBtn->setText("EEG...");

    // More button
    QPushButton *moreBtn = new QPushButton(widget);
    moreBtn->setObjectName("moreBtn");
    moreBtn->setCheckable(true);
    moreBtn->setChecked(true);
    moreBtn->move(baseX + 1125, baseY + 320 - 50);
    moreBtn->resize(130, 50);
    moreBtn->setFont(font);
    moreBtn->setText("More");

    jogMode->setFocus();
    // Init the connects
    connect(jogMode, SIGNAL(currentIndexChanged(int)), this, SLOT(onJogModeChanged(int)));
    onJogModeChanged(0);

    QSignalMapper *signalMapper = new QSignalMapper(this);
    connect(baseAngleAddBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(baseAngleSubBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(longArmAddBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(longArmSubBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(shortArmAddBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(shortArmSubBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(rHeadAddBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(rHeadSubBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));

    connect(gripperAddBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(gripperSubBtn, SIGNAL(pressed()), signalMapper, SLOT(map()));

    signalMapper->setMapping(baseAngleAddBtn, 0);
    signalMapper->setMapping(baseAngleSubBtn, 1);
    signalMapper->setMapping(longArmAddBtn, 2);
    signalMapper->setMapping(longArmSubBtn, 3);
    signalMapper->setMapping(shortArmAddBtn, 4);
    signalMapper->setMapping(shortArmSubBtn, 5);
    signalMapper->setMapping(rHeadAddBtn, 6);
    signalMapper->setMapping(rHeadSubBtn, 7);
    signalMapper->setMapping(gripperAddBtn, 8);
    signalMapper->setMapping(gripperSubBtn, 9);

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(onJogCtrlBtnPressed(int)));

    connect(baseAngleAddBtn, SIGNAL(released()), this, SLOT(onJogCtrlBtnReleased()));
    connect(baseAngleSubBtn, SIGNAL(released()), this, SLOT(onJogCtrlBtnReleased()));
    connect(longArmAddBtn, SIGNAL(released()), this, SLOT(onJogCtrlBtnReleased()));
    connect(longArmSubBtn, SIGNAL(released()), this, SLOT(onJogCtrlBtnReleased()));
    connect(shortArmAddBtn, SIGNAL(released()), this, SLOT(onJogCtrlBtnReleased()));
    connect(shortArmSubBtn, SIGNAL(released()), this, SLOT(onJogCtrlBtnReleased()));
    connect(rHeadAddBtn, SIGNAL(released()), this, SLOT(onJogCtrlBtnReleased()));
    connect(rHeadSubBtn, SIGNAL(released()), this, SLOT(onJogCtrlBtnReleased()));
    connect(gripperAddBtn, SIGNAL(released()), this, SLOT(onJogCtrlBtnReleased()));
    connect(gripperSubBtn, SIGNAL(released()), this, SLOT(onJogCtrlBtnReleased()));
    onJogCtrlBtnReleased();

    connect(endType, SIGNAL(currentIndexChanged(int)), this, SLOT(onEndTypeChanged(int)));
    onEndTypeChanged(0);

    connect(grab, SIGNAL(toggled(bool)), this, SLOT(onGrabToggled(bool)));
    connect(laser, SIGNAL(toggled(bool)), this, SLOT(onLaserToggled(bool)));

    connect(configBtn, SIGNAL(clicked()), this, SLOT(onJogConfigBtnClicked()));

    connect(userDefBtn, SIGNAL(clicked()), this, SLOT(onUserDefBtnClicked()));

    connect(eegBtn, SIGNAL(clicked()), this, SLOT(onEEGBtnClicked()));
}

void MainWindow::initPlayBackUI(QWidget *widget)
{
    QGroupBox *group = new QGroupBox("PlayBack", widget);
    group->move(10, 8);
    group->resize(1280, 380);
    group->setStyleSheet("QGroupBox{border:1px solid #303030;border-radius:5px;}");

    int baseX = 10 + 10, baseY = 8 + 30;
    QFont font(QFont("微软雅黑", 8));

    // X
    QLabel *xLabel = new QLabel(widget);
    xLabel->move(baseX + 35 + 5, baseY + 5);
    xLabel->resize(25, 30);
    xLabel->setFont(font);
    xLabel->setAlignment(Qt::AlignCenter);
    xLabel->setText("X:");

    QLineEdit *x = new QLineEdit(widget);
    x->setObjectName("x");
    x->move(baseX + 35 + 30, baseY + 5);
    x->resize(100, 30);
    x->setFont(font);
    x->setAlignment(Qt::AlignCenter);
    x->setReadOnly(true);
    x->setStyleSheet("background-color:#e0e0e0");
    x->setText("0");

    // Y
    QLabel *yLabel = new QLabel(widget);
    yLabel->move(baseX + 35 + 5 + 1 * 130, baseY + 5);
    yLabel->resize(25, 30);
    yLabel->setFont(font);
    yLabel->setAlignment(Qt::AlignCenter);
    yLabel->setText("Y:");

    QLineEdit *y = new QLineEdit(widget);
    y->setObjectName("y");
    y->move(baseX + 35 + 30 + 1 * 130, baseY + 5);
    y->resize(100, 30);
    y->setFont(font);
    y->setAlignment(Qt::AlignCenter);
    y->setReadOnly(true);
    y->setStyleSheet("background-color:#e0e0e0");
    y->setText("0");

    // Z
    QLabel *zLabel = new QLabel(widget);
    zLabel->move(baseX + 35 + 5 + 2 * 130, baseY + 5);
    zLabel->resize(25, 30);
    zLabel->setFont(font);
    zLabel->setAlignment(Qt::AlignCenter);
    zLabel->setText("Z:");

    QLineEdit *z = new QLineEdit(widget);
    z->setObjectName("z");
    z->move(baseX + 35 + 30 + 2 * 130, baseY + 5);
    z->resize(100, 30);
    z->setFont(font);
    z->setAlignment(Qt::AlignCenter);
    z->setReadOnly(true);
    z->setStyleSheet("background-color:#e0e0e0");
    z->setText("0");

    // RHead
    QLabel *rHeadLabel = new QLabel(widget);
    rHeadLabel->move(baseX + 35 + 5 + 3 * 130, baseY + 5);
    rHeadLabel->resize(65, 30);
    rHeadLabel->setFont(font);
    rHeadLabel->setAlignment(Qt::AlignCenter);
    rHeadLabel->setText("RHead:");

    QLineEdit *rHead = new QLineEdit(widget);
    rHead->setObjectName("rHead");
    rHead->move(baseX + 35 + 70 + 3 * 130, baseY + 5);
    rHead->resize(100, 30);
    rHead->setFont(font);
    rHead->setAlignment(Qt::AlignCenter);
    rHead->setReadOnly(true);
    rHead->setStyleSheet("background-color:#e0e0e0");
    rHead->setText("0");

    // MotionStyle
    QComboBox *motionStyle = new QComboBox(widget);
    motionStyle->setObjectName("motionStyle");
    motionStyle->move(baseX + 35, baseY + 40);
    motionStyle->resize(130, 30);
    motionStyle->setFont(font);
    motionStyle->addItem("JUMP");
    motionStyle->addItem("MOVJ");
    motionStyle->addItem("MOVL");
    // IsGrab
    QLineEdit *isGrab = new QLineEdit(widget);
    isGrab->setObjectName("isGrab");
    isGrab->move(baseX + 35 + 1 * 130, baseY + 40);
    isGrab->resize(130, 30);
    isGrab->setFont(font);
    isGrab->setAlignment(Qt::AlignCenter);
    isGrab->setReadOnly(true);
    isGrab->setStyleSheet("background-color:#e0e0e0");
    isGrab->setText("false");
    // BaseAngle
    QLineEdit *baseAngle = new QLineEdit(widget);
    baseAngle->setObjectName("baseAngle");
    baseAngle->move(baseX + 35 + 2 * 130, baseY + 40);
    baseAngle->resize(130, 30);
    baseAngle->setFont(font);
    baseAngle->setAlignment(Qt::AlignCenter);
    baseAngle->setReadOnly(true);
    baseAngle->setStyleSheet("background-color:#e0e0e0");
    baseAngle->setText("0");
    // LongArmAngle
    QLineEdit *longArmAngle = new QLineEdit(widget);
    longArmAngle->setObjectName("longArmAngle");
    longArmAngle->move(baseX + 35 + 3 * 130, baseY + 40);
    longArmAngle->resize(130, 30);
    longArmAngle->setFont(font);
    longArmAngle->setAlignment(Qt::AlignCenter);
    longArmAngle->setReadOnly(true);
    longArmAngle->setStyleSheet("background-color:#e0e0e0");
    longArmAngle->setText("0");
    // ShortArmAngle
    QLineEdit *shortArmAngle = new QLineEdit(widget);
    shortArmAngle->setObjectName("shortArmAngle");
    shortArmAngle->move(baseX + 35 + 4 * 130, baseY + 40);
    shortArmAngle->resize(130, 30);
    shortArmAngle->setFont(font);
    shortArmAngle->setAlignment(Qt::AlignCenter);
    shortArmAngle->setReadOnly(true);
    shortArmAngle->setStyleSheet("background-color:#e0e0e0");
    shortArmAngle->setText("0");
    // PawAngle
    QLineEdit *pawAngle = new QLineEdit(widget);
    pawAngle->setObjectName("pawAngle");
    pawAngle->move(baseX + 35 + 5 * 130, baseY + 40);
    pawAngle->resize(130, 30);
    pawAngle->setFont(font);
    pawAngle->setAlignment(Qt::AlignCenter);
    pawAngle->setReadOnly(true);
    pawAngle->setStyleSheet("background-color:#e0e0e0");
    pawAngle->setText("0");
    // Gripper
    QLineEdit *gripper = new QLineEdit(widget);
    gripper->setObjectName("gripper");
    gripper->move(baseX + 35 + 6 * 130, baseY + 40);
    gripper->resize(130, 30);
    gripper->setFont(font);
    gripper->setAlignment(Qt::AlignCenter);
    gripper->setReadOnly(true);
    gripper->setStyleSheet("background-color:#e0e0e0");
    gripper->setText("0");
    // PauseTime
    QLineEdit *pauseTime = new QLineEdit(widget);
    pauseTime->setObjectName("pauseTime");
    pauseTime->move(baseX + 35 + 7 * 130, baseY + 40);
    pauseTime->resize(130, 30);
    pauseTime->setFont(font);
    pauseTime->setAlignment(Qt::AlignCenter);
    pauseTime->setText("0");

    // Save
    QPushButton *saveBtn = new QPushButton(widget);
    saveBtn->setObjectName("saveBtn");
    saveBtn->move(baseX + 35 + 8 * 130 + 50, baseY + 75);
    saveBtn->resize(130, 50);
    saveBtn->setFont(font);
    saveBtn->setText("Save");

    // SendAll
    QPushButton *sendAllBtn = new QPushButton(widget);
    sendAllBtn->setObjectName("sendAllBtn");
    sendAllBtn->move(baseX + 35 + 8 * 130 + 50, baseY + 130);
    sendAllBtn->resize(130, 50);
    sendAllBtn->setFont(font);
    sendAllBtn->setText("SendAll");

    // Stop
    QPushButton *stopBtn = new QPushButton(widget);
    stopBtn->setObjectName("stopBtn");
    stopBtn->move(baseX + 35 + 8 * 130 + 50, baseY + 185);
    stopBtn->resize(130, 50);
    stopBtn->setFont(font);
    stopBtn->setText("Stop");

    // Config
    QPushButton *configBtn = new QPushButton(widget);
    configBtn->setObjectName("configBtn");
    configBtn->move(baseX + 35 + 8 * 130 + 50, baseY + 275);
    configBtn->resize(130, 50);
    configBtn->setFont(font);
    configBtn->setText("Config...");

    QTableWidget *playBackTable = new QTableWidget(widget);
    playBackTable->setObjectName("playBackTable");
    playBackTable->move(baseX + 5, baseY + 75);
    playBackTable->resize(1100, 220);
    playBackTable->setColumnCount(8);
    QStringList header;
    header << "MotionStyle" << "IsGrab";
#if PLAYBACK_ENABLE == 1
    header << "Joint1" << "Joint2" << "Joint3" << "Joint4";
#else
    header << "X" << "Y" << "Z" << "RHead";
#endif
    header << "Gripper" << "PauseTime";
    playBackTable->setHorizontalHeaderLabels(header);
    playBackTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    playBackTable->setSelectionMode(QAbstractItemView::SingleSelection);
    playBackTable->setFocusPolicy(Qt::StrongFocus);

    for (int i = 0; i < 8; i++) {
        playBackTable->setColumnWidth(i, 130);
    }
    playBackTable->horizontalHeader()->setFont(QFont("微软雅黑", 8));
    playBackTable->horizontalHeader()->setStyleSheet("QHeaderView::section{border:1px solid #808080;color:blue;}");

    playBackTable->verticalHeader()->setFont(QFont("微软雅黑", 8));
    playBackTable->verticalHeader()->setStyleSheet("QHeaderView::section{border:1px solid #808080;color:blue;}");
    playBackTable->verticalHeader()->setFixedWidth(30);
    playBackTable->setGridStyle(Qt::SolidLine);
    playBackTable->setFont(QFont("微软雅黑", 8));

    COptions::PlayBackParams playBackParams;
    COptions::instance()->getPlayBackParams(&playBackParams);

    QLabel *label = new QLabel(widget);
    label->move(baseX + 5, baseY + 305);
    label->resize(120, 30);
    label->setText("VelocityRatio");

    // VelocityRatio
    QSlider *slider = new QSlider(widget);
    slider->setObjectName("VelocityRatio");
    slider->move(baseX + 135, baseY + 305);
    slider->resize(180, 30);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(1, 100);
    slider->setValue((int)playBackParams.velocityRatio);

    QSpinBox *spinBox = new QSpinBox(widget);
    spinBox->move(baseX + 325, baseY + 305);
    spinBox->resize(80, 30);
    spinBox->setRange(1, 100);
    spinBox->setValue((int)playBackParams.velocityRatio);

    connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
    connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onPlayBackRatioChanged()));

    // AccelerationRatio
    label = new QLabel(widget);
    label->move(baseX + 425, baseY + 305);
    label->resize(160, 30);
    label->setText("AccelerationRatio");

    slider = new QSlider(widget);
    slider->setObjectName("AccelerationRatio");
    slider->move(baseX + 600, baseY + 305);
    slider->resize(180, 30);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(1, 100);
    slider->setValue((int)playBackParams.accelerationRatio);

    spinBox = new QSpinBox(widget);
    spinBox->move(baseX + 790, baseY + 305);
    spinBox->resize(80, 30);
    spinBox->setRange(1, 100);
    spinBox->setValue((int)playBackParams.accelerationRatio);

    connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
    connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onPlayBackRatioChanged()));

    // Restore the playback points!
    QVector<DobotClient::PlayBackPoint> playBackPoints;
    CPlayBackManager::instance()->restore(playBackPoints);
    foreach(DobotClient::PlayBackPoint playBackPoint, playBackPoints) {
        insertPlayBackRow(playBackPoint);
    }

    // Init the connects
    connect(sendAllBtn, SIGNAL(clicked()), this, SLOT(onSendAllBtnClicked()));
    connect(stopBtn, SIGNAL(clicked()), this, SLOT(onStopBtnClicked()));

    connect(saveBtn, SIGNAL(clicked()), this, SLOT(insertPlayBackRow()));
    connect(configBtn, SIGNAL(clicked()), this, SLOT(onPlayBackConfigBtnClicked()));
}

void MainWindow::insertPlayBackRow(const DobotClient::PlayBackPoint &playBackPoint)
{
    QTableWidget *playBackTable = widget[1]->findChild<QTableWidget *>("playBackTable");

    int rowCount = playBackTable->rowCount();
    playBackTable->insertRow(rowCount);

    // MotionStyle
    QComboBox *motionStyle = new QComboBox(this);
    motionStyle->setFocusPolicy(Qt::NoFocus);
    motionStyle->addItem("JUMP");
    motionStyle->addItem("MOVJ");
    motionStyle->addItem("MOVL");
    motionStyle->setCurrentIndex(playBackPoint.motionStyle);
    playBackTable->setCellWidget(rowCount, 0, motionStyle);
    // IsGrap
    QComboBox *isGrab = new QComboBox(this);
    isGrab->addItem("No");
    isGrab->addItem("Yes");
    isGrab->setCurrentIndex(playBackPoint.isGrab);
    playBackTable->setCellWidget(rowCount, 1, isGrab);
    // BaseAngle
    QLineEdit *baseAngle = new QLineEdit(this);
    baseAngle->setAlignment(Qt::AlignCenter);
    baseAngle->setText(QString("%1").arg(playBackPoint.baseAngle));
    playBackTable->setCellWidget(rowCount, 2, baseAngle);
    // LongArmAngle
    QLineEdit *longArmAngle = new QLineEdit(this);
    longArmAngle->setAlignment(Qt::AlignCenter);
    longArmAngle->setText(QString("%1").arg(playBackPoint.longArmAngle));
    playBackTable->setCellWidget(rowCount, 3, longArmAngle);
    // ShortArmAngle
    QLineEdit *shortArmAngle = new QLineEdit(this);
    shortArmAngle->setAlignment(Qt::AlignCenter);
    shortArmAngle->setText(QString("%1").arg(playBackPoint.shortArmAngle));
    playBackTable->setCellWidget(rowCount, 4, shortArmAngle);
    // PawAngle
    QLineEdit *pawAngle = new QLineEdit(this);
    pawAngle->setAlignment(Qt::AlignCenter);
    pawAngle->setText(QString("%1").arg(playBackPoint.pawAngle));
    playBackTable->setCellWidget(rowCount, 5, pawAngle);
    // Gripper
    QLineEdit *gripper = new QLineEdit(this);
    gripper->setAlignment(Qt::AlignCenter);
    gripper->setText(QString("%1").arg(playBackPoint.gripper));
    playBackTable->setCellWidget(rowCount, 6, gripper);
    // PauseTime
    QLineEdit *pauseTime = new QLineEdit(this);
    pauseTime->setAlignment(Qt::AlignCenter);
    pauseTime->setText(QString("%1").arg(playBackPoint.pauseTime));
    playBackTable->setCellWidget(rowCount, 7, pauseTime);

    playBackTable->scrollToBottom();
}

void MainWindow::getPlayBackPoints(QVector<DobotClient::PlayBackPoint> &playBackPoints)
{
    playBackPoints.clear();
    QTableWidget *playBackTable = widget[1]->findChild<QTableWidget *>("playBackTable");
    int rowCount = playBackTable->rowCount();

    for (int j = 0; j < rowCount; j++) {
        QComboBox *motionStyle = (QComboBox *)playBackTable->cellWidget(j, 0);
        QComboBox *isGrab = (QComboBox *)playBackTable->cellWidget(j, 1);
        QLineEdit *baseAngle = (QLineEdit *)playBackTable->cellWidget(j, 2);
        QLineEdit *longArmAngle = (QLineEdit *)playBackTable->cellWidget(j, 3);
        QLineEdit *shortArmAngle = (QLineEdit *)playBackTable->cellWidget(j, 4);
        QLineEdit *pawAngle = (QLineEdit *)playBackTable->cellWidget(j, 5);
        QLineEdit *gripper = (QLineEdit *)playBackTable->cellWidget(j, 6);
        QLineEdit *pauseTime = (QLineEdit *)playBackTable->cellWidget(j, 7);

        DobotClient::PlayBackPoint playBackPoint;
        memset(&playBackPoint, 0, sizeof(playBackPoint));
        playBackPoint.motionStyle = (DobotClient::MotionStyle)motionStyle->currentIndex();
        playBackPoint.isGrab = isGrab->currentIndex() == 1;
        playBackPoint.baseAngle = baseAngle->text().toFloat();
        playBackPoint.longArmAngle = longArmAngle->text().toFloat();
        playBackPoint.shortArmAngle = shortArmAngle->text().toFloat();
        playBackPoint.pawAngle = pawAngle->text().toFloat();
        playBackPoint.gripper = gripper->text().toFloat();
        playBackPoint.pauseTime = pauseTime->text().toFloat();

        playBackPoints.push_back(playBackPoint);
    }
}

void MainWindow::onJogModeChanged(int index)
{
    QPushButton *baseAngleAddBtn = widget[0]->findChild<QPushButton *>("baseAngleAddBtn");
    QPushButton *baseAngleSubBtn = widget[0]->findChild<QPushButton *>("baseAngleSubBtn");
    QPushButton *longArmAddBtn = widget[0]->findChild<QPushButton *>("longArmAddBtn");
    QPushButton *longArmSubBtn = widget[0]->findChild<QPushButton *>("longArmSubBtn");
    QPushButton *shortArmAddBtn = widget[0]->findChild<QPushButton *>("shortArmAddBtn");
    QPushButton *shortArmSubBtn = widget[0]->findChild<QPushButton *>("shortArmSubBtn");
    QPushButton *rHeadAddBtn = widget[0]->findChild<QPushButton *>("rHeadAddBtn");
    QPushButton *rHeadSubBtn = widget[0]->findChild<QPushButton *>("rHeadSubBtn");

    if (index == 0) {
        baseAngleAddBtn->setText("Joint1+");
        baseAngleSubBtn->setText("Joint1-");
        longArmAddBtn->setText("Joint2+");
        longArmSubBtn->setText("Joint2-");
        shortArmAddBtn->setText("Joint3+");
        shortArmSubBtn->setText("Joint3-");
        rHeadAddBtn->setText("Joint4+");
        rHeadSubBtn->setText("Joint4-");
    } else {
        baseAngleAddBtn->setText("X+");
        baseAngleSubBtn->setText("X-");
        longArmAddBtn->setText("Y+");
        longArmSubBtn->setText("Y-");
        shortArmAddBtn->setText("Z+");
        shortArmSubBtn->setText("Z-");
        rHeadAddBtn->setText("R+");
        rHeadSubBtn->setText("R-");
    }
    DobotClient::instance()->setJogMode(index);
}

void MainWindow::onJogCtrlBtnPressed(int index)
{
    switch (index) {
        case 0:
            DobotClient::instance()->addAxis1();
        break;
        case 1:
            DobotClient::instance()->subAxis1();
        break;
        case 2:
            DobotClient::instance()->addAxis2();
        break;
        case 3:
            DobotClient::instance()->subAxis2();
        break;
        case 4:
            DobotClient::instance()->addAxis3();
        break;
        case 5:
            DobotClient::instance()->subAxis3();
        break;
        case 6:
            DobotClient::instance()->addAxis4();
        break;
        case 7:
            DobotClient::instance()->subAxis4();
        break;
        case 8:
            DobotClient::instance()->addAxis5();
        break;
        case 9:
            DobotClient::instance()->subAxis5();
        break;
    }
}

void MainWindow::onJogCtrlBtnReleased(void)
{
    DobotClient::instance()->stop();
}

void MainWindow::onJogVelocityRatioChanged(int value)
{
    COptions::instance()->setJogVelocityRatio(value);
    DobotClient::instance()->setVelocityRatio(value);
}

void MainWindow::onEndTypeChanged(int index)
{
    QCheckBox *grab = widget[0]->findChild<QCheckBox *>("grab");
    QCheckBox *laser = widget[0]->findChild<QCheckBox *>("laser");
    QPushButton *gripperAddBtn = widget[0]->findChild<QPushButton *>("gripperAddBtn");
    QPushButton *gripperSubBtn = widget[0]->findChild<QPushButton *>("gripperSubBtn");

    switch (index) {
        case 0:
            grab->setEnabled(true);
            gripperAddBtn->setEnabled(false);
            gripperSubBtn->setEnabled(false);
            laser->setEnabled(false);
        break;
        case 1:
            grab->setEnabled(false);
            gripperAddBtn->setEnabled(true);
            gripperSubBtn->setEnabled(true);
            laser->setEnabled(false);
        break;
        case 2:
            grab->setEnabled(false);
            gripperAddBtn->setEnabled(false);
            gripperSubBtn->setEnabled(false);
            laser->setEnabled(true);
        break;
    }
}

void MainWindow::onGrabToggled(bool checked)
{
    DobotClient::instance()->enableGrab(checked);
}

void MainWindow::onLaserToggled(bool checked)
{
    DobotClient::instance()->enableLaser(checked);
}

void MainWindow::onJogConfigBtnClicked(void)
{
    CConfigJogDialog dlg;
    dlg.exec();
}

void MainWindow::onUserDefBtnClicked(void)
{
    CConfigUserDefDialog dlg;
    dlg.exec();
}

void MainWindow::onEEGBtnClicked(void)
{
    eegCurveDlg = new CEEGCurveDialog(this);
    connect(eegCurveDlg, SIGNAL(finished(int)), this, SLOT(onEEGCurveDlgFinished()));
    eegCurveDlg->show();
}

void MainWindow::onEEGCurveDlgFinished(void)
{
    eegCurveDlg->deleteLater();
    eegCurveDlg = 0;
}

void MainWindow::onEEGNewFrame(const CEEGManager::EEGData &eegData)
{
    if (eegCurveDlg == 0) {
        return;
    }
    if (eegData.poorSignal == 0 and
        eegData.attention > 90) {
        onSendAllBtnClicked(true);
    }
}

void MainWindow::insertPlayBackRow(void)
{
    DobotClient::PlayBackPoint playBackPoint;

    memset(&playBackPoint, 0, sizeof(playBackPoint));

    QComboBox *motionStyle = widget[1]->findChild<QComboBox *>("motionStyle");
    playBackPoint.motionStyle = (DobotClient::MotionStyle)motionStyle->currentIndex();
    QLineEdit *isGrab = widget[1]->findChild<QLineEdit *>("isGrab");
    playBackPoint.isGrab = isGrab->text() == QString("Yes");
#if PLAYBACK_ENABLE == 1
    QLineEdit *baseAngle = widget[1]->findChild<QLineEdit *>("baseAngle");
    playBackPoint.baseAngle = baseAngle->text().toFloat();

    QLineEdit *longArmAngle = widget[1]->findChild<QLineEdit *>("longArmAngle");
    playBackPoint.longArmAngle = longArmAngle->text().toFloat();

    QLineEdit *shortArmAngle = widget[1]->findChild<QLineEdit *>("shortArmAngle");
    playBackPoint.shortArmAngle = shortArmAngle->text().toFloat();

    QLineEdit *pawAngle = widget[1]->findChild<QLineEdit *>("pawAngle");
    playBackPoint.pawAngle = pawAngle->text().toFloat();
#else
    QLineEdit *baseAngle = widget[1]->findChild<QLineEdit *>("x");
    playBackPoint.baseAngle = baseAngle->text().toFloat();

    QLineEdit *longArmAngle = widget[1]->findChild<QLineEdit *>("y");
    playBackPoint.longArmAngle = longArmAngle->text().toFloat();

    QLineEdit *shortArmAngle = widget[1]->findChild<QLineEdit *>("z");
    playBackPoint.shortArmAngle = shortArmAngle->text().toFloat();

    QLineEdit *pawAngle = widget[1]->findChild<QLineEdit *>("rHead");
    playBackPoint.pawAngle = pawAngle->text().toFloat();
#endif

    QLineEdit *gripper = widget[1]->findChild<QLineEdit *>("gripper");
    playBackPoint.gripper = gripper->text().toFloat();

    QLineEdit *pauseTime = widget[1]->findChild<QLineEdit *>("pauseTime");
    playBackPoint.pauseTime = pauseTime->text().toFloat();

    insertPlayBackRow(playBackPoint);

    QVector<DobotClient::PlayBackPoint> playBackPoints;
    getPlayBackPoints(playBackPoints);
    CPlayBackManager::instance()->store(playBackPoints);
}

void MainWindow::removePlayBackRow(void)
{
    QTableWidget *playBackTable = widget[1]->findChild<QTableWidget *>("playBackTable");
    if (not playBackTable) {
        return;
    }
    playBackTable->removeRow(playBackTable->currentRow());

    QVector<DobotClient::PlayBackPoint> playBackPoints;
    getPlayBackPoints(playBackPoints);
    CPlayBackManager::instance()->store(playBackPoints);
}

void MainWindow::onPlayBackRatioChanged(void)
{
    QSlider *slider1 = widget[1]->findChild<QSlider *>("VelocityRatio");
    QSlider *slider2 = widget[1]->findChild<QSlider *>("AccelerationRatio");
    float params1 = slider1->value();
    float params2 = slider2->value();

    COptions::instance()->setPlayBackVelocityParams(params1);
    COptions::instance()->setPlayBackAccelerationRatio(params2);
    DobotClient::instance()->configPlayBackDynamicParams(params1, params2);
}

void MainWindow::onSendAllBtnClicked(bool force)
{
    QTableWidget *playBackTable = widget[1]->findChild<QTableWidget *>("playBackTable");
    int rowCount = playBackTable->rowCount();

    if (rowCount < 1) {
        QMessageBox::warning(this, "Warning", "No enough points!!!");
        return;
    }
    bool ok;
    int loopTimes;

    if (force) {
        ok = true;
        loopTimes = 1;
    } else {
        loopTimes = QInputDialog::getInt(this, "Set loop times", "Loop times", 1, 1, 100000, 1, &ok, Qt::Dialog);
    }
    if (ok) {
        for (int i = 0; i < loopTimes; i++) {
            QVector<DobotClient::PlayBackPoint> playBackPoints;
            getPlayBackPoints(playBackPoints);
            foreach(DobotClient::PlayBackPoint playBackPoint, playBackPoints) {
#if PLAYBACK_ENABLE == 1
                DobotClient::instance()->playBack(
                            playBackPoint.motionStyle,
                            playBackPoint.isGrab,
                            playBackPoint.baseAngle,
                            playBackPoint.longArmAngle,
                            playBackPoint.shortArmAngle,
                            playBackPoint.pawAngle,
                            playBackPoint.gripper,
                            playBackPoint.pauseTime);
#else
                DobotClient::instance()->gotoXYZ(
                            playBackPoint.motionStyle,
                            playBackPoint.isGrab,
                            playBackPoint.baseAngle,
                            playBackPoint.longArmAngle,
                            playBackPoint.shortArmAngle,
                            playBackPoint.pawAngle,
                            playBackPoint.gripper,
                            playBackPoint.pauseTime);
#endif
            }
        }
    }
}

void MainWindow::onStopBtnClicked(void)
{
    DobotClient::instance()->stop();
}

void MainWindow::onPlayBackConfigBtnClicked(void)
{
    CConfigPlayBackDialog dlg;
    dlg.exec();
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() != Qt::Key_Delete) {
        return;
    }
    removePlayBackRow();
}

void MainWindow::onNewFrame(const QVector<unsigned char> &frame)
{
    float *params = (float *)frame.data();

    QLineEdit *x = widget[1]->findChild<QLineEdit *>("x");
    x->setText(QString("%1").arg(params[0]));
    QLineEdit *y = widget[1]->findChild<QLineEdit *>("y");
    y->setText(QString("%1").arg(params[1]));
    QLineEdit *z = widget[1]->findChild<QLineEdit *>("z");
    z->setText(QString("%1").arg(params[2]));
    QLineEdit *rHead = widget[1]->findChild<QLineEdit *>("rHead");
    rHead->setText(QString("%1").arg(params[3]));

    QLineEdit *baseAngle = widget[1]->findChild<QLineEdit *>("baseAngle");
    baseAngle->setText(QString("%1").arg(params[4]));
    QLineEdit *longArmAngle = widget[1]->findChild<QLineEdit *>("longArmAngle");
    longArmAngle->setText(QString("%1").arg(params[5]));
    QLineEdit *shortArmAngle = widget[1]->findChild<QLineEdit *>("shortArmAngle");
    shortArmAngle->setText(QString("%1").arg(params[6]));
    QLineEdit *pawAngle = widget[1]->findChild<QLineEdit *>("pawAngle");
    pawAngle->setText(QString("%1").arg(params[7]));

    QLineEdit *isGrab = widget[1]->findChild<QLineEdit *>("isGrab");
    if (params[8] != 0) {
        isGrab->setText("Yes");
    } else {
        isGrab->setText("No");
    }

    QLineEdit *gripper = widget[1]->findChild<QLineEdit *>("gripper");
    gripper->setText(QString("%1").arg(params[9]));
}
