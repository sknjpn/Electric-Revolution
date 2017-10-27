#pragma once

struct Group;

struct Blueprint
{
	int	type;
	String	name;
	Group*	group;
	FilePath	mainPath;

	Blueprint(int type, const String& _name, const FilePath& _mainPath);

	Size	baseSize();
	Texture baseTexture();
};

struct Group
{
	String	name;
	Array<Blueprint*>	blueprints;

	Group(const String& _name);
};