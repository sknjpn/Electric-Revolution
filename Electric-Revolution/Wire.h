#pragma once

struct Node;

//M†ü
struct Wire
{
	Node*	n0;
	Node*	n1;
	Color	color;


	Wire(Node* _n0, Node* _n1, const Color& _color);

	Bezier3	getBezier3() const;
};
