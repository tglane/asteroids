/*
 * MyEllipse.cpp
 */
#include "view2D/MyEllipse.hpp"

namespace strategy
{    

MyEllipse::MyEllipse(int x, int y)
{
    this->x = x;
    this->y = y;
    setFlag(ItemIsMovable);
}

QRectF MyEllipse::boundingRect() const
{
    // outer most edges
    return QRectF(x/25,y/25,20,20);
}

void MyEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

    
    QPen pen(Qt::red, 3);
    painter->setPen(pen);
    painter->drawEllipse(rect);
    
}

void MyEllipse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    std::cout << "Planet geklickt" << std::endl;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void MyEllipse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

}