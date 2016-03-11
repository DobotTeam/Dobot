/*
  CConfigJogDialog.cpp - Teach parameter config dialog
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

#include "CConfigJogDialog.h"
#include <QtWidgets>
#include "COptions.h"
#include "DobotClient.h"

CConfigJogDialog::CConfigJogDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Jog params");
    setFixedSize(412, 312);
    initLayout();
}

CConfigJogDialog::~CConfigJogDialog()
{

}

void CConfigJogDialog::initLayout(void)
{
    static const char *strLut[] = {
        "JointVelocity",
        "JointAcceleration",
        "ServoVelocity",
        "ServoAcceleration",
        "LinearVelocity",
        "LinearAcceleration",
        "VelocityRatio"
    };
    static const char *unitLut[] = {
        "°/s",
        "°/(s·s)",
        "°/s",
        "°/(s·s)",
        "mm/s",
        "mm/(s·s)",
        "%"
    };
    static const char *toolTipLut[] = {
        "Velocity of single joint",
        "Acceleration of single joint",
        "Velocity of the servo",
        "Acceleration of the servo",
        "Velocity of single axis",
        "Acceleration of single axis",
        "Ratio of the velocity"
    };

    QFont font("微软雅黑", 8);

    COptions::JogParams jogParams;
    COptions::instance()->getJogParams(&jogParams);
    float *params = (float *)&jogParams;

    int i = 0;
    for (i = 0; i < 6; i++) {
        QLabel *label = new QLabel(this);
        label->move(10, 10 + i * 40);
        label->resize(150, 30);
        label->setFont(font);
        label->setText(strLut[i]);

        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->move(180, 10 + i * 40);
        lineEdit->resize(100, 30);
        lineEdit->setFont(font);
        lineEdit->setToolTip(toolTipLut[i]);
        lineEdit->setText(QString("%1").arg(params[i]));
        paramWidgetList << lineEdit;

        label = new QLabel(this);
        label->move(300, 10 + i * 40);
        label->resize(50, 20);
        label->setFont(font);
        label->setText(unitLut[i]);
    }

    QPushButton *sendBtn = new QPushButton(this);
    sendBtn->move(280, 10 + i * 40);
    sendBtn->resize(120, 50);
    sendBtn->setFont(font);
    sendBtn->setText("Send");
    connect(sendBtn, SIGNAL(clicked()), this, SLOT(onSendBtnClicked()));
}

void CConfigJogDialog::onSendBtnClicked(void)
{
    COptions::JogParams jogParams;
    float *params = (float *)&jogParams;

    int i = 0;
    for (i = 0; i < 6; i++) {
        QLineEdit *lineEdit = (QLineEdit *)paramWidgetList.at(i);
        params[i] = lineEdit->text().toFloat();
    }

    COptions::instance()->setJogParams(&jogParams);
    DobotClient::instance()->configJogParams(
                jogParams.jointVelocity,
                jogParams.jointAcceleration,
                jogParams.servoVelocity,
                jogParams.servoAcceleration,
                jogParams.linearVelocity,
                jogParams.linearAcceleration
                );

    accept();
}
