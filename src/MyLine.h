#pragma once

#include <QPainter>
#include <QGraphicsLineItem>

class MyLine : public QGraphicsLineItem
{
	//Q_OBJECT

public:


	MyLine(qreal x1, qreal y1, qreal x2, qreal y2, QString tooltiptext = "", QGraphicsItem * parent = 0) :
		QGraphicsLineItem(x1, y1, x2, y2, parent)
	{
		selected = false;
		next = nullptr;
		setToolTip(tooltiptext);
	}

	~MyLine()
	{

	}

	void mousePressEvent(QGraphicsSceneMouseEvent *event)
	{
		this->clicked(!selected);
	}

	/*
	void virtual hoverEnterEvent(QGraphicsSceneHoverEvent *event)
	{
	std::cout << "Hovered in";

	if (QApplication::mouseButtons() == Qt::LeftButton)
	this->clicked(!selected);
	}
	*/

	void clicked(bool setTo)
	{
		if (selected == setTo)
			return;

		selected = setTo;

		if (selected)
		{
			this->setPen(QPen(Qt::red, 3));
		}
		else
		{
			this->setPen(QPen());
		}

		if (next != nullptr)
			next->clicked(setTo);
	}

	bool selected;
	MyLine *next;
};