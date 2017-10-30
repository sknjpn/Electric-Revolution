#pragma once

#include"Machine.h"
#include"Camera.h"
#include"Tile.h"
#include"Wire.h"
#include"Item.h"
#include"Pipe.h"
#include"UI.h"

struct Urban;
struct Game;

struct Factory
{
	UI	ui;
	Vec2	rightClickedPos;
	bool	isMain;
	Size	size;
	Node*	selectedNode;
	Color	selectedWireColor;
	Urban*	urban;
	String	name;
	Camera	camera;
	Texture	tileTexture;
	Forklift	forklift;
	Grid<Tile>	tiles;
	Array<Item>	items;
	Array<Wire>	wires;
	Array<Pipe>	pipes;
	Array<Machine>	machines;
	Array<Plumber>	plumbers;

	Factory(Urban* _urban, const JSONValue& _j);

	Texture	texture(const FilePath& _path);
	Node*	mouseOverNode();
	void	addItem(const Point& _pos, int _id, int _layer);
	Tile*	getTile(const Point& _pos);
	bool	canPutItemAt(const Point& _pos, int _id, int _layer);
	Pipe*	newPipe();
	Machine*	newMachine();
	Plumber*	newPlumber();
};
