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

RsrpCruve::RsrpCruve(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(0, 0, X, Y);
    setFont(QFont("wqy-zenhei", 15, QFont::Normal));
    background = QBrush(QColor(255, 255, 255));

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
}

void RsrpCruve::updateCruve(CPIT230 newData)
{
    cpit230Data = newData;
    updateCruve();
}

void RsrpCruve::updateCruve()
{
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
    update();
}

void RsrpCruve::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), background);

    painter.save();

    painter.setPen(myBlackPen);
    painter.translate(XORIG, (Y - YORIG));//50,30

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
    int xPosition = 0;
    int yPosition = 0;
    int xOld = 0;
    int yOld = -((listRSRP.first()) + 120)*9;
    QLine lineTemp;
    myLines.clear();
    for(int i = 1; i < 11; i++)
    {
        xPosition = xdPix*i;
        yPosition = -(listRSRP.at(i) + 120)*9;
        lineTemp = QLine(xOld, yOld, xPosition, yPosition);
        xOld = xPosition;
        yOld = yPosition;
        myLines.append(lineTemp);
        painter.drawLines(myLines);
    }
    painter.restore();
}
