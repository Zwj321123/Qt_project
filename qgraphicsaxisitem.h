#ifndef QGRAPHICSAXISITEM_H
#define QGRAPHICSAXISITEM_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QString>

class QGraphicsAxisItem : public QGraphicsItem
{
private:
    qreal pix_low, pix_high;

    char mode;
    int tick_number;
    qreal start_tick;
    qreal tick_increment;

public:
    qreal d_pix; //number of pixels in a given interval
    QGraphicsAxisItem(char mode, qreal pix_min, qreal pix_max, int NumofTicks, qreal TickIncrement, qreal TickStart);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;
    qreal Pixels2Data();
};

#endif // QGRAPHICSAXISITEM_H
