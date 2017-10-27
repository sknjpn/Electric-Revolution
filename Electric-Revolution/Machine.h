#pragma once

#include"Node.h"
#include"Gearbox.h"

struct Blueprint;
struct Factory;

//機械
struct Machine
{
	int	type;
	int	angle;
	bool	enabled;
	Size	baseSize;
	Point	pos;
	Texture	baseTexture;	//一時的な表示用
	FilePath	mainPath;
	Factory*	factory;
	Blueprint*	blueprint;
	sol::state	lua;
	Array<Node>	nodes;
	Array<Gearbox>	gearboxes;
	Array<std::pair<FilePath, Audio>>	audioAssets;
	Array<std::pair<FilePath, Texture>>	textureAssets;

	Machine(Factory* _factory);

	void	set(Blueprint* _blueprint, const Point& _pos, int _angle);
	void	initLua();
	Rect	region() const;	//実座標上
	Audio	audio(const FilePath& _path);
	Texture	texture(const FilePath& _path);
	Point	transformInMachinePos(const Point& _pos) const;
	Vec2	transformInMachinePos(const Vec2& _pos) const;
};