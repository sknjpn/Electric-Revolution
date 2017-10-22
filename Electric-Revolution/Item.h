#pragma once

struct Factory;

struct Item
{
	bool	enabled;
	int		id;
	Point	p;
	int		angle;
	double	t;
	Factory*	factory;

	static Array<Texture>	textureAssets;

	Item();

	Vec2	pos() const;
	void	draw();
	void	move(int _angle, double _t);
	void	erase();

	static void	addItem(Factory* _factory, const Point& _pos, int _id);
};
