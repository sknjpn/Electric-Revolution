#pragma once

struct Machine;
struct Factory;
struct Group;
struct Blueprint;

enum struct UIMode
{
	None,
	EditMachineMode,
	EditWireMode,
	EditGearbox,
	Exit,
};

struct Forklift
{
	int	type;
	int angle;
	bool	enabled;
	Size	baseSize;
	Rect	region;
	Texture	baseTexture;
	Machine*	selectedMachine;
	Factory*	factory;
	Blueprint*	blueprint;

	Forklift(Factory* _factory);

	void	update();
	void	set(Machine* _sm);
	void	set(Blueprint* _blueprint);
	bool	canPutMachine() const;
};

struct UI
{
	Group*	selectedGroup;
	UIMode	uiMode;
	Texture	uiTexture;
	Factory*	factory;

	UI(Factory* _factory);
};