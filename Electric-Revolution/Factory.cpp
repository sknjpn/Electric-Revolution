#include"Factory.h"

Factory::Factory(Urban* _urban, const JSONValue& _j)
	: urban(_urban)
	, isMain(false)
	, size(_j[L"size"].get<Size>())
	, name(_j[L"name"].get<String>())
	, camera(RectF(size), RectF(1280, 720).scaled(1 / 16.0).setCenter(size / 2.0), 64.0, 8.0)
	, tileTexture(L"assets/map/tileTexture.png")
	, forklift(this)
	, ui(this)
	, selectedNode(nullptr)
	, selectedWireColor(Palette::Red)
	, rightClickedPos(0, 0)
{
	{
		Image tileData(L"assets/map/" + _j[L"tileData"].get<String>());
		JSONReader json(L"assets/map/tileData.json");
		tiles.resize(size);
		for (auto p : step(tiles.size()))
		{
			tiles[p.y][p.x].pos = p;
			tiles[p.y][p.x].items.resize(3, nullptr);
		}
		for (auto j : json.arrayView())
		{
			const auto& type = j[L"type"].get<int>();
			const auto& color = j[L"color"].get<Color>();
			const auto& canPutMachine = j[L"canPutMachine"].get<bool>();
			const auto& isTradeTile = j[L"isTradeTile"].get<bool>();
			for (auto p : step(size))
			{
				if (tileData[p.y][p.x] == color)
				{
					tiles[p.y][p.x].type = type;
					tiles[p.y][p.x].canPutMachine = canPutMachine;
					tiles[p.y][p.x].isTradeTile = isTradeTile;
				}
			}
		}
	}

	//MachineÇÃÉÅÉÇÉäämï€
	machines.reserve(1024);

	//ItemÇÃÉÅÉÇÉäämï€
	items.reserve(1024);
}
Node*	Factory::mouseOverNode()
{
	for (auto& m : machines)
	{
		for (auto& n : m.nodes)
		{
			if (Circle(n.pos(), 1 / 8.0).mouseOver()) return &n;
		}
	}
	return nullptr;
}
void	Factory::addItem(const Point& _pos, int _id, int _layer)
{
	for (auto& i : items)
	{
		if (!i.enabled)
		{
			i.set(_pos, _id, _layer);
			return;
		}
	}
	items.emplace_back(this, _id, _pos, _layer);
}
Machine*	Factory::newMachine()
{
	for (auto& m : machines)
	{
		if (!m.enabled) return &m;
	}
	return &machines.emplace_back(this);
}
Tile*	Factory::getTile(const Point& _pos)
{
	if (_pos.x < 0 || _pos.y < 0 || _pos.x >= size.x || _pos.y >= size.y) return nullptr;
	return &tiles[_pos.y][_pos.x];
}
bool	Factory::canPutItemAt(const Point& _pos, int _id, int _layer)
{
	auto* tile = getTile(_pos);

	if (tile == nullptr) return false;
	else
	{
		if (tile->machine == nullptr) return false;
		else
		{
			auto* m = tile->machine;
			Point p(0, 0);
			if (m->angle == 0) p = _pos - m->pos;
			else if (m->angle == 1) p = Point((_pos - m->pos).y, m->baseSize.y - 1 - (_pos - m->pos).x);
			else if (m->angle == 2) p = Point(m->baseSize.x - 1 - (_pos - m->pos).x, m->baseSize.y - 1 - (_pos - m->pos).y);
			else p = Point(m->baseSize.x - 1 - (_pos - m->pos).y, (_pos - m->pos).x);

			if (tile->machine->lua[L"canPutItemAt"].get_type() != sol::type::function) return false;
			if (tile->machine->lua[L"canPutItemAt"](p.x, p.y, _id, _layer)) return true;
			return false;
		}
	}
}