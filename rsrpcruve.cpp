/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#define X 720
#define Y 450//pixmap 大小尺寸
#define XORIG 50//原点纵坐标         (50)
#define YORIG 30//原点横坐标         (30)
#define XLONG 600//长度
#define YLONG 360//Y轴长度
#define XDIVI 10//X轴平均分成10份
#define YDIVI 8//Y轴平均分成8份
#define TEXTWIDTH 10//10pix
#define YYLONGER 25//y轴比y刻度长20
#define XXLONGER 25//x轴比x刻度长20
#define ARROWXWIDTH 5//箭头x偏移量
#define ARROWYWIDTH 5//箭头y偏移量
#define THRESHOLD 110

#include "rsrpcruve.h"

//! [0] //! [1]
RsrpCruve::RsrpCruve(QWidget *parent)
//! [0] //! [2]
    : QWidget(parent)
//! [2] //! [3]
{
//! [3] //! [4]

    setGeometry(0, 0, X, Y);
    setFont(QFont("wqy-zenhei", 15, QFont::Normal));
    background = QBrush(QColor(255, 255, 255));

//    memset(array, 0, sizeof(array));
    memset(&cpit230Data, 0, sizeof(CPIT230));
    for(int i = 0; i < 11; i++)
    {
        listRSRP.append(-120);
    }

    myLines.clear();

    myBlackPen = QPen(Qt::black);
    myBlackPen.setWidth(2);
    myBlackPen.setStyle(Qt::SolidLine);
    myRedPen = QPen(Qt::red);
    myRedPen.setWidth(2);
    myRedPen.setStyle(Qt::SolidLine);
    myDashPen = QPen(Qt::black);
    myDashPen.setWidth(2);
    myDashPen.setStyle(Qt::DashLine);

    //    QTimer *timer = new QTimer(this);
//! [4] //! [5]
//    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//! [5] //! [6]
//    timer->start(100);
//! [6]

//    setWindowTitle(tr("SinrCruve"));
//    resize(X, Y);//X:720 Y:450

//! [7]
}

void RsrpCruve::updateCruve(CPIT230 newData)
{
    cpit230Data = newData;
    updateCruve();
}

void RsrpCruve::updateCruve()
{
//    cpit230Data = newData;
//    qDebug() << "cpit230Data.bDataValidFlag" << cpit230Data.bDataValidFlag;
    if(cpit230Data.bDataValidFlag)
    {
        if((cpit230Data.RSRP > -80) & (cpit230Data.RSRP != 0))
        {
            cpit230Data.RSRP = -80;
        }
        else if((cpit230Data.RSRP < -120) | (cpit230Data.RSRP == 0))
        {
            cpit230Data.RSRP = -120;
        }
    }
    else
    {
        cpit230Data.RSRP = -120;
    }
    listRSRP.prepend(cpit230Data.RSRP);
    listRSRP.removeLast();
//    int size = listRSRP.size();
//    qDebug() << "listRSRP.size = " << size;
//    qDebug() << "listRSRP = " << listRSRP;
    update();
}

//! [1] //! [7]

//! [8] //! [9]
void RsrpCruve::paintEvent(QPaintEvent *event)
//! [8] //! [10]
{
//    static const QPoint hourHand[3] = {
//        QPoint(7, 8),
//        QPoint(-7, 8),
//        QPoint(0, -40)
//    };
//    static const QPoint minuteHand[3] = {
//        QPoint(7, 8),
//        QPoint(-7, 8),
//        QPoint(0, -70)
//    };

//    QColor hourColor(127, 0, 127);
//    QColor minuteColor(0, 127, 127, 191);

//    int side = qMin(width(), height());
//    QTime time = QTime::currentTime();
//! [10]

//! [11]
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), background);

    painter.save();

    painter.setPen(myBlackPen);
//    painter.setPen(Qt::red);
//    painter.setBrush(Qt::red);
    painter.translate(XORIG, (Y - YORIG));//50,30
//    painter.translate(XORIG - width() / 2, (Y - YORIG) - height() / 2);//50,30

    painter.drawLine(0, 0, (XLONG + XXLONGER), 0);//X轴
    painter.drawLine(XLONG + ARROWXWIDTH, -ARROWYWIDTH, (XLONG + XXLONGER), 0);//画上部分-箭头
    painter.drawLine(XLONG + ARROWXWIDTH, ARROWYWIDTH, (XLONG + XXLONGER), 0);//画下部分-箭头

    painter.drawLine(0, 0, 0, -(YLONG + YYLONGER));//Y轴
    painter.drawLine(-ARROWXWIDTH, -(YLONG + ARROWYWIDTH), 0, -(YLONG + YYLONGER + ARROWYWIDTH));//左部分-箭头
    painter.drawLine(ARROWXWIDTH, -(YLONG + ARROWYWIDTH), 0, -(YLONG + YYLONGER + ARROWYWIDTH));//右部分-箭头

    int xdPix = XLONG/XDIVI;//600/10=60
    for(int i = 1; i <= XDIVI; i++)
        painter.drawLine(i*xdPix, 0, i*xdPix, -10);//画x轴的刻度
    int ydPix = YLONG/YDIVI;//360/8=20
    for(int i = 1; i <= YDIVI; i++)
        painter.drawLine(0, -i*ydPix, 10, -i*ydPix);//画y轴的刻度

    //完成X轴标尺数字的绘制
    for(int i = 0; i <= XDIVI; i++)
    {
        painter.drawText((xdPix*i - 5), 20, QString::number(i));
    }

    //完成Ｙ轴标尺数字的绘制
    int startP = YORIG - 25;//10
    for(int i = 0; i <= YDIVI; i++)
    {
        painter.drawText(-TEXTWIDTH*5, -(ydPix*i - startP), QString::number(-120 + i*5));
    }

    painter.drawText((XLONG + XXLONGER + 5), 5, QString(tr("t(s)")));
    painter.drawText(5, -(YLONG + 20), QString(tr("RSRP(dBm)")));

    //drawThresholdLine

    painter.setPen(myDashPen);
    int yValue = 5;
    painter.drawLine(0,((THRESHOLD - 120)*YLONG/yValue/YDIVI), XLONG, ((THRESHOLD - 120)*YLONG/yValue/YDIVI));

    //drawallpointers

    painter.setPen(myRedPen);
//    for(int i = 10; i > 0; i--)
//    {
//        array[i] = array[i-1];
//    }
//    array[0] = random()%40-10;
    int xPosition = 0;
    int yPosition = 0;
    int xOld = 0;
//    int yOld = -(array[0] + 10)*9;
    int yOld = -((listRSRP.first()) + 120)*9;
    QLine lineTemp;
    myLines.clear();
    for(int i = 1; i < 11; i++)
    {
        xPosition = xdPix*i;
//        yPosition = -(array[i] + 10)*9;
        yPosition = -(listRSRP.at(i) + 120)*9;
        lineTemp = QLine(xOld, yOld, xPosition, yPosition);
        xOld = xPosition;
        yOld = yPosition;
        myLines.append(lineTemp);
        painter.drawLines(myLines);
    }


    painter.restore();
/*
//! [11] //! [12]
//    painter.setRenderHint(QPainter::Antialiasing);
//! [12] //! [13]
    painter.translate(width() / 2, height() / 2);
//! [13] //! [14]
    painter.scale(side / 200.0, side / 200.0);
//! [9] //! [14]

//! [15]
    painter.setPen(Qt::NoPen);
//! [15] //! [16]
    painter.setBrush(hourColor);
//! [16]

//! [17] //! [18]
    painter.save();
//! [17] //! [19]
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();
//! [18] //! [19]

//! [20]
    painter.setPen(hourColor);
//! [20] //! [21]

    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }
//! [21]

//! [22]
    painter.setPen(Qt::NoPen);
//! [22] //! [23]
    painter.setBrush(minuteColor);

//! [24]
    painter.save();
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();
//! [23] //! [24]

//! [25]
    painter.setPen(minuteColor);
//! [25] //! [26]

//! [27]
    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }
//! [27]

*/
}
//! [26]
