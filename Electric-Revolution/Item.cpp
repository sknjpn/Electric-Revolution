#include"Item.h"
#include"Factory.h"

Item::Item(Factory* _factory, int _id, const Point& _p, int _layer)
	: id(_id)
	, p(_p)
	, angle(0)
	, t(0.0)
	, enabled(true)
	, factory(_factory)
	, layer(_layer)
{
	texture = factory->texture(L"assets/items/" + Format(_id) + L".png");
	factory->tiles[p.y][p.x].items[layer] = this;
}

void	Item::set(const Point& _p, int _id, int _layer)
{
	id = _id;
	p = _p;
	angle = 0;
	t = 0.0;
	texture = Texture(L"assets/items/" + Format(_id) + L".png");
	enabled = true;
	layer = _layer;

	factory->tiles[p.y][p.x].items[layer] = this;
}

Vec2	Item::pos() const
{
	Vec2 toPos = (angle == 0) ? p.movedBy(1, 0) : (angle == 1) ? p.movedBy(0, 1) : (angle == 2) ? p.movedBy(-1, 0) : p.movedBy(0, -1);
	return Vec2(p).lerp(toPos, t);
}

void	Item::remove()
{
	enabled = false;
	factory->getTile(p)->items[layer] = nullptr;
	if (t != 0)
	{
		auto toPos = (angle == 0) ? p.movedBy(1, 0) : (angle == 1) ? p.movedBy(0, 1) : (angle == 2) ? p.movedBy(-1, 0) : p.movedBy(0, -1);
		factory->getTile(toPos)->items[layer] = nullptr;
	}
}

void	Item::move(Point _p, int _angle, double _t)
{
	auto fromPos = (angle == 0) ? p.movedBy(1, 0) : (angle == 1) ? p.movedBy(0, 1) : (angle == 2) ? p.movedBy(-1, 0) : p.movedBy(0, -1);
	auto toPos = (_angle == 0) ? p.movedBy(1, 0) : (_angle == 1) ? p.movedBy(0, 1) : (_angle == 2) ? p.movedBy(-1, 0) : p.movedBy(0, -1);

	if (p == _p)	//“¯‚¶À•W‚É‚©‚©‚é—Í‚Ìê‡
	{
		if (angle == _angle || t == 0)
		{
			if (!factory->canPutItemAt(toPos, id, layer)) return;
			angle = _angle;

			if (factory->getTile(toPos) == nullptr || (factory->getTile(toPos)->items[layer] != nullptr && factory->getTile(toPos)->items[layer] != this))
			{
				return;	//“®‚©‚¹‚È‚¢
			}
			else
			{
				factory->getTile(toPos)->items[layer] = this;
				t += _t*(1.0 - (pos() - Vec2(_p)).length());
				if (t >= 1.0)
				{
					factory->getTile(p)->items[layer] = nullptr;
					t = 0.0;
					p = toPos;
				}
			}
		}
		else
		{
			t -= _t*(1.0 - (pos() - Vec2(_p)).length());
			if (t <= 0)
			{
				angle = _angle;
				t = -t;
				factory->getTile(fromPos)->items[layer] = nullptr;
			}
		}
	}
	else	//sæ‚ÌÀ•W‚É‚©‚©‚é—Í‚Ìê‡
	{
		t += _t*(1.0 - (pos() - Vec2(_p)).length());
		if (t >= 1.0)
		{
			factory->getTile(p)->items[layer] = nullptr;
			t = 0.0;
			p = _p;
		}
	}
}