
#ifndef MYLABEL_H
#define MYLABEL_H

#include <QPainter>
#include <QGraphicsSimpleTextItem>
#include <QFontMetrics>
#include <QDragEnterEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include "MultiSet.h"
#include "MyScene.h"

class MyLabel : public QGraphicsSimpleTextItem
{

public:
	MyLabel(MultiSet* multiset, MyScene* scene, int id);
	~MyLabel();

	//QRectF boundingRect() const;

	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

private:
	MyScene* scene;
	MultiSet* multiset;
	QPointF clickPos;
	int id;
};


#endif // MYLABEL_H

