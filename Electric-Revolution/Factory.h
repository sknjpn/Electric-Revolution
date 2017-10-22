#pragma once
#include"Item.h"
#include"Wire.h"
#include"Machine.h"

struct Node;

struct Factory
{
	Array<Item> items;
	Array<Wire>	wires;
	Grid<Item*>	itemMap;
	Grid<double>	keMap;
	Array<Machine>	machines;
	Grid<Machine*>	machineMap;
	Size	size;
	Texture	tile;
	
	Factory(const Size& _size);
	Node*	mouseOverNode();
	bool	canMoveItemTo(const Point& _pos, int _id);
};