#pragma once

struct Machine;

//信号種類
enum struct NodeState
{
	None,
	Hi,
	Low,
};

//信号接続点
struct Node
{
	bool	fixed;			//true:出力 false:入力
	Vec2	inMachinePos;	//相対座標上
	Vec2	deltaVector;	//ベジェ曲線用
	Machine*	machine;
	NodeState	state;

	Node(const Vec2& _pos, const Vec2& _dv, Machine* _machine, NodeState _state, bool _fixed);

	Vec2	pos() const;
};