#include"Machine.h"
#include"Factory.h"

Blueprint::Blueprint(const FilePath& _mainFile)
	: mainFile(_mainFile)
	, group(nullptr)
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
Machine::Machine(Blueprint* _blueprint, Factory* _factory)
	: blueprint(_blueprint)
	, factory(_factory)
	, pos(-1000, -1000)
	, angle(0)
	, isVirtual(true)
{
	baseSize = blueprint->size;
	initLua();
}
void	Machine::draw()
{
	if (isVirtual) return;
	if (lua["draw"].get_type() == sol::type::function) lua["draw"]();
}

void	Machine::updateSystem()
{
	if (isVirtual) return;
	if (lua["updateSystem"].get_type() == sol::type::function) lua["updateSystem"]();

}
bool	Machine::updateConnects()
{
	if (isVirtual) return false;
	if (lua["updateConnects"].get_type() == sol::type::function) lua["updateConnects"]();
	return false;
}
Rect	Machine::region() const
{
	if (angle % 2 == 1) return Rect(pos, baseSize.y, baseSize.x);
	return Rect(pos, baseSize.x, baseSize.y);
}
Size	Machine::size() const
{
	if (angle % 2 == 1) return Size(baseSize.y, baseSize.x);
	return baseSize;
}
Point	Machine::transformedPos(const Point& _pos) const
{
	if (angle == 0) return Point(_pos.x, _pos.y);
	if (angle == 1) return Point(baseSize.y - 1 - _pos.y, _pos.x);
	if (angle == 2) return Point(baseSize.x - 1 - _pos.x, baseSize.y - 1 - _pos.y);
	return Point(_pos.y, baseSize.x - 1 - _pos.x);
}
Vec2	Machine::transformedPos(const Vec2& _pos) const
{
	if (angle == 0) return Vec2(_pos.x, _pos.y);
	if (angle == 1) return Vec2(baseSize.y - _pos.y, _pos.x);
	if (angle == 2) return Vec2(baseSize.x - _pos.x, baseSize.y - _pos.y);
	return Vec2(_pos.y, baseSize.x - _pos.x);
}
Size	Machine::factorySize() const
{
	return factory->size;
}
int		Machine::newMachineAngle;
Rect	Machine::newMachineRegion;
Machine*	Machine::selectedMachine = nullptr;
Array<Group>	Machine::groups;
Array<Blueprint>	Machine::blueprints;