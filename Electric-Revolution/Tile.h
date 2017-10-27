#pragma once

struct Machine;
struct Gearbox;
struct Item;

//マップチップ
struct Tile
{
	int	type;
	bool	isTradeTile;
	bool	canPutMachine;
	Point	pos;
	Machine*	machine;
	Gearbox*	gearbox;
	Array<Item*>	items;

	Tile();
};
