#pragma once

struct Machine;
struct Gearbox;
struct Item;
struct Pipe;

//マップチップ
struct Tile
{
	int	type;
	bool	isTradeTile;
	bool	canPutMachine;
	Point	pos;
	Pipe*	pipe;
	Machine*	machine;
	Gearbox*	gearbox;
	Array<Item*>	items;

	Tile();
};