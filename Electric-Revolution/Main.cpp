// OpenSiv3D v0.1.7

#include<Siv3D.hpp>
#include"sol.hpp"

struct Machine;

struct Item
{
	Point	from;
	Point	to;
	double	t;
	Texture	texture;

	Item(const Point& _pos)
		: from(_pos)
		, to(_pos)
		, t(0.0)
		, texture(L"assets/items/cookie.png")
	{}

	Vec2	pos() const
	{
		return Vec2(from).lerp(Vec2(to), t);
	}
	void	draw()
	{
		texture.resize(1, 1).draw(pos());
	}
};

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
		Circle(pos(), 0.25).draw(color).drawFrame(0.125, 0, Palette::Black);
		if (mouseOver()) Circle(pos(), 0.25).draw(Color(0, 128));
	}
	Vec2	pos() const;
	bool	mouseOver() const
	{
		return Circle(pos(), 0.25).mouseOver();
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
		line().draw(0.25, Palette::Black).draw(0.125, color);
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

struct Blueprint
{
	Size	size;
	String	name;
	FilePath	mainFile;
	Array<std::pair<FilePath, Texture>>	textureAssets;
	Array<std::pair<FilePath, Audio>>	audioAssets;

	Blueprint(const FilePath& _mainFile)
		: mainFile(_mainFile)
	{
		INIReader	ini(_mainFile + L"config.ini");
		name = ini.get<String>(L"Base.name");
		size = ini.get<Size>(L"Base.size");

		auto dc = FileSystem::DirectoryContents(mainFile);
		for (auto& c : dc)
		{
			if (FileSystem::IsFile(c))
			{
				auto ex = FileSystem::Extension(c);
				if (ex == L"png")
				{
					textureAssets.emplace_back(c.removed(mainFile), Texture(c));
				}
				if (ex == L"mp3" || ex == L"wav")
				{
					audioAssets.emplace_back(c.removed(mainFile), Audio(c));
				}
			}
		}
	}
};

struct Machine
{
	sol::state	lua;
	Blueprint*	blueprint;
	Array<Node>	nodes;
	Array<Audio>	audios;

	static RectF	newRegion;
	static Assets*	assets;
	static Machine*	selectedMachine;
	static Array<Item> items;
	static Array<Blueprint>	blueprints;

	Machine(int _type, Point _pos)
		: blueprint(&blueprints[_type])
	{
		lua.script_file(CharacterSet::Narrow(blueprint->mainFile + L"main.lua").c_str());

		lua.open_libraries(sol::lib::base, sol::lib::package);
		lua["Print"] = [](const char* str) {
			Print << CharacterSet::Widen(str);
		};

		lua["drawTexture"] = [this](const char* str, int posX = 0, int posY = 0, int sizeX = 0, int sizeY = 0)
		{
			for (auto& ta : blueprint->textureAssets)
			{
				if (ta.first == CharacterSet::Widen(str))
				{
					if (sizeX != 0 && sizeY != 0) ta.second.resize(sizeX, sizeY).draw(pos().movedBy(posX, posY));
					else ta.second.resize(blueprint->size).draw(pos().movedBy(posX, posY));
					return;
				}
			}
		};
		lua["setNode"] = [this](double x, double y, int state, bool fixed)
		{
			nodes.emplace_back(Vec2(x, y), this, NodeState(state), fixed);
		};
		lua["getNodeState"] = [this](int nodeID)
		{
			return int(nodes[nodeID].state);
		};
		lua["setNodeState"] = [this](int nodeID, int state)
		{
			nodes[nodeID].state = NodeState(state);
		};
		lua["machineLeftClicked"] = [this]()
		{
			return region().leftClicked();
		};
		lua["machineRightClicked"] = [this]()
		{
			return region().leftClicked();
		};
		lua["playAudio"] = [this](const char* str)
		{
			audios.emplace_back(blueprint->mainFile + CharacterSet::Widen(str));
			audios.back().play();
		};
		lua["addItem"] = [this](int x, int y)
		{
			items.emplace_back(pos() + Point(x, y));
		};
		lua["moveItem"] = [this](int fromX, int fromY, int toX, int toY, double t)
		{
			for (auto& i : items)
			{
				if (i.from == Point(fromX, fromY).movedBy(pos()))
				{
					auto to = Point(toX, toY).movedBy(pos());
					if (to != i.to)
					{
						bool flag = true;
						for (auto& it : items)
						{
							if (it.from == to || it.to == to)
							{
								flag = false;
								break;
							}
						}
						if (!flag) continue;
						i.to = to;
					}
					i.t += t;
					if (i.t >= 1.0)
					{
						i.from = i.to;
						i.t = 0.0;
					}
				}
			}
		};
		lua["isItemPos"] = [this](int x, int y)
		{
			for (auto& i : items)
			{
				if (i.from == Point(x, y).movedBy(pos()) || i.to == Point(x, y).movedBy(pos())) return true;
			}
			return false;
		};
		lua["machinePosX"] = _pos.x;
		lua["machinePosY"] = _pos.y;
		lua["machineSizeX"] = blueprint->size.x;
		lua["machineSizeY"] = blueprint->size.y;
		lua["init"]();

	}
	void	draw()
	{
		lua["draw"]();

		for (auto& n : nodes) n.draw();

		if (selectedMachine == this) region().draw(Color(Palette::Orange, 128));
	}

	void	updateSystem()
	{
		lua["updateSystem"]();
	}
	bool	updateConnects()
	{
		lua["updateConnects"]();
		audios.remove_if([](Audio& a) { return !a.isPlaying(); });
		return false;
	}
	Point	pos() const
	{
		return { lua["machinePosX"].get<int>(), lua["machinePosY"].get<int>() };
	}
	Rect	region() const
	{
		return Rect(pos(), blueprint->size);
	}
	void	setRegion(const RectF& _region)
	{
		lua["machinePosX"] = int(_region.pos.x);
		lua["machinePosY"] = int(_region.pos.y);
		lua["machineSizeX"] = int(_region.size.x);
		lua["machineSizeY"] = int(_region.size.y);
	}
};

RectF	Machine::newRegion;
Assets*	Machine::assets;
Machine*	Machine::selectedMachine = nullptr;
Array<Item> Machine::items;
Array<Blueprint>	Machine::blueprints;

Vec2	Node::pos() const
{
	return parentMachine->pos().movedBy(inMachinePos);
}

class Camera
{
public:
	RectF	restrictedRegion;
	RectF	drawingRegion;
	RectF	smoothDrawingRegion;

	Camera()
		: restrictedRegion(Vec2(-2048, -2048), 3 * Vec2(2048, 2048))
		, drawingRegion(0, 0, 32, 32)
		, smoothDrawingRegion(drawingRegion)
	{}
	void	update()
	{
		{
			auto t = createTransformer2D();

			double delta = Mouse::Wheel();
			//if (drawingRegion.size.y >= 180_deg && delta > 0) delta = 0;
			//if (drawingRegion.size.y <= 1.8_deg && delta < 0) delta = 0;
			drawingRegion = drawingRegion.scaledAt(Cursor::PosF(), 1.0 + 0.1*delta);

			/*
			if (drawingRegion.size.y > 180_deg) drawingRegion = drawingRegion.scaledAt(drawingRegion.center(), (180_deg / drawingRegion.size.y));
			if (drawingRegion.pos.y < -90_deg) drawingRegion.pos.y = -90_deg;
			if (drawingRegion.pos.y + drawingRegion.size.y > 90_deg) drawingRegion.pos.y = 90_deg - drawingRegion.size.y;
			if (smoothDrawingRegion.pos.x < -180_deg) { drawingRegion.pos.x += 360_deg; smoothDrawingRegion.pos.x += 360_deg; }
			if (smoothDrawingRegion.pos.x > 180_deg) { drawingRegion.pos.x -= 360_deg; smoothDrawingRegion.pos.x -= 360_deg; }
			*/
			const double followingSpeed = 0.2;
			smoothDrawingRegion.pos = smoothDrawingRegion.pos*(1.0 - followingSpeed) + drawingRegion.pos*followingSpeed;
			smoothDrawingRegion.size = smoothDrawingRegion.size*(1.0 - followingSpeed) + drawingRegion.size*followingSpeed;
		}

		const double slidingSpeed = (drawingRegion.size.y / 180_deg)*0.05;
		const bool useKeyViewControl = true;

		if ((useKeyViewControl && KeyA.pressed()) || Cursor::Pos().x <= 0) drawingRegion.pos.x -= slidingSpeed;
		if ((useKeyViewControl && KeyW.pressed()) || Cursor::Pos().y <= 0) drawingRegion.pos.y -= slidingSpeed;
		if ((useKeyViewControl && KeyD.pressed()) || Cursor::Pos().x >= Window::Size().x - 1) drawingRegion.pos.x += slidingSpeed;
		if ((useKeyViewControl && KeyS.pressed()) || Cursor::Pos().y >= Window::Size().y - 1) drawingRegion.pos.y += slidingSpeed;
	}
	Transformer2D	createTransformer2D(double _magnification = 1.0) const
	{
		auto mat3x2 = Mat3x2::Scale(_magnification).translated(-smoothDrawingRegion.center()).scaled(Window::Size().y / smoothDrawingRegion.size.y).translated(Window::ClientRect().center());

		return Transformer2D(mat3x2, true);
	}
};

struct Game
{
	Vec2	rightClickedPoint;
	Assets	assets;
	Camera	camera;
	Array<Wire>	wires;
	Array<Machine>	machines;

	Game()
	{
		Window::Resize(1280, 720);
		Window::SetTitle(L"Electric Revolution");
		Graphics::SetBackground(Palette::Darkkhaki);

		Machine::assets = &assets;

		for (auto c : FileSystem::DirectoryContents(L"assets/machines/"))
		{
			if (c.includes(L"config.ini")) Machine::blueprints.emplace_back(c.removed(L"config.ini"));
		}

		machines.reserve(1024);

		for (int i = 0; i < int(Machine::blueprints.size()); i++)
		{
			for (int j = 0; j < 32; j++)
			{
				machines.emplace_back(i, Point(5 * j, 4 * i));
			}
		}

	}

	void	update()
	{
		camera.update();
		auto t = camera.createTransformer2D();

		if (MouseR.down()) rightClickedPoint = Cursor::PosF();
		if (MouseR.up())
		{
			for (auto& w : wires)
			{
				if (w.line().intersects(Line(rightClickedPoint, Cursor::PosF())))
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
					if (m.region().leftClicked())
					{
						Machine::newRegion = RectF(m.region()).movedBy(0.5, 0.5);
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
					if (&m != Machine::selectedMachine && m.region().intersects(Rect(Machine::newRegion))) flag = true;
				}

				if (!flag) Machine::selectedMachine->setRegion(Machine::newRegion);

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
			Machine::newRegion.moveBy(Cursor::DeltaF());
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
				if (m.region().mouseOver()) m.region().draw(Color(Palette::Green, 128));
			}
		}
		for (auto& i : Machine::items) i.draw();
		if (Machine::selectedMachine != nullptr)
		{
			bool flag = false;
			for (auto& m : machines)
			{
				if (&m != Machine::selectedMachine && m.region().intersects(Rect(Machine::newRegion))) flag = true;
			}

			if (flag) Rect(Machine::newRegion).draw(Color(Palette::Red, 128)).drawFrame(0.1, 0, Palette::Red);
			else Rect(Machine::newRegion).draw(Color(Palette::Orange, 128)).drawFrame(0.1, 0, Palette::Orange);
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
			Line(Node::selectedNode->pos(), Cursor::PosF()).draw(0.25, Palette::Black).draw(0.125, color);
		}

		if (MouseR.pressed())
		{
			Line(rightClickedPoint, Cursor::PosF()).draw(0.125, Palette::Red);
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
