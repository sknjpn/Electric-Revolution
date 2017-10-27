#pragma once

#include"Factory.h"

struct Planet;

struct Urban
{
	Point	pos;
	String	name;
	Planet*	planet;
	Factory	factory;

	Urban(Planet* _planet, const JSONValue& _j);
};