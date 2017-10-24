#pragma once
#include"Factory.h"
#include"Scuttle.h"

struct Urban
{
	Point	pos;
	String	name;
	Factory	factory;

	Urban(const String& _name, const Point& _pos);
};

struct Map
{
	Texture	texture;
	Array<Urban>	urbans;

	Map();
};