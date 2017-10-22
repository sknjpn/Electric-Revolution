#pragma once

struct Node;

struct Wire
{
	Node*	from;
	Node*	to;
	bool	broken;

	Wire(Node* _from, Node* _to);

	void	draw();
	bool	update();
	Bezier3	line() const;
};
