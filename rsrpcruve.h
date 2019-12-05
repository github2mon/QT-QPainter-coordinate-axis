#ifndef RSRPCRUVE_H
#define RSRPCRUVE_H

#include <QWidget>
#include <QPen>
#include <QDebug>
#include <QLine>
#include <QVector>
#include <QList>
#include <QPalette>
#include <QBrush>

#include "structmodel230data.h"
#include "mydefine.h"

class RsrpCruve : public QWidget
{
    Q_OBJECT

public:
    RsrpCruve(QWidget *parent = 0);
    void updateCruve(CPIT230);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QVector<QLine>  myLines;
    QPen            myBlackPen;
    QPen            myRedPen;
    QPen            myDashPen;
    CPIT230         cpit230Data;
    QList<int>      listRSRP;
    QBrush          background;

    void updateCruve();
};
#endif //RSRPCRUVE_H
