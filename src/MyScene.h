
#ifndef MYSCENE_H
#define MYSCENE_H

#include <QPainter>
#include <QGraphicsScene>
#include <iostream>


class MyScene : public QGraphicsScene
{
	Q_OBJECT

	public:
		MyScene();
		~MyScene();

		void updateScene(int id1, int id2);
		void switchAxis(int id);

	signals:
		void sceneChanged(int, int);
		void sceneAxisChanged(int);
};

#endif
