#pragma once

#include"Planet.h"
#include"Tile.h"
#include"Music.h"
#include"Blueprint.h"

struct Factory;

enum struct SceneState
{
	Title,
	MapView,
	FactoryView,
};

struct Game
{
	Music	music;
	Planet	planet;
	Factory*	mainFactory;
	SceneState	sceneState;
	Array<Font>	fontAssets;
	Array<Group>	groups;
	Array<Blueprint>	blueprints;
	Array<std::pair<FilePath, Texture>>	textureAssets;

	Game();

	Font&	font(int _size);	//_size > 0
	void	updateFactory(Factory* _f);
	void	updateTitle();
	void	updateMapView();
	void	updateFactoryView();
	Texture	texture(const FilePath& _path);
};