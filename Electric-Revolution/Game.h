#pragma once
#include"Camera.h"
#include"Factory.h"
#include"UI.h"
#include"Map.h"
#include<HamFramework.hpp>


enum struct GameState {
	Title,
	MapView,
	FactoryView,
	Tutorial,
};

struct Game
{
	UI	ui;
	Map	map;
	Audio	bgm;
	Camera	camera;
	Vec2	rightClickedPoint;
	Factory*	mainFactory;
	MiniScene<GameState> scene;
	Stopwatch	timer;
	Array<Font>	fontAssets;

	Game();
	Font&	font(int _size);
	void	updateFactory(Factory* _f);
	void	drawFactory(Factory* _f);
};
