#include "mypixmapitem.h"

Mypixmapitem::Mypixmapitem(QGraphicsPixmapItem *parent)
{
    myScale=scale();
}

void Mypixmapitem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void Mypixmapitem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setPos(pos()+mapToParent(event->pos())-mapToParent(event->lastPos()));
}

void Mypixmapitem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    setTransformOriginPoint(boundingRect().width()/2,boundingRect().height()/2);
    if(0!=event->delta())
    {
        if(event->delta()>0)
            setScale(scale()*1.1);
        if(event->delta()<0)
            setScale(scale()*0.9);
    }
}

void Mypixmapitem::reset()
{
    setScale(myScale);
    setPos(0,0);
}
