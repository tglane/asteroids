

#ifndef __MYELLIPSE_HPP__
#define __MYELLIPSE_HPP__

#include <QGraphicsItem>
#include <QObject>
#include <QPainter>

#include <iostream>

namespace strategy
{
class MyEllipse : public QGraphicsItem
{
public:
    MyEllipse(int x, int y);

    QRectF boundingRect() const;

    // overriding paint()
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);

    
protected:
    // overriding mouse events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    int x;
    int y;
};

}

#endif  //MYELLIPSE_HPP