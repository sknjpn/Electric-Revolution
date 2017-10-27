#include"Gearbox.h"
#include"Machine.h"
#include"Factory.h"

Gearbox::Gearbox(Machine* _machine, const Point& _pos, double _resistance)
	: machine(_machine)
	, resistance(_resistance)
	, pressure(0.0)
	, gain(0.0)
	, inMachinePos(_pos)
{}

Point	Gearbox::pos() const
{
	return machine->pos + machine->transformInMachinePos(inMachinePos);
}
Vec2	Gearbox::centerPos() const
{
	return Vec2(pos()).movedBy(0.5, 0.5);
}