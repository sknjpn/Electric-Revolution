#pragma once
#include"Camera.h"
#include"Factory.h"
#include"UI.h"

struct Game
{
	UI	ui;
	Camera	camera;
	Vec2	rightClickedPoint;
	Factory*	mainFactory;
	Array<Factory>	factories;

	Game();
	void	update();
	void	updateFactory(Factory* _f);
	void	drawFactory(Factory* _f);
};