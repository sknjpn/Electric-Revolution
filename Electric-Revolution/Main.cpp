// OpenSiv3D v0.1.7

#include<Siv3D.hpp>
#include"sol.hpp"

struct Machine;
struct Game;


struct Item
{
	int		id;
	Point	from;
	Point	to;
	double	t;
	Texture	texture;

	Item(const Point& _pos, int _id, Texture _texture)
		: from(_pos)
		, to(_pos)
		, t(0.0)
		, texture(_texture)
		, id(_id)
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

	Node(const Vec2& _pos, const Vec2& _dv, Machine* _parentMachine, NodeState _state, bool _fixed)
		: inMachinePos(_pos)
		, parentMachine(_parentMachine)
		, state(_state)
		, numConnected(0)
		, fixed(_fixed)
		, dv(_dv)
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
		Circle(pos(), 0.125).draw(color).drawFrame(0.0625, 0, Palette::Black);
		if (mouseOver()) Circle(pos(), 0.125).draw(Color(0, 128));
	}
	Vec2	pos() const;
	bool	mouseOver() const
	{
		return Circle(pos(), 0.125).mouseOver();
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

	void	draw();

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
	Bezier3	line() const;
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

	Texture*	texture(const String& _name)
	{
		for (auto& ta : textureAssets)
		{
			if (ta.first == _name) return &ta.second;
		}
		return nullptr;
	}
};

struct Machine
{
	Game*		game;
	sol::state	lua;
	Blueprint*	blueprint;
	Array<Node>	nodes;
	Array<Audio>	audios;

	static int	newMachineAngle;
	static Rect	newMachineRegion;
	static Machine*	selectedMachine;
	static Array<Item> items;
	static Array<Blueprint>	blueprints;

	Machine(int _type, Point _pos, Game* _game)
		: blueprint(&blueprints[_type])
		, game(_game)
	{
		lua.script_file(CharacterSet::Narrow(blueprint->mainFile + L"main.lua").c_str());

		lua.open_libraries(sol::lib::base, sol::lib::package);
		lua["Print"] = [](const char* str) {
			Print << CharacterSet::Widen(str);
		};

		lua["drawTexture"] = [this](const char* str)
		{
			for (auto& ta : blueprint->textureAssets)
			{
				if (ta.first == CharacterSet::Widen(str))
				{
					ta.second
						.resize(blueprint->size)
						.rotate(lua["machineAngle"].get<int>() * 90_deg)
						.drawAt(transformedRegion().center());
					return;
				}
			}
		};
		lua["setNodeFixed"] = [this](int nodeID, bool fixed)
		{
			nodes[nodeID].fixed = fixed;
		};
		lua["setNode"] = [this](double x, double y, double dx, double dy, int state, bool fixed)
		{
			nodes.emplace_back(Vec2(x, y), Vec2(dx, dy), this, NodeState(state), fixed);
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
			return transformedRegion().leftClicked();
		};
		lua["machineRightClicked"] = [this]()
		{
			return transformedRegion().rightClicked();
		};
		lua["playAudio"] = [this](const char* str, double volume)
		{
			audios.emplace_back(blueprint->mainFile + CharacterSet::Widen(str));
			audios.back().play();
			if (volume != 0.0) audios.back().setVolume(volume);
		};
		lua["addItem"] = [this](int x, int y, int id)
		{
			items.emplace_back(pos() + transformedPos(Point(x, y)), id, Texture(L"assets/items/" + Format(id) + L".png"));
		};
		lua["removeItem"] = [this](int x, int y)
		{
			Point p = pos() + transformedPos(Point(x, y));
			items.remove_if([&p](Item& i) { return i.from == p || i.to == p; });
		};
		lua["moveItem"] = [this](int fromX, int fromY, int toX, int toY, double t)
		{
			for (auto& i : items)
			{
				if (i.from == pos() + transformedPos(Point(fromX, fromY)))
				{
					auto to = pos() + transformedPos(Point(toX, toY));
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
				if (i.from == pos() + transformedPos(Point(x, y)) || i.to == pos() + transformedPos(Point(x, y))) return true;
			}
			return false;
		};
		lua["machinePosX"] = _pos.x;
		lua["machinePosY"] = _pos.y;
		lua["machineSizeX"] = blueprint->size.x;
		lua["machineSizeY"] = blueprint->size.y;
		lua["machineAngle"] = 0;
		lua["init"]();
		lua["isWallTile"] = [this](int x, int y)
		{
			auto p = pos() + transformedPos(Point(x, y));
			return p.x == -1 || p.y == -1 || p.x == factorySize().x || p.y == factorySize().y;
		};

	}
	void	draw()
	{
		lua["draw"]();

		for (auto& n : nodes) n.draw();

		if (selectedMachine == this) transformedRegion().draw(Color(Palette::Orange, 128));
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
	Size	size() const
	{
		return { lua["machineSizeX"].get<int>(), lua["machineSizeY"].get<int>() };
	}
	Rect	region() const
	{
		return Rect(pos(), size());
	}
	void	setRegion(const RectF& _region)
	{
		lua["machinePosX"] = int(_region.pos.x);
		lua["machinePosY"] = int(_region.pos.y);
		lua["machineSizeX"] = int(_region.size.x);
		lua["machineSizeY"] = int(_region.size.y);
	}
	int		angle() const
	{
		return lua["machineAngle"].get<int>();
	}
	void	setAngle(int _angle)
	{
		lua["machineAngle"] = _angle;
	}
	Rect	transformedRegion() const
	{
		if (lua["machineAngle"].get<int>() % 2 == 1) return Rect(region().pos, region().h, region().w);
		return region();
	}
	Point	transformedPos(const Point& _pos) const
	{
		if (lua["machineAngle"].get<int>() == 0) return Point(_pos.x, _pos.y);
		if (lua["machineAngle"].get<int>() == 1) return Point(size().y - 1 - _pos.y, _pos.x);
		if (lua["machineAngle"].get<int>() == 2) return Point(size().x - 1 - _pos.x, size().y - 1 - _pos.y);
		return Point(_pos.y, size().x - 1 - _pos.x);
	}
	Vec2	transformedPos(const Vec2& _pos) const
	{
		if (lua["machineAngle"].get<int>() == 0) return Vec2(_pos.x, _pos.y);
		if (lua["machineAngle"].get<int>() == 1) return Vec2(size().y - _pos.y, _pos.x);
		if (lua["machineAngle"].get<int>() == 2) return Vec2(size().x - _pos.x, size().y - _pos.y);
		return Vec2(_pos.y, size().x - _pos.x);
	}
	Size	factorySize() const;
};

int		Machine::newMachineAngle;
Rect	Machine::newMachineRegion;
Machine*	Machine::selectedMachine = nullptr;
Array<Item> Machine::items;
Array<Blueprint>	Machine::blueprints;

Bezier3	Wire::line() const
{
	const auto& p0 = from->pos();
	const auto& p1 = from->pos() + from->dv.rotated(from->parentMachine->angle() * 90_deg);
	const auto& p2 = to->pos() + to->dv.rotated(to->parentMachine->angle() * 90_deg);
	const auto& p3 = to->pos();

	return Bezier3(p0, p1, p2, p3);
}
void	Wire::draw()
{
	Color color = Color(0, 0);
	switch (from->state)
	{
	case NodeState::Hi: color = Palette::Red;	break;
	case NodeState::Low: color = Palette::Blue;	break;
	case NodeState::None: color = Palette::White;	break;
	}
	line().draw(0.125 + 0.0625, Palette::Black);
	line().draw(0.125, color);
}
Vec2	Node::pos() const
{
	return parentMachine->pos() + parentMachine->transformedPos(inMachinePos);
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
			drawingRegion = drawingRegion.scaledAt(Cursor::PosF(), 1.0 + 0.1*delta);

			const double followingSpeed = 0.2;
			smoothDrawingRegion.pos = smoothDrawingRegion.pos*(1.0 - followingSpeed) + drawingRegion.pos*followingSpeed;
			smoothDrawingRegion.size = smoothDrawingRegion.size*(1.0 - followingSpeed) + drawingRegion.size*followingSpeed;
		}

		Print << Window::GetState().focused;
		if (Window::GetState().focused)
		{
			const double slidingSpeed = (drawingRegion.size.y / 180_deg)*0.05;
			const bool useKeyViewControl = true;

			if ((useKeyViewControl && KeyA.pressed()) || Cursor::Pos().x <= 0) drawingRegion.pos.x -= slidingSpeed;
			if ((useKeyViewControl && KeyW.pressed()) || Cursor::Pos().y <= 0) drawingRegion.pos.y -= slidingSpeed;
			if ((useKeyViewControl && KeyD.pressed()) || Cursor::Pos().x >= Window::Size().x - 1) drawingRegion.pos.x += slidingSpeed;
			if ((useKeyViewControl && KeyS.pressed()) || Cursor::Pos().y >= Window::Size().y - 1) drawingRegion.pos.y += slidingSpeed;
		}
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
	Camera	camera;
	Array<Wire>	wires;
	Array<Machine>	machines;
	Texture	tile;
	Texture	wall;
	Size	factorySize;

	Game()
		: tile(L"assets/tile.png")
		, wall(L"assets/wall.png")
		, factorySize(128, 256)
	{
		Window::Resize(1280, 720);
		Window::SetTitle(L"Electric Revolution");
		Graphics::SetBackground(Palette::Darkkhaki);

		for (auto c : FileSystem::DirectoryContents(L"assets/machines/"))
		{
			if (c.includes(L"config.ini")) Machine::blueprints.emplace_back(c.removed(L"config.ini"));
		}

		machines.reserve(2048);

		Point pos(1, 1);
		for (int i = 0; i < int(Machine::blueprints.size()); i++)
		{
			for (int j = 0; j < factorySize.x - Machine::blueprints[i].size.x - 1; j += Machine::blueprints[i].size.x + 1)
			{
				machines.emplace_back(i, pos, this);
				pos.moveBy(Machine::blueprints[i].size.x + 1, 0);
			}
			pos.set(1, pos.y + Machine::blueprints[i].size.y + 1);
		}

	}

	void	update()
	{
		{
			//Graphics2D::SetSamplerState(SamplerState::MirrorNearest);
			ClearPrint();
			Print << L"選択中にRキーで回転できます。";
			Print << L"カウンタは左下のピンが入力です";
			//カメラ処理
			camera.update();
			auto t = camera.createTransformer2D();

			//信号初期化処理
			for (auto& m : machines)
			{
				for (auto& n : m.nodes)
				{
					if (!n.fixed) n.state = NodeState::None;
				}
			}

			//信号伝達処理
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

			//機械固有処理
			for (auto& m : machines) m.updateSystem();

			//背景描画
			for (int x = -1; x <= factorySize.x; x++)
			{
				wall.resize(1.0, 1.0).draw(x, -1.0);
				wall.resize(1.0, 1.0).draw(x, factorySize.y);
			}
			for (int y = -1; y <= factorySize.y; y++)
			{
				wall.resize(1.0, 1.0).draw(-1.0, y);
				wall.resize(1.0, 1.0).draw(factorySize.x, y);
			}
			for (auto p : step(factorySize)) tile.resize(1.0, 1.0).draw(p);
			/*
			for (int x = 0; x < 128; x++) Line(x, 0, x, 128).draw(1 / 16.0, Palette::Black);
			for (int x = 0; x < 128; x++) Line(0, x, 128, x).draw(1 / 16.0, Palette::Black);
			*/

			//機械描画
			for (auto& m : machines) m.draw();

			//アイテム描画
			for (auto& i : Machine::items) i.draw();

			//ノードもしくは機械の選択
			if (MouseL.down())
			{
				if (KeyShift.pressed())
				{
					for (auto& m : machines)
					{
						if (m.transformedRegion().leftClicked())
						{
							Machine::newMachineRegion = m.transformedRegion();
							Machine::newMachineAngle = m.angle();
							Machine::selectedMachine = &m;
						}
					}
				}
				else Node::selectedNode = mouseOverNode();
			}

			//選択された機械の処理
			if (Machine::selectedMachine != nullptr)
			{
				Machine::newMachineRegion = RectF(Machine::newMachineRegion).setCenter(Cursor::PosF().movedBy(0.5, 0.5));

				if (KeyR.down())
				{
					Machine::newMachineRegion.size.set(Machine::newMachineRegion.size.y, Machine::newMachineRegion.size.x);
					Machine::newMachineAngle = (Machine::newMachineAngle + 1) % 4;
				}
				if (MouseL.up() || KeyShift.up())
				{
					bool flag = !Machine::newMachineRegion.pos.intersects(Rect(factorySize)) || !Machine::newMachineRegion.br().movedBy(-1, -1).intersects(Rect(factorySize));
					for (auto& m : machines)
					{
						if (&m != Machine::selectedMachine && m.transformedRegion().intersects(Machine::newMachineRegion)) flag = true;
					}

					if (!flag)
					{
						Machine::selectedMachine->setAngle(Machine::newMachineAngle);
						Machine::selectedMachine->setRegion(Rect(Machine::newMachineRegion.pos, Machine::selectedMachine->region().size));
					}
					Machine::selectedMachine = nullptr;
				}
				else
				{
					bool flag = !Machine::newMachineRegion.pos.intersects(Rect(factorySize)) || !Machine::newMachineRegion.br().movedBy(-1, -1).intersects(Rect(factorySize));
					for (auto& m : machines)
					{
						if (&m != Machine::selectedMachine && m.transformedRegion().intersects(Machine::newMachineRegion)) flag = true;
					}

					Machine::selectedMachine->blueprint->texture(L"image.png")
						->resize(Machine::selectedMachine->blueprint->size)
						.rotate(Machine::newMachineAngle * 90_deg)
						.drawAt(Machine::newMachineRegion.center(), Color(255, 128));

					if (flag) Machine::newMachineRegion.draw(Color(Palette::Red, 128)).drawFrame(0.1, 0, Palette::Red);
					else Machine::newMachineRegion.draw(Color(Palette::Orange, 128)).drawFrame(0.1, 0, Palette::Orange);
				}
			}
			else if (KeyShift.pressed())
			{
				//機械に選択予定の表示
				for (auto& m : machines)
				{
					if (m.transformedRegion().mouseOver()) m.transformedRegion().draw(Color(Palette::Green, 128));
				}
			}
			for (auto& w : wires) w.draw();

			//選択されたノードの処理
			if (Node::selectedNode != nullptr)
			{
				if (MouseL.up())
				{
					auto* node = mouseOverNode();

					if (node != nullptr && node != Node::selectedNode)
					{
						wires.emplace_back(node, Node::selectedNode);
					}
					Node::selectedNode = nullptr;
				}
				else
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
			}

			//配線削除
			if (MouseR.down()) rightClickedPoint = Cursor::PosF();
			if (MouseR.up())
			{
				for (auto& w : wires)
				{
					if (Line(w.from->pos(), w.to->pos()).intersects(Line(rightClickedPoint, Cursor::PosF()))) w.broken = true;
				}
			}
			if (MouseR.pressed()) Line(rightClickedPoint, Cursor::PosF()).draw(0.125, Palette::Red);

		}

		//Shiftの押されているとき
		if (KeyShift.pressed()) Window::ClientRect().drawFrame(32.0, Color(Palette::Red, 128));
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
Size	Machine::factorySize() const
{
	return game->factorySize;
}

void Main()
{
	Game game;

	while (System::Update()) game.update();
}
