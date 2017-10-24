#pragma once
#include"Node.h"
struct Game;
struct Factory;
struct Blueprint;

struct Group
{
	String	name;
	Array<Blueprint*>	blueprints;
	
	Group(const String& _name)
		: name(_name)
	{}
};

struct Blueprint
{
	Group*	group;
	Size	size;
	String	name;
	FilePath	mainFile;
	Array<std::pair<FilePath, Texture>>	textureAssets;

	Blueprint(const FilePath& _mainFile);

	Texture*	texture(const String& _name);
};

struct Machine
{
	int	angle;
	bool	isVirtual;
	Size	baseSize;
	Point	pos;
	Factory*	factory;
	sol::state	lua;
	Blueprint*	blueprint;
	Array<Node>	nodes;
	Array<std::pair<FilePath, Audio>>	audioAssets;

	static int	newMachineAngle;
	static Rect	newMachineRegion;
	static Machine*	selectedMachine;
	static Array<Group>	groups;
	static Array<Blueprint>	blueprints;

	Machine(Blueprint* _blueprint, Factory* _factory);
	void	initLua();
	void	draw();
	void	updateSystem();
	bool	updateConnects();
	Rect	region() const;
	Size	size() const;
	Point	transformedPos(const Point& _pos) const;
	Vec2	transformedPos(const Vec2& _pos) const;
	Size	factorySize() const;
};

