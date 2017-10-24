#include"Wire.h"
#include"Node.h"
#include"Machine.h"

Color	Wire::selectedColor = RandomHSV();

Wire::Wire(Node* _from, Node* _to, const Color& _color)
	: from(_from)
	, to(_to)
	, broken(false)
	, color(_color)
{}

bool	Wire::update()
{
	if (broken) return false;
	if (from->state == to->state) return false;
	if (from->state != NodeState::None && to->state != NodeState::None)
	{
		broken = true;
		return true;
	}
	if (from->state == NodeState::None) from->state = to->state;
	else to->state = from->state;
	return true;
}
Bezier3	Wire::line() const
{
	const auto& p0 = from->pos();
	const auto& p1 = from->pos() + from->dv.rotated(from->parentMachine->angle * 90_deg);
	const auto& p2 = to->pos() + to->dv.rotated(to->parentMachine->angle * 90_deg);
	const auto& p3 = to->pos();

	return Bezier3(p0, p1, p2, p3);
}