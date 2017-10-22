#pragma once

struct Machine;

enum struct NodeState
{
	None,
	Hi,
	Low,
};

struct Node
{
	int	numConnected;
	bool	fixed;
	Vec2	inMachinePos;
	Vec2	dv;
	NodeState	state;
	Machine*	parentMachine;

	static Node*	selectedNode;

	Node(const Vec2& _pos, const Vec2& _dv, Machine* _parentMachine, NodeState _state, bool _fixed);

	void	draw() const;
	Vec2	pos() const;
	bool	mouseOver() const;
};
