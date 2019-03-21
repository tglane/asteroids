/**
 * MyEllipse.hpp
 * 
 * @brief   Custom Class that is derived from QGraphicsItem for representation of planets
 * 
 */

#ifndef __MYELLIPSE_HPP__
#define __MYELLIPSE_HPP__

#include <QGraphicsItem>
#include <QObject>
#include <QPainter>

#include <iostream>

namespace strategy
{
class MyEllipse : public QGraphicsObject
{
    Q_OBJECT
public:
    MyEllipse(int x, int y);

    QRectF boundingRect() const;

    // overriding paint()
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);

signals:
    // signal that is emited when a planet is clicked
    void show_planetInfo(int id);
    
protected:
    // overriding mouse events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    int x;
    int y;
    QBrush outer_brush;
    QBrush inner_brush;
};

}

#endif  //MYELLIPSE_HPP