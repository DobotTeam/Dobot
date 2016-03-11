/*
  COptionsDialog.h - Dobot server configuration dialog
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

#ifndef COPTIONSDIALOG_H
#define COPTIONSDIALOG_H

#include <QDialog>

class COptionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit COptionsDialog(QString baudrate, QWidget *parent);
    ~COptionsDialog();
public:
    QString getBaudrate(void) const;
private:
    void initLayout(void);
private slots:
    void onCurrentTextChanged(const QString &text);
private:
    QString baudrate;
};

#endif // COPTIONSDIALOG_H
