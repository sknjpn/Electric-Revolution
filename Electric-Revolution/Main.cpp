# include <Siv3D.hpp> // OpenSiv3D v0.1.7

struct Machine;

enum struct NodeState
{
	Hi,
	Low,
	None,
};

struct Node
{
	int	numConnected;
	Vec2	inMachinePos;
	NodeState	state;
	Machine*	parentMachine;

	Vec2	pos() const;
};

struct Wire
{
	Node*	from;
	Node*	to;

	Wire()
		: from(nullptr)
		, to(nullptr)
	{}

	void	draw()
	{

	}

	bool	update()
	{
		if (from->state == to->state) return true;
		if (from->state != NodeState::None && to->state != NodeState::None) return false;
		if (from->state == NodeState::None) from->state = to->state;
		else to->state = from->state;
		return true;
	}
};

struct Machine
{
	Vec2	pos;
	Array<Node>	nodes;

	Machine()
	{
		pos = { 640, 480 };
	}

	void	draw()
	{
		Rect(pos.x, pos.y, 64, 48).draw(Palette::Red).drawFrame(2, Palette::Black);

	}

	void	update()
	{

	}
};

Vec2	Node::pos() const
{
	return parentMachine->pos.movedBy(inMachinePos);
}

struct Game
{
	Array<Wire>	wires;
	Array<Machine>	machines;

	Game()
	{
		Window::Resize(1280, 720);
		Window::SetTitle(L"Electric Revolution");

		machines.emplace_back();
	}

	void	update()
	{
		for (auto& m : machines) m.update();
		for (auto& w : wires) w.update();

		for (auto& m : machines) m.draw();
		for (auto& w : wires) w.draw();
	}
};

void Main()
{
	Game game;

	while (System::Update()) game.update();
}
