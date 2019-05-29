#include "MyScene.h"


MyScene::MyScene()
{
}

MyScene::~MyScene()
{
}

void MyScene::updateScene(int id1, int id2)
{
	std::cout << "updateScene()\n";
	emit sceneChanged(id1, id2);

}

void MyScene::switchAxis(int id)
{
	std::cout << "switchAxis()\n";
	emit sceneAxisChanged(id);

}