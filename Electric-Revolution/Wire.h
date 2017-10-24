#pragma once

struct Node;

struct Wire
{
	Node*	from;
	Node*	to;
	bool	broken;
	Color	color;

	static Color	selectedColor;

	Wire(Node* _from, Node* _to, const Color& _color);

	bool	update();
	Bezier3	line() const;
};
