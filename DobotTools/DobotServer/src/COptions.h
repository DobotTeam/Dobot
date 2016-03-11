/*
  COptions.h - Dobot server configuration files
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

#ifndef COPTIONS_H
#define COPTIONS_H

#include <QObject>

class COptions : public QObject
{
    Q_OBJECT
private:
    explicit COptions(QObject *parent = 0);
    ~COptions();
    void loadConfig(void);
    void saveConfig(void);
public:
    static COptions *instance(void);

    void setBaudrate(const QString &baudrate);
    QString getBaudrate(void);
private:
    QString baudrate;
};

#endif // COPTIONS
