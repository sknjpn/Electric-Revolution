#pragma once
#include"Camera.h"
#include"Factory.h"

struct Game
{
	Camera	camera;
	Vec2	rightClickedPoint;
	Factory*	mainFactory;
	Array<Factory>	factories;

	Game();
	void	update();

};