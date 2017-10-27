#pragma once

#include"Urban.h"
#include"Camera.h"

struct Game;

struct Planet
{
	Game*	game;
	String	name;
	Camera	camera;
	Texture	mapTexture;
	Array<Urban>	urbans;

	Planet(Game* _game);
};
