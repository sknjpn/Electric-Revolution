#include"Machine.h"
#include"Factory.h"
#include"Blueprint.h"

void	Machine::initLua()
{

	lua.script_file(CharacterSet::Narrow(blueprint->mainPath + L"main.lua").c_str());
	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);

	lua["drawTexture"] = [this](const char* str)
	{
		texture(mainPath + CharacterSet::Widen(str))
			.resize(baseSize)
			.rotate(angle * 90_deg)
			.drawAt(region().center());
	};
	lua["drawTextureAt"] = [this](const char* str, double sizeX, double sizeY, double posX, double posY)
	{
		texture(mainPath + CharacterSet::Widen(str))
			.resize(sizeX, sizeY)
			.rotate(angle * 90_deg)
			.drawAt(transformInMachinePos(RectF(posX, posY, sizeX, sizeY).center()) + pos);
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
	lua["connectGearbox"] = [this](int gearbox0ID, int gearbox1ID)
	{
		gearboxes[gearbox0ID].connectedGearbox.emplace_back(&gearboxes[gearbox1ID]);
		gearboxes[gearbox1ID].connectedGearbox.emplace_back(&gearboxes[gearbox0ID]);
	};
	lua["setGearbox"] = [this](int x, int y, double resistance)
	{
		gearboxes.emplace_back(this, Point(x, y), resistance);
	};
	lua["setGearboxResistance"] = [this](int gearboxID, double resistance)
	{
		gearboxes[gearboxID].resistance = resistance;
	};
	lua["addGearboxPressure"] = [this](int gearboxID, double pressure)
	{
		gearboxes[gearboxID].pressure += pressure;
	};
	lua["getGearboxGain"] = [this](int gearboxID)
	{
		return gearboxes[gearboxID].gain;
	};
	lua["addMole"] = [this](int x, int y, double mole)
	{
		Point p(pos + transformInMachinePos(Point(x, y)));
		if (factory->getTile(p) != nullptr && factory->getTile(p)->pipe != nullptr)
		{
			factory->getTile(p)->pipe->plumber->mole += mole;
		}
	};
	lua["getPressure"] = [this](int x, int y)
	{
		Point p(pos + transformInMachinePos(Point(x, y)));
		if (factory->getTile(p) != nullptr && factory->getTile(p)->pipe != nullptr)
		{
			return factory->getTile(p)->pipe->plumber->pressure();
		}
		return 0.0;
	};
	lua["pullMole"] = [this](int x, int y, double mole)
	{
		Point p(pos + transformInMachinePos(Point(x, y)));
		if (factory->getTile(p) != nullptr && factory->getTile(p)->pipe != nullptr)
		{
			factory->getTile(p)->pipe->plumber->mole -= mole;
		}
	};
	lua["machineLeftClicked"] = [this]()
	{
		return region().leftClicked();
	};
	lua["machineLeftPressed"] = [this]()
	{
		return region().leftPressed();
	};
	lua["machineRightClicked"] = [this]()
	{
		return region().rightClicked();
	};
	lua["mouseRightPressed"] = []()
	{
		return MouseR.pressed();
	};
	lua["mouseLeftPressed"] = []()
	{
		return MouseL.pressed();
	};
	lua["cursorPosX"] = []()
	{
		return Cursor::Pos().x;
	};
	lua["cursorPosY"] = []()
	{
		return Cursor::Pos().y;
	};
	lua["playAudio"] = [this](const char* str)
	{
		if (!factory->isMain) return;

		auto a = audio(mainPath + CharacterSet::Widen(str));
		a.stop();
		a.play();
	};
	lua["addItem"] = [this](int x, int y, int id, int layer)
	{
		Point p(pos + transformInMachinePos(Point(x, y)));
		if (factory->getTile(p) != nullptr && factory->getTile(p)->items[layer] == nullptr) factory->addItem(p, id, layer);
	};
	lua["removeItem"] = [this](int x, int y, int layer)
	{
		Point p(pos + transformInMachinePos(Point(x, y)));
		if (factory->getTile(p) != nullptr && factory->getTile(p)->items[layer] != nullptr) factory->getTile(p)->items[layer]->remove();
	};
	lua["moveItem"] = [this](int fromX, int fromY, int _angle, double t, int layer)
	{
		Point p(pos + transformInMachinePos(Point(fromX, fromY)));
		if (factory->getTile(p) != nullptr && factory->getTile(p)->items[layer] != nullptr) factory->getTile(p)->items[layer]->move(p, (_angle + angle) % 4, t);
	};
	lua["isItemPos"] = [this](int x, int y, int layer)
	{
		Point p(pos + transformInMachinePos(Point(x, y)));
		if (factory->getTile(p) != nullptr && factory->getTile(p)->items[layer] != nullptr) return true;
		return false;
	};
	lua["machinePosX"] = [this]()
	{
		return pos.x;
	};
	lua["machinePosY"] = [this]()
	{
		return pos.y;
	};
	lua["machineAngle"] = [this]()
	{
		return angle;
	};
	lua["isTradeTile"] = [this](int x, int y)
	{
		Point p(pos + transformInMachinePos(Point(x, y)));
		return factory->getTile(p)->isTradeTile;
	};
	lua["setMachineSizeX"] = [this](int x)
	{
		baseSize.x = x;
	};
	lua["setMachineSizeY"] = [this](int y)
	{
		baseSize.y = y;
	};
	lua["machineAngle"] = 0;

	if (lua["init"].get_type() == sol::type::function) lua["init"]();
}