#pragma once
#include"Node.h"
struct Game;
struct Factory;

struct Blueprint
{
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
	Point	pos;
	Factory*	factory;
	sol::state	lua;
	Blueprint*	blueprint;
	Array<Node>	nodes;
	Array<std::pair<FilePath, Audio>>	audioAssets;

	static int	newMachineAngle;
	static Rect	newMachineRegion;
	static Machine*	selectedMachine;
	static Array<Blueprint>	blueprints;

	Machine(int _type, Point _pos, Factory*	_factory);
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

