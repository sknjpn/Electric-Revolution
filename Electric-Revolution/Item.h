#pragma once

struct Tile;
struct Factory;

//ƒAƒCƒeƒ€
struct Item
{
	int	id;
	int	layer;
	int	angle;
	bool	enabled;
	Point	p;
	double	t;
	Texture	texture;
	Factory* factory;

	Item(Factory* _factory, int _id, const Point& _p, int _layer);

	void	move(Point _p, int _angle, double _t);
	void	set(const Point& _p, int _id, int _layer);
	void	remove();
	Vec2	pos() const;
};