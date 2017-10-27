#include"Urban.h"

Urban::Urban(Planet* _planet, const JSONValue& _j)
	: planet(_planet)
	, pos(_j[L"pos"].get<Point>())
	, name(_j[L"name"].get<String>())
	, factory(this, _j[L"factory"])
{

}