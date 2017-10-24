#pragma once

struct Game;
struct Group;
struct Blueprint;

enum struct UIMode
{
	None,
	EditMachineMode,
	EditWireMode,
	ReturnToMap,
};

struct UI
{
	bool	trashAreaMouseOver;
	Game*	game;
	UIMode	uiMode;
	Texture	uiTexture;
	Texture	trashBox;
	Group*	selectedGroup;
	Rect	trashArea;

	UI(Game* _game);

	void	update();
};