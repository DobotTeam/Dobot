/*
  COptionsDialog.cpp - Dobot server configuration dialog
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

#include "COptionsDialog.h"
#include <QtWidgets>
#include "COptions.h"

COptionsDialog::COptionsDialog(QString baudrate, QWidget *parent)
  : QDialog(parent),
  baudrate(baudrate)
{
    setWindowTitle("Options");
    initLayout();
}

COptionsDialog::~COptionsDialog()
{

}

void COptionsDialog::initLayout(void)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    QLabel *label = new QLabel(this);
    layout->addWidget(label);
    label->setText("Baudrate");
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QComboBox *comboBox = new QComboBox(this);
    comboBox->setObjectName("baudrate");
    layout->addWidget(comboBox);
    comboBox->addItem("4800");
    comboBox->addItem("9600");
    comboBox->addItem("14400");
    comboBox->addItem("19200");
    comboBox->addItem("38400");
    comboBox->addItem("57600");
    comboBox->addItem("115200");
    comboBox->addItem("256000");

    comboBox->setCurrentIndex(comboBox->count() - 1);
    for (qint32 i = 0; i < comboBox->count(); i++) {
        if (baudrate == comboBox->itemText(i)) {
            comboBox->setCurrentIndex(i);
        }
    }
    connect(comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onCurrentTextChanged(QString)));

    QHBoxLayout *hLayout = new QHBoxLayout();
    layout->addLayout(hLayout);

    QPushButton *okBtn = new QPushButton(this);
    QPushButton *cancelBtn = new QPushButton(this);

    hLayout->addWidget(okBtn);
    hLayout->addWidget(cancelBtn);

    okBtn->setText("OK");
    cancelBtn->setText("Cancel");

    connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
}

void COptionsDialog::onCurrentTextChanged(const QString &text)
{
    baudrate = text;
}

QString COptionsDialog::getBaudrate(void) const
{
    return baudrate;
}
