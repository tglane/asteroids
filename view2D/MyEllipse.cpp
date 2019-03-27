/**
 * MyEllipse.cpp
 */
#include "view2D/MyEllipse.hpp"

namespace strategy
{    

MyEllipse::MyEllipse(int x, int y)
{
    this->x = x;
    this->y = y;
    QPixmap pix("../resources/surface/neutral1.jpg");
    myBrush = QBrush(pix);
    myPen = QPen(Qt::black,1);
}

QRectF MyEllipse::boundingRect() const
{
    // outer most edges
    return QRectF(x,y,20,20);
}

void MyEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    QPainterPath myPath;
    myPath.addEllipse(rect);
    painter->setBrush(myBrush);
    painter->setPen(myPen);
    painter->drawPath(myPath);
}

void MyEllipse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{    
    emit show_planetInfo(this->data(1).toInt());
    QGraphicsItem::mousePressEvent(event);
    update();
}

void MyEllipse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

}