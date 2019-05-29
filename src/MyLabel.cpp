#include "MyLabel.h"


MyLabel::MyLabel(MultiSet* multiset, MyScene* scene, int id)
{
	this->scene = scene;
	this->multiset = multiset;
	this->id = id;
	setFont(QFont("Arial", 10));
}


MyLabel::~MyLabel()
{
}

/*
QRectF MyLabel::boundingRect() const
{
	QFontMetrics fm(font());
	return fm.boundingRect(text());
}

*/
void MyLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{	
	QRectF rect = boundingRect();

	painter->drawText(rect, text());
	//painter->drawRect(rect);
}

void MyLabel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	std::cout << "pressed\n";
	// save starting position
	clickPos = event->scenePos();
	std::cout << "clickpos: " << clickPos.x() << ", " << clickPos.y() << "\n";

	QGraphicsSimpleTextItem::mousePressEvent(event);
}

void MyLabel::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	std::cout << "released\n";
	std::cout << "clickpos: " << event->scenePos().x() << ", " << event->scenePos().y() << "\n";

	// if its just clicking
	if (event->scenePos().x() == clickPos.x() && event->scenePos().y() == clickPos.y()) {
		std::cout << "just clicked\n";
		scene->switchAxis(id);
	}
	else // dragging
	{
		QList<QGraphicsItem*> colItems = collidingItems();
		if (colItems.isEmpty()) {
			// do nothing
			std::cout << "no collision\n"; 
			QGraphicsSimpleTextItem::mouseReleaseEvent(event);			
			this->setPos(clickPos);
		}
		else {
			// get colliding items with this item
			MyLabel* closestItem = NULL;
			qreal shortestDist = 100000;
			// get nearest label
			foreach(QGraphicsItem* item, colItems){
				// cast mylabel. null if cast not possible
				MyLabel* label = dynamic_cast<MyLabel*>(item);
				if (label != NULL) {
					std::cout << "colliding with " << label->text().toStdString() << "\n";
					QLineF line(label->sceneBoundingRect().center(),
						this->sceneBoundingRect().center());
					if (line.length() < shortestDist){
						shortestDist = line.length();
						closestItem = label;
					}
				}
			}
			if (closestItem != NULL) {
				MyLabel* label = dynamic_cast<MyLabel*>(closestItem);
				QGraphicsSimpleTextItem::mouseReleaseEvent(event);
				scene->updateScene(id, label->id);

			}
			else {
				QGraphicsSimpleTextItem::mouseReleaseEvent(event);
				this->setPos(clickPos);
			}
		}
	}
	
}