#include"Factory.h"

Factory::Factory(const Size& _size)
	: tile(L"assets/tile.png")
	, size(_size)
{
	itemMap.resize(size, nullptr);
	machineMap.resize(size, nullptr);
	keMap.resize(size);
	Window::Resize(1280, 720);

	items.resize(4096);
	machines.reserve(4096);

	Point pos(0, 0);

	for (int i = 0; i < int(Machine::blueprints.size()); i++)
	{
		for (int j = 0; j + Machine::blueprints[i].size.x <= size.x; j += Machine::blueprints[i].size.x)
		{
			machines.emplace_back(i, pos, this);
			pos.moveBy(Machine::blueprints[i].size.x, 0);
		}
		pos.set(0, pos.y + Machine::blueprints[i].size.y);
	}

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
	else if (m->angle == 1) p = Point((_pos - m->pos).y, m->blueprint->size.y - 1 - (_pos - m->pos).x);
	else if (m->angle == 2) p = Point(m->blueprint->size.x - 1 - (_pos - m->pos).x, m->blueprint->size.y - 1 - (_pos - m->pos).y);
	else p = Point(m->blueprint->size.x - 1 - (_pos - m->pos).y, (_pos - m->pos).x);

	if (m->lua["canPutItemAt"].get_type() != sol::type::function) return false;
	if (m->lua["canPutItemAt"](p.x, p.y, _id)) return true;
	else return false;
}