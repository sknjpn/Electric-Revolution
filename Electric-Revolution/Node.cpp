#include"Node.h"
#include"Machine.h"

Node::Node(const Vec2& _pos, const Vec2& _dv, Machine* _parentMachine, NodeState _state, bool _fixed)
	: inMachinePos(_pos)
	, parentMachine(_parentMachine)
	, state(_state)
	, numConnected(0)
	, fixed(_fixed)
	, dv(_dv)
{

}

void	Node::draw() const
{
	Color color = Color(0, 0);
	switch (state)
	{
	case NodeState::Hi: color = Palette::Red;	break;
	case NodeState::Low: color = Palette::Blue;	break;
	case NodeState::None: color = Palette::White;	break;
	}
	Circle(pos(), 0.125).draw(color).drawFrame(0.0625, 0, Palette::Black);
	if (mouseOver()) Circle(pos(), 0.125).draw(Color(0, 128));
}
Vec2	Node::pos() const
{
	return parentMachine->pos + parentMachine->transformedPos(inMachinePos);
}

bool	Node::mouseOver() const
{
	return Circle(pos(), 0.125).mouseOver();
}
Node*	Node::selectedNode = nullptr;
