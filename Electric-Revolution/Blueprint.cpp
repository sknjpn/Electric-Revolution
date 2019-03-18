#include"Blueprint.h"

Blueprint::Blueprint(int _type, const String& _name, const FilePath& _mainPath)
	: type(_type)
	, name(_name)
	, mainPath(_mainPath)
{}

Group::Group(const String& _name)
	: name(_name)
{}

Size	Blueprint::baseSize()
{
	sol::state lua;
	lua.script_file(Unicode::Narrow(mainPath + U"main.lua"));
	return { lua["machine"]["size"]["x"].get<int>(), lua["machine"]["size"]["y"].get<int>() };
}

Texture Blueprint::baseTexture()
{
	return Texture(mainPath + U"image.png");
}