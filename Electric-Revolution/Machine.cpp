#include"Machine.h"
#include"Factory.h"

Blueprint::Blueprint(const FilePath& _mainFile)
	: mainFile(_mainFile)
{
	INIReader	ini(_mainFile + L"config.ini");
	name = ini.get<String>(L"Base.name");
	size = ini.get<Size>(L"Base.size");

	auto dc = FileSystem::DirectoryContents(mainFile);
	for (auto& c : dc)
	{
		if (FileSystem::IsFile(c))
		{
			if (FileSystem::Extension(c) == L"png")
			{
				textureAssets.emplace_back(c.removed(mainFile), Texture(c));
			}
		}
	}
}

Texture*	Blueprint::texture(const String& _name)
{
	for (auto& ta : textureAssets)
	{
		if (ta.first == _name) return &ta.second;
	}
	return nullptr;
}
Machine::Machine(int _type, Point _pos, Factory* _factory)
	: blueprint(&blueprints[_type])
	, factory(_factory)
	, pos(_pos)
	, angle(0)
{

	initLua();

	for (auto& p : step(region().pos, region().size)) factory->machineMap.at(p) = this;
}
void	Machine::draw()
{
	if (lua["draw"].get_type() == sol::type::function) lua["draw"]();

	for (auto& n : nodes) n.draw();

	if (selectedMachine == this) region().draw(Color(Palette::Orange, 128));
}

void	Machine::updateSystem()
{
	if (lua["updateSystem"].get_type() == sol::type::function) lua["updateSystem"]();

}
bool	Machine::updateConnects()
{
	if (lua["updateConnects"].get_type() == sol::type::function) lua["updateConnects"]();
	return false;
}
Rect	Machine::region() const
{
	if (angle % 2 == 1) return Rect(pos, blueprint->size.y, blueprint->size.x);
	return Rect(pos, blueprint->size.x, blueprint->size.y);
}
Size	Machine::size() const
{
	if (angle % 2 == 1) return Size(blueprint->size.y, blueprint->size.x);
	return blueprint->size;
}
Point	Machine::transformedPos(const Point& _pos) const
{
	if (angle == 0) return Point(_pos.x, _pos.y);
	if (angle == 1) return Point(blueprint->size.y - 1 - _pos.y, _pos.x);
	if (angle == 2) return Point(blueprint->size.x - 1 - _pos.x, blueprint->size.y - 1 - _pos.y);
	return Point(_pos.y, blueprint->size.x - 1 - _pos.x);
}
Vec2	Machine::transformedPos(const Vec2& _pos) const
{
	if (angle == 0) return Vec2(_pos.x, _pos.y);
	if (angle == 1) return Vec2(blueprint->size.y - _pos.y, _pos.x);
	if (angle == 2) return Vec2(blueprint->size.x - _pos.x, blueprint->size.y - _pos.y);
	return Vec2(_pos.y, blueprint->size.x - _pos.x);
}
Size	Machine::factorySize() const
{
	return factory->size;
}
int		Machine::newMachineAngle;
Rect	Machine::newMachineRegion;
Machine*	Machine::selectedMachine = nullptr;
Array<Blueprint>	Machine::blueprints;
