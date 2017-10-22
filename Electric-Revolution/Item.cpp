#include"Item.h"
#include"Factory.h"
Array<Texture>	Item::textureAssets;

void	Item::addItem(Factory* _factory, const Point& _pos, int _id)
{
	for (auto& i : _factory->items)
	{
		if (!i.enabled)
		{
			i.enabled = true;
			i.p = _pos;
			i.id = _id;
			i.angle = 0;
			i.t = 0.0;
			i.factory = _factory;
			_factory->itemMap.at(i.p) = &i;
			return;
		}
	}
}
Item::Item()
	: p()
	, t(0.0)
	, factory(nullptr)
	, id(0)
	, angle(0)
	, enabled(false)
{}
Vec2	Item::pos() const
{
	if (angle == 0) return p + Vec2::Right()*t;
	if (angle == 1) return p + Vec2::Down()*t;
	if (angle == 2) return p + Vec2::Left()*t;
	return p + Vec2::Up()*t;
}
void	Item::draw()
{
	if (!enabled) return;
	textureAssets[id].resize(1, 1).draw(pos());
}
void	Item::erase()
{
	enabled = false;
	if (t > 0)
	{
		const auto& tot = angle == 0 ? p.movedBy(1, 0) : angle == 1 ? p.movedBy(0, 1) : angle == 2 ? p.movedBy(-1, 0) : p.movedBy(0, -1);
		factory->itemMap.at(tot) = nullptr;
	}
	factory->itemMap.at(p) = nullptr;
}
void	Item::move(int _angle, double _t)
{
	const auto& tom = _angle == 0 ? p.movedBy(1, 0) : _angle == 1 ? p.movedBy(0, 1) : _angle == 2 ? p.movedBy(-1, 0) : p.movedBy(0, -1);
	const auto& tot = angle == 0 ? p.movedBy(1, 0) : angle == 1 ? p.movedBy(0, 1) : angle == 2 ? p.movedBy(-1, 0) : p.movedBy(0, -1);

	if (!factory->canMoveItemTo(tom, id)) return;

	if (t == 0)
	{
		if (factory->itemMap.at(tom) == nullptr)
		{
			factory->itemMap.at(tom) = this;
			t = _t;
			angle = _angle;
		}
	}
	else
	{
		if (angle == _angle)
		{
			t += _t;
			if (t >= 1.0)
			{
				t = 0.0;

				factory->itemMap.at(p) = nullptr;
				p = tom;
			}
		}
		else if (angle % 2 == _angle % 2)
		{
			t -= _t;
			if (t <= 0.0)
			{
				t = 0.0;

				factory->itemMap.at(tot) = nullptr;
			}
		}
	}
}