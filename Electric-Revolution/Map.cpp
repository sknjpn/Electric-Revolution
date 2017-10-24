#include"Map.h"

Urban::Urban(const String& _name, const Point& _pos)
	: pos(_pos)
	, name(_name)
{
}

Map::Map()
	: texture(L"assets/map/image.png")
{
	JSONReader json(L"assets/map/urbans.json");
	for (auto j : json.arrayView())
	{
		auto& u = urbans.emplace_back(j[L"name"].get<String>(), j[L"pos"].get<Point>());
		u.factory.name = u.name + L"çHèÍ";
	}
}