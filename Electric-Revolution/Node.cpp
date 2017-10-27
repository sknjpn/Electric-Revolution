#include"Node.h"
#include"Machine.h"

Node::Node(const Vec2& _pos, const Vec2& _dv, Machine* _machine, NodeState _state, bool _fixed)
	: inMachinePos(_pos)
	, machine(_machine)
	, state(_state)
	, fixed(_fixed)
	, deltaVector(_dv)
{}

Vec2	Node::pos() const
{
	return machine->pos + machine->transformInMachinePos(inMachinePos);
}
