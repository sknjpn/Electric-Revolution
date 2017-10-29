#pragma once

struct Tile;
struct Machine;

//動力系統
struct Gearbox
{
	Tile*	tile;
	Point	inMachinePos;		//相対座標上
	double	gain;
	double	resistance;
	double	pressure;
	Machine*	machine;
	Array<Gearbox*>	connectedGearbox;	//接続中のギアボックス

	Gearbox(Machine* _machine, const Point& _pos, double _resistance);

	Point	pos() const;
	Vec2	centerPos() const;
};