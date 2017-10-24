#pragma once
#include"Item.h"
#include"Wire.h"
#include"Machine.h"

struct Node;

struct Factory
{
	bool	isMain;
	Size	size;
	String	name;
	Texture	tile;
	Array<Item> items;
	Array<Wire>	wires;
	Grid<Item*>	itemMap;
	Grid<double>	keMap;
	Grid<Machine*>	machineMap;
	Array<Machine>	machines;

	Factory();
	Node*	mouseOverNode();
	bool	canMoveItemTo(const Point& _pos, int _id);
};