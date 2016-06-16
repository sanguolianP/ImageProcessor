#ifndef MYPIXMAPITEM_H
#define MYPIXMAPITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QPointF>

class Mypixmapitem : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Mypixmapitem(QGraphicsPixmapItem *parent = 0);
    double myScale;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void reset();

signals:

public slots:
};

#endif // MYPIXMAPITEM_H
