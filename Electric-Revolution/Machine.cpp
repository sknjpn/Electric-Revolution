#include"Machine.h"
#include"Blueprint.h"
#include"Factory.h"

Machine::Machine(Factory* _factory)
	: enabled(false)
	, factory(_factory)
{}

void	Machine::set(Blueprint* _blueprint, const Point& _pos, int _angle)
{
	enabled = true;
	blueprint = _blueprint;
	type = blueprint->type;
	baseSize = blueprint->baseSize();
	baseTexture = blueprint->baseTexture();
	pos = _pos;
	angle = _angle;
	mainPath = blueprint->mainPath;
	initLua();

	//Tile‚É“o˜^
	for (auto p : step(pos, region().size)) factory->tiles[p.y][p.x].machine = this;
}

Rect	Machine::region() const
{
	if (angle % 2 == 0) return { pos, baseSize };
	else return { pos, baseSize.y, baseSize.x };
}

Audio	Machine::audio(const FilePath& _path)
{
	for (auto& aa : audioAssets)
	{
		if (aa.first == _path) return aa.second;
	}

	return audioAssets.emplace_back(_path, _path).second;
}

Texture	Machine::texture(const FilePath& _path)
{
	return factory->texture(_path);
}

Point	Machine::transformInMachinePos(const Point& _pos) const
{
	if (angle == 0) return Point(_pos.x, _pos.y);
	if (angle == 1) return Point(baseSize.y - 1 - _pos.y, _pos.x);
	if (angle == 2) return Point(baseSize.x - 1 - _pos.x, baseSize.y - 1 - _pos.y);
	return Point(_pos.y, baseSize.x - 1 - _pos.x);
}

Vec2	Machine::transformInMachinePos(const Vec2& _pos) const
{
	if (angle == 0) return Vec2(_pos.x, _pos.y);
	if (angle == 1) return Vec2(baseSize.y - _pos.y, _pos.x);
	if (angle == 2) return Vec2(baseSize.x - _pos.x, baseSize.y - _pos.y);
	return Vec2(_pos.y, baseSize.x - _pos.x);
}