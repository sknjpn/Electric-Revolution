#include"Wire.h"
#include"Node.h"
#include"Machine.h"

Wire::Wire(Node* _n0, Node* _n1, const Color& _color)
	: n0(_n0)
	, n1(_n1)
	, color(_color)
{}

Bezier3	Wire::getBezier3() const
{
	Vec2 p0 = n0->pos();
	Vec2 p1 = n0->machine->pos + n0->machine->transformInMachinePos(n0->inMachinePos + n0->deltaVector);
	Vec2 p2 = n1->machine->pos + n1->machine->transformInMachinePos(n1->inMachinePos + n1->deltaVector);
	Vec2 p3 = n1->pos();
	return Bezier3(p0, p1, p2, p3);
}