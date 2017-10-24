#include"Factory.h"

Factory::Factory()
	: name()
	, tile(L"assets/tile.png")
	, size(64,64)
	, isMain(false)
{
	itemMap.resize(size, nullptr);
	machineMap.resize(size, nullptr);
	keMap.resize(size);

	items.resize(4096);
	machines.reserve(4096);
}

Node*	Factory::mouseOverNode()
{

	for (auto& m : machines)
	{
		for (auto& n : m.nodes)
		{
			if (n.mouseOver()) return &n;
		}
	}
	return nullptr;
}

bool	Factory::canMoveItemTo(const Point& _pos, int _id)
{
	if (_pos.x < 0 || _pos.y < 0 || _pos.x >= size.x || _pos.y >= size.y) return false;

	auto* m = machineMap.at(_pos);
	if (m == nullptr) return false;

	Point p(0, 0);
	if (m->angle == 0) p = _pos - m->pos;
	else if (m->angle == 1) p = Point((_pos - m->pos).y, m->baseSize.y - 1 - (_pos - m->pos).x);
	else if (m->angle == 2) p = Point(m->baseSize.x - 1 - (_pos - m->pos).x, m->baseSize.y - 1 - (_pos - m->pos).y);
	else p = Point(m->baseSize.x - 1 - (_pos - m->pos).y, (_pos - m->pos).x);

	if (m->lua["canPutItemAt"].get_type() != sol::type::function) return false;
	if (m->lua["canPutItemAt"](p.x, p.y, _id)) return true;
	else return false;
}