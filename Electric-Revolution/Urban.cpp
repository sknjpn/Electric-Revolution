#include"Urban.h"

Urban::Urban(Planet* _planet, const JSONValue& _j)
	: planet(_planet)
	, pos(_j[U"pos"].get<Point>())
	, name(_j[U"name"].get<String>())
	, factory(this, _j[U"factory"])
{}