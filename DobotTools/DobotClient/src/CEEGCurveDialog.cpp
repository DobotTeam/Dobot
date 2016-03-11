/*
  CEEGCurveDialog.cpp - EEG curve dialog
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

#include "CEEGCurveDialog.h"
#include "qcustomplot.h"

CEEGCurveDialog::CEEGCurveDialog(QWidget *parent)
  : QDialog(parent)
{
    customPlot = new QCustomPlot(this);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);

    customPlot->setFixedSize(1024, 1024 * 0.618);
    customPlot->setMinimumSize(800, 480);

    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("微软雅黑", 12));

    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(Qt::red));
    customPlot->graph(0)->setName("Noise");
    customPlot->graph(0)->setBrush(QBrush(QColor(245, 145, 138)));
    customPlot->graph(0)->setAntialiasedFill(false);
    customPlot->addGraph(); // red line
    customPlot->graph(1)->setPen(QPen(Qt::green));
    customPlot->graph(1)->setName("Attention");
    customPlot->graph(1)->setBrush(QBrush(QColor(145, 195, 138)));
    //customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));

    customPlot->addGraph(); // blue line
    customPlot->graph(2)->setPen(QPen(Qt::blue));
    customPlot->graph(2)->setName("Mediation");
    customPlot->graph(2)->setBrush(QBrush(QColor(145, 145, 238)));

    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(2);
    customPlot->axisRect()->setupFullAxesBox();

    customPlot->yAxis->setRange(0, 210);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    connect(CEEGManager::instance(), SIGNAL(newFrame(CEEGManager::EEGData)), this, SLOT(onNewFrame(CEEGManager::EEGData)));
}

CEEGCurveDialog::~CEEGCurveDialog()
{

}

void CEEGCurveDialog::onNewFrame(const CEEGManager::EEGData &eegData)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;

    customPlot->graph(0)->addData(key, eegData.poorSignal);
    customPlot->graph(1)->addData(key, eegData.attention);
    customPlot->graph(2)->addData(key, eegData.mediation);
    customPlot->graph(0)->removeDataBefore(key - 16);
    customPlot->graph(1)->removeDataBefore(key - 16);
    customPlot->graph(1)->removeDataBefore(key - 16);

    customPlot->xAxis->setRange(key + 0.25, 16, Qt::AlignRight);
    customPlot->replot();
}
