# include <Siv3D.hpp> // OpenSiv3D v0.1.7

struct Machine;

struct Assets
{
	Array<std::pair<FilePath, Texture>>	textureAssets;

	Assets()
	{
		auto dc = FileSystem::DirectoryContents(L"assets");
		for (auto& c : dc)
		{
			if (FileSystem::IsFile(c) && FileSystem::Extension(c) == L"png")
			{
				auto t = c.removed(FileSystem::CurrentPath() + L"assets/");
				textureAssets.emplace_back(t, Texture(c));
			}
		}
	}

	Texture*	texture(const String& _path)
	{
		for (auto& ta : textureAssets)
		{
			if (ta.first == _path) return &ta.second;
		}
		return nullptr;
	}
};

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
	NodeState	state;
	Machine*	parentMachine;

	static Node*	selectedNode;

	Node(const Vec2& _pos, Machine* _parentMachine, NodeState _state = NodeState::None, bool _fixed = false)
		: inMachinePos(_pos)
		, parentMachine(_parentMachine)
		, state(_state)
		, numConnected(0)
		, fixed(_fixed)
	{

	}

	void	draw() const
	{
		Color color = Color(0, 0);
		switch (state)
		{
		case NodeState::Hi: color = Palette::Red;	break;
		case NodeState::Low: color = Palette::Blue;	break;
		case NodeState::None: color = Palette::White;	break;
		}
		Circle(pos(), 4).draw(color).drawFrame(2, Palette::Black);
		if (mouseOver()) Circle(pos(), 4).draw(Color(0, 128));
	}
	Vec2	pos() const;
	bool	mouseOver() const
	{
		return Circle(pos(), 4).mouseOver();
	}
};
Node*	Node::selectedNode = nullptr;

struct Wire
{
	Node*	from;
	Node*	to;
	bool	broken;

	Wire(Node* _from, Node* _to)
		: from(_from)
		, to(_to)
		, broken(false)
	{}

	void	draw()
	{
		Color color = Color(0, 0);
		switch (from->state)
		{
		case NodeState::Hi: color = Palette::Red;	break;
		case NodeState::Low: color = Palette::Blue;	break;
		case NodeState::None: color = Palette::White;	break;
		}
		line().draw(6, Palette::Black).draw(4, color);
	}

	bool	update()
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
	Line	line() const
	{
		return Line(from->pos(), to->pos());
	}
};

struct Machine
{
	int	type;
	Rect	region;
	Array<Node>	nodes;

	static Rect	newRegion;
	static Assets*	assets;
	static Machine*	selectedMachine;

	Machine(int _type, Rect _region)
		: type(_type)
		, region(_region)
	{
		switch (_type)
		{
		case 0:
			nodes.emplace_back(Vec2(0.0, 24.0), this);
			nodes.emplace_back(Vec2(64.0, 24.0), this);
			break;
		case 1:
			nodes.emplace_back(Vec2(32.0, 0.0), this);
			nodes.emplace_back(Vec2(32.0, 48.0), this);
			nodes.emplace_back(Vec2(0.0, 24.0), this);
			nodes.emplace_back(Vec2(64.0, 24.0), this);
			break;
		case 2:
			nodes.emplace_back(Vec2(0.0, 24.0), this);
			nodes.emplace_back(Vec2(32.0, 48.0), this);
			nodes.emplace_back(Vec2(64.0, 24.0), this);
			break;
		case 3:
			nodes.emplace_back(Vec2(0.0, 24.0), this, NodeState::Hi, true);
			nodes.emplace_back(Vec2(64.0, 24.0), this, NodeState::Low, true);
			break;
		case 4:
			nodes.emplace_back(Vec2(17.0, 48.0), this);
			nodes.emplace_back(Vec2(27.0, 48.0), this);
			nodes.emplace_back(Vec2(37.0, 48.0), this);
			nodes.emplace_back(Vec2(47.0, 48.0), this);
			break;
		case 5:
			nodes.emplace_back(Vec2(0.0, 24.0), this);
			nodes.emplace_back(Vec2(64.0, 24.0), this);
			nodes.emplace_back(Vec2(32.0, 24.0), this, NodeState::Low, true);
			break;
		default:
			break;
		}
	}

	void	draw()
	{
		String path;
		switch (type)
		{
		case 0:
			path = L"machine1/";
			assets->texture(L"machine1/image.png")->draw(region.pos);
			break;
		case 1:
			path = L"machine2/";
			assets->texture(L"machine2/image.png")->draw(region.pos);
			break;
		case 2:
			path = L"machine3/";
			assets->texture(L"machine3/image.png")->draw(region.pos);
			break;
		case 3:
			path = L"machine4/";
			assets->texture(L"machine4/image.png")->draw(region.pos);
			break;
		case 4:
		{
			path = L"machine5/";
			assets->texture(L"machine5/image.png")->draw(region.pos);
			int number = 0;
			number += nodes[0].state == NodeState::Hi ? 1 : 0;
			number += nodes[1].state == NodeState::Hi ? 2 : 0;
			number += nodes[2].state == NodeState::Hi ? 4 : 0;
			number += nodes[3].state == NodeState::Hi ? 8 : 0;
			switch (number)
			{
			case 0:
				assets->texture(L"machine5/led-a.png")->draw(region.pos);
				assets->texture(L"machine5/led-b.png")->draw(region.pos);
				assets->texture(L"machine5/led-c.png")->draw(region.pos);
				assets->texture(L"machine5/led-d.png")->draw(region.pos);
				assets->texture(L"machine5/led-e.png")->draw(region.pos);
				assets->texture(L"machine5/led-f.png")->draw(region.pos);
				break;
			case 1:
				assets->texture(L"machine5/led-b.png")->draw(region.pos);
				assets->texture(L"machine5/led-c.png")->draw(region.pos);
				break;
			case 2:
				assets->texture(L"machine5/led-a.png")->draw(region.pos);
				assets->texture(L"machine5/led-b.png")->draw(region.pos);
				assets->texture(L"machine5/led-d.png")->draw(region.pos);
				assets->texture(L"machine5/led-e.png")->draw(region.pos);
				assets->texture(L"machine5/led-g.png")->draw(region.pos);
				break;
			case 3:
				assets->texture(L"machine5/led-a.png")->draw(region.pos);
				assets->texture(L"machine5/led-b.png")->draw(region.pos);
				assets->texture(L"machine5/led-c.png")->draw(region.pos);
				assets->texture(L"machine5/led-d.png")->draw(region.pos);
				assets->texture(L"machine5/led-g.png")->draw(region.pos);
				break;
			case 4:
				assets->texture(L"machine5/led-b.png")->draw(region.pos);
				assets->texture(L"machine5/led-c.png")->draw(region.pos);
				assets->texture(L"machine5/led-f.png")->draw(region.pos);
				assets->texture(L"machine5/led-g.png")->draw(region.pos);
				break;
			case 5:
				assets->texture(L"machine5/led-a.png")->draw(region.pos);
				assets->texture(L"machine5/led-c.png")->draw(region.pos);
				assets->texture(L"machine5/led-d.png")->draw(region.pos);
				assets->texture(L"machine5/led-f.png")->draw(region.pos);
				assets->texture(L"machine5/led-g.png")->draw(region.pos);
				break;
			case 6:
				assets->texture(L"machine5/led-a.png")->draw(region.pos);
				assets->texture(L"machine5/led-c.png")->draw(region.pos);
				assets->texture(L"machine5/led-d.png")->draw(region.pos);
				assets->texture(L"machine5/led-e.png")->draw(region.pos);
				assets->texture(L"machine5/led-f.png")->draw(region.pos);
				assets->texture(L"machine5/led-g.png")->draw(region.pos);
				break;
			case 7:
				assets->texture(L"machine5/led-a.png")->draw(region.pos);
				assets->texture(L"machine5/led-b.png")->draw(region.pos);
				assets->texture(L"machine5/led-c.png")->draw(region.pos);
				assets->texture(L"machine5/led-f.png")->draw(region.pos);
				break;
			case 8:
				assets->texture(L"machine5/led-a.png")->draw(region.pos);
				assets->texture(L"machine5/led-b.png")->draw(region.pos);
				assets->texture(L"machine5/led-c.png")->draw(region.pos);
				assets->texture(L"machine5/led-d.png")->draw(region.pos);
				assets->texture(L"machine5/led-e.png")->draw(region.pos);
				assets->texture(L"machine5/led-f.png")->draw(region.pos);
				assets->texture(L"machine5/led-g.png")->draw(region.pos);
				break;
			case 9:
				assets->texture(L"machine5/led-a.png")->draw(region.pos);
				assets->texture(L"machine5/led-b.png")->draw(region.pos);
				assets->texture(L"machine5/led-c.png")->draw(region.pos);
				assets->texture(L"machine5/led-d.png")->draw(region.pos);
				assets->texture(L"machine5/led-f.png")->draw(region.pos);
				assets->texture(L"machine5/led-g.png")->draw(region.pos);
				break;
			case 10:
				assets->texture(L"machine5/led-a.png")->draw(region.pos);
				assets->texture(L"machine5/led-b.png")->draw(region.pos);
				assets->texture(L"machine5/led-c.png")->draw(region.pos);
				assets->texture(L"machine5/led-e.png")->draw(region.pos);
				assets->texture(L"machine5/led-f.png")->draw(region.pos);
				assets->texture(L"machine5/led-g.png")->draw(region.pos);
				break;
			case 11:
				assets->texture(L"machine5/led-c.png")->draw(region.pos);
				assets->texture(L"machine5/led-d.png")->draw(region.pos);
				assets->texture(L"machine5/led-e.png")->draw(region.pos);
				assets->texture(L"machine5/led-f.png")->draw(region.pos);
				assets->texture(L"machine5/led-g.png")->draw(region.pos);
				break;
			case 12:
				assets->texture(L"machine5/led-d.png")->draw(region.pos);
				assets->texture(L"machine5/led-e.png")->draw(region.pos);
				assets->texture(L"machine5/led-g.png")->draw(region.pos);
				break;
			case 13:
				assets->texture(L"machine5/led-b.png")->draw(region.pos);
				assets->texture(L"machine5/led-c.png")->draw(region.pos);
				assets->texture(L"machine5/led-d.png")->draw(region.pos);
				assets->texture(L"machine5/led-e.png")->draw(region.pos);
				assets->texture(L"machine5/led-g.png")->draw(region.pos);
				break;
			case 14:
				assets->texture(L"machine5/led-a.png")->draw(region.pos);
				assets->texture(L"machine5/led-d.png")->draw(region.pos);
				assets->texture(L"machine5/led-e.png")->draw(region.pos);
				assets->texture(L"machine5/led-f.png")->draw(region.pos);
				assets->texture(L"machine5/led-g.png")->draw(region.pos);
				break;
			case 15:
				assets->texture(L"machine5/led-a.png")->draw(region.pos);
				assets->texture(L"machine5/led-e.png")->draw(region.pos);
				assets->texture(L"machine5/led-f.png")->draw(region.pos);
				assets->texture(L"machine5/led-g.png")->draw(region.pos);
				break;
			}
		}
		break;
		case 5:
			path = L"machine6/";
			assets->texture(L"machine6/image.png")->draw(region.pos);
			break;
		default:
			break;
		}
		for (int i = 0; i < int(nodes.size()); i++)
		{
			Texture* tex = nullptr;
			switch (nodes[i].state)
			{
			case NodeState::Hi:
				tex = assets->texture(path + L"node" + Format(i) + L"-hi.png");
				break;
			case NodeState::Low:
				tex = assets->texture(path + L"node" + Format(i) + L"-low.png");
				break;
			}
			if (tex != nullptr) tex->draw(region.pos);
		}

		for (auto& n : nodes) n.draw();

		if (selectedMachine == this) region.draw(Color(Palette::Orange, 128));
	}

	void	updateSystem()
	{
		switch (type)
		{
		case 5:
			if (region.leftClicked())
			{
				if (nodes[2].state == NodeState::Hi) nodes[2].state = NodeState::Low;
				else nodes[2].state = NodeState::Hi;
			}
			break;
		}
	}
	bool	updateConnects()
	{
		bool flag = false;
		switch (type)
		{
		case 0:
			if (nodes[0].state == NodeState::Hi)
			{
				if (nodes[1].state != nodes[0].state) flag = true;
				nodes[1].state = nodes[0].state;
			}
			break;
		case 1:
			if (nodes[0].state != NodeState::None)
			{
				if (nodes[1].state != nodes[0].state) flag = true;
				if (nodes[2].state != nodes[0].state) flag = true;
				if (nodes[3].state != nodes[0].state) flag = true;
				nodes[1].state = nodes[0].state;
				nodes[2].state = nodes[0].state;
				nodes[3].state = nodes[0].state;
			}
			if (nodes[1].state != NodeState::None)
			{
				if (nodes[0].state != nodes[1].state) flag = true;
				if (nodes[2].state != nodes[1].state) flag = true;
				if (nodes[3].state != nodes[1].state) flag = true;
				nodes[0].state = nodes[1].state;
				nodes[2].state = nodes[1].state;
				nodes[3].state = nodes[1].state;
			}
			if (nodes[2].state != NodeState::None)
			{
				if (nodes[0].state != nodes[2].state) flag = true;
				if (nodes[1].state != nodes[2].state) flag = true;
				if (nodes[3].state != nodes[2].state) flag = true;
				nodes[0].state = nodes[2].state;
				nodes[1].state = nodes[2].state;
				nodes[3].state = nodes[2].state;
			}
			if (nodes[3].state != NodeState::None)
			{
				if (nodes[0].state != nodes[3].state) flag = true;
				if (nodes[1].state != nodes[3].state) flag = true;
				if (nodes[2].state != nodes[3].state) flag = true;
				nodes[0].state = nodes[3].state;
				nodes[1].state = nodes[3].state;
				nodes[2].state = nodes[3].state;
			}
			break;
		case 2:
			if (nodes[1].state == NodeState::Hi)
			{
				if (nodes[0].state != NodeState::None)
				{
					if (nodes[2].state != nodes[0].state) flag = true;
					nodes[2].state = nodes[0].state;

				}
				if (nodes[2].state != NodeState::None)
				{
					if (nodes[0].state != nodes[2].state) flag = true;
					nodes[0].state = nodes[2].state;
				}
			}
			break;
		case 5:
			if (nodes[2].state == NodeState::Hi)
			{
				if (nodes[0].state != NodeState::None)
				{
					if (nodes[1].state != nodes[0].state) flag = true;
					nodes[1].state = nodes[0].state;

				}
				if (nodes[1].state != NodeState::None)
				{
					if (nodes[0].state != nodes[1].state) flag = true;
					nodes[0].state = nodes[1].state;
				}
			}
			break;
		}
		return flag;
	}
};

Rect	Machine::newRegion;
Assets*	Machine::assets;
Machine*	Machine::selectedMachine = nullptr;

Vec2	Node::pos() const
{
	return parentMachine->region.pos.movedBy(inMachinePos);
}

struct Game
{
	Point	rightClickedPoint;
	Assets	assets;
	Array<Wire>	wires;
	Array<Machine>	machines;


	Game()
	{
		Window::Resize(1280, 720);
		Window::SetTitle(L"Electric Revolution");
		Graphics::SetBackground(Palette::Darkkhaki);

		Machine::assets = &assets;

		machines.reserve(1024);

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				machines.emplace_back(i, Rect(80 * j, 64 * i, 64, 48));
			}
		}
	}

	void	update()
	{
		if (MouseR.down()) rightClickedPoint = Cursor::Pos();
		if (MouseR.up())
		{
			for (auto& w : wires)
			{
				if (w.line().intersects(Line(rightClickedPoint, Cursor::Pos())))
				{
					w.broken = true;
				}
			}
		}
		if (MouseL.down())
		{
			auto* node = mouseOverNode();

			if (node == nullptr)
			{
				for (auto& m : machines)
				{
					if (m.region.leftClicked())
					{
						Machine::newRegion = m.region;
						Machine::selectedMachine = &m;
					}
				}
			}
			else
			{
				Node::selectedNode = node;
			}
		}
		if (MouseL.up())
		{
			if (Machine::selectedMachine != nullptr)
			{
				bool flag = false;
				for (auto& m : machines)
				{
					if (&m != Machine::selectedMachine && m.region.intersects(Machine::newRegion)) flag = true;
				}

				if (!flag) Machine::selectedMachine->region = Machine::newRegion;

				Machine::selectedMachine = nullptr;
			}
			if (Node::selectedNode != nullptr)
			{
				auto* node = mouseOverNode();

				if (node != nullptr && node != Node::selectedNode)
				{
					wires.emplace_back(node, Node::selectedNode);
				}
				Node::selectedNode = nullptr;
			}
		}
		if (Machine::selectedMachine != nullptr)
		{
			Machine::newRegion.moveBy(Cursor::Delta());
		}


		for (auto& m : machines)
		{
			for (auto& n : m.nodes)
			{
				if (!n.fixed) n.state = NodeState::None;
			}
		}
		for (;;)
		{
			bool flag = false;
			for (auto& m : machines)
			{
				if (m.updateConnects()) flag = true;
			}
			for (auto& w : wires)
			{
				if (w.update()) flag = true;
			}
			wires.remove_if([](Wire& w) { return w.broken; });
			if (!flag) break;
		}
		for (auto& m : machines) m.updateSystem();

		for (auto& m : machines) m.draw();
		if (mouseOverNode() == nullptr)
		{
			for (auto& m : machines)
			{
				if (m.region.mouseOver()) m.region.draw(Color(Palette::Green, 128));
			}
		}
		if (Machine::selectedMachine != nullptr)
		{
			bool flag = false;
			for (auto& m : machines)
			{
				if (&m != Machine::selectedMachine && m.region.intersects(Machine::newRegion)) flag = true;
			}

			if (flag) Machine::newRegion.draw(Color(Palette::Red, 128)).drawFrame(2, 0, Palette::Red);
			else Machine::newRegion.draw(Color(Palette::Orange, 128)).drawFrame(2, 0, Palette::Orange);
		}
		for (auto& w : wires) w.draw();
		if (Node::selectedNode != nullptr)
		{
			Color color = Color(0, 0);
			switch (Node::selectedNode->state)
			{
			case NodeState::Hi: color = Palette::Red;	break;
			case NodeState::Low: color = Palette::Blue;	break;
			case NodeState::None: color = Palette::White;	break;
			}
			Line(Node::selectedNode->pos(), Cursor::Pos()).draw(6, Palette::Black).draw(4, color);
		}

		if (MouseR.pressed())
		{
			Line(rightClickedPoint, Cursor::Pos()).draw(3, Palette::Red);
		}
	}

	Node*	mouseOverNode()
	{
		for (auto& m : machines)
		{
			for (auto& n : m.nodes)
			{
				if (n.mouseOver()) return &n;
			}
		}
		return nullptr;
	}

};

void Main()
{
	Game game;

	while (System::Update()) game.update();
}
