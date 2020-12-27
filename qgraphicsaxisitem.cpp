#include "qgraphicsaxisitem.h"

QGraphicsAxisItem::QGraphicsAxisItem(char mode, qreal pix_min, qreal pix_max, int NumofTicks, qreal TickIncrement, qreal TickStart)
{
    pix_low = pix_min;
    pix_high = pix_max;
    this->mode = mode;
    tick_number = NumofTicks;
    start_tick = TickStart;
    tick_increment = TickIncrement;
    d_pix = (pix_high-pix_low)/(double)tick_number;
}

QRectF QGraphicsAxisItem::boundingRect() const
{
    QRectF outerRect;

    qreal length = pix_high-pix_low;

    if (mode == 'x') outerRect.setRect(pix_low-30, -50, length+60, 100);
    else outerRect.setRect(-100, -(pix_high+60), 100, length+60);

    return outerRect;
}

void QGraphicsAxisItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    qreal current_value = pix_low;
    qreal current_tick = start_tick;
    QString text;

    if (mode == 'x'){

        painter->drawLine(pix_low, 0, pix_high, 0);
        //draw arrow at the end:
        painter->drawLine(pix_high-15, 15, pix_high, 0);
        painter->drawLine(pix_high-15, -15, pix_high, 0);

        for (int i = 0; i < tick_number; i++){
            painter->drawLine(current_value, 0, current_value, -5); // the tick has a length of 5 pixels
            text = QString::number(current_tick, 'f',1);
            painter->drawText(current_value-5, 20, text);
            current_value += d_pix;
            current_tick += tick_increment;
        }
    }
    else{
        painter->rotate(180);
        painter->drawLine(0,pix_low,0, pix_high);
        //draw arrow at the end:
        painter->drawLine(15, pix_high-15, 0, pix_high);
        painter->drawLine(-15, pix_high-15, 0, pix_high);

        for (int i = 0; i < tick_number; i++){
            painter->drawLine(0, current_value, 5,  current_value); // the tick has a length of 5 pixels
            text = QString::number(current_tick, 'f',1);
            painter->rotate(-180);
            painter->drawText(-30, -current_value,text);
            painter->rotate(180);
            current_value += d_pix;
            current_tick += tick_increment;

    }
    }
}

qreal QGraphicsAxisItem::Pixels2Data()
{
    return d_pix / tick_increment;
}
