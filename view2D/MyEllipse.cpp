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
    QPixmap pix("../models/surface/neutral1.jpg");
    outer_brush = QBrush(pix);
}

QRectF MyEllipse::boundingRect() const
{
    // outer most edges
    return QRectF(x,y,20,20);
}

void MyEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    QPen myPen = QPen(Qt::black,2);
    QPainterPath myPath;
    myPath.addEllipse(rect);
    painter->setBrush(outer_brush);
    painter->setPen(myPen);
    painter->drawPath(myPath);
}

void MyEllipse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit show_planetInfo(this->data(1).toInt());
    update();
    QGraphicsItem::mousePressEvent(event);
    QPixmap pix("../models/surface/my1.jpg");
    outer_brush = QBrush(pix);
    update();
}



void MyEllipse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

}