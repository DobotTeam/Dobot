/*
  CSelectSerialDialog.h - Select serial dialog
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

#include "CSelectSerialDialog.h"
#include <QtWidgets>
#include <QSerialPortInfo>

CSelectSerialDialog::CSelectSerialDialog(QWidget *parent)
  : QDialog(parent),
    selectedSerial("")
{
    setWindowTitle("Serial port not found");
    initLayout();
}

CSelectSerialDialog::~CSelectSerialDialog()
{

}

void CSelectSerialDialog::initLayout(void)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QLabel *label = new QLabel(this);
    layout->addWidget(label);
    label->setText("Serial port not found!\nSelect other serial ports?");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QComboBox *comboBox = new QComboBox(this);
    comboBox->setObjectName("SerialList");
    layout->addWidget(comboBox);
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    foreach(QSerialPortInfo portInfo, availablePorts) {
        comboBox->addItem(portInfo.portName());
    }
    selectedSerial = comboBox->currentText();

    QHBoxLayout *hLayout = new QHBoxLayout();
    layout->addLayout(hLayout);

    QPushButton *okBtn = new QPushButton(this);
    QPushButton *cancelBtn = new QPushButton(this);

    hLayout->addWidget(okBtn);
    hLayout->addWidget(cancelBtn);

    okBtn->setText("OK");
    cancelBtn->setText("Cancel");
    if (comboBox->count() == 0) {
        okBtn->setEnabled(false);
    }

    connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
}

void CSelectSerialDialog::onCurrentIndexChanged(const QString &text)
{
    selectedSerial = text;
}

QString CSelectSerialDialog::getSelectedSerial(void)
{
    return selectedSerial;
}
