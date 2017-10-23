#include"Machine.h"
#include"Factory.h"

void	Machine::initLua()
{

	lua.script_file(CharacterSet::Narrow(blueprint->mainFile + L"main.lua").c_str());
	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
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
					.rotate(angle * 90_deg)
					.drawAt(region().center());
				return;
			}
		}
	};
	lua["drawTextureAt"] = [this](const char* str, double sizeX, double sizeY, double posX, double posY)
	{
		for (auto& ta : blueprint->textureAssets)
		{
			if (ta.first == CharacterSet::Widen(str))
			{
				ta.second
					.resize(sizeX, sizeY)
					.rotate(angle * 90_deg)
					.drawAt(transformedPos(RectF(posX, posY, sizeX, sizeY).center()) + pos);
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
		return region().leftClicked();
	};
	lua["machineRightClicked"] = [this]()
	{
		return region().rightClicked();
	};
	lua["playAudio"] = [this](const char* str)
	{
		const String& name = CharacterSet::Widen(str);
		for (auto& ta : audioAssets)
		{
			if (ta.first == name)
			{
				ta.second.stop();
				ta.second.play();
				return;
			}
		}

		auto dc = FileSystem::DirectoryContents(blueprint->mainFile);
		for (auto& c : dc)
		{
			if (c.removed(blueprint->mainFile) == name)
			{
				audioAssets.emplace_back(c.removed(blueprint->mainFile), Audio(c));
				audioAssets.back().second.play();
			}
		}
	};
	lua["addItem"] = [this](int x, int y, int id)
	{
		Point p(pos + transformedPos(Point(x, y)));
		if (factory->itemMap.at(p) == nullptr) Item::addItem(factory, p, id);
	};
	lua["removeItem"] = [this](int x, int y)
	{
		auto* p = factory->itemMap.at(pos + transformedPos(Point(x, y)));
		if (p != nullptr) p->erase();
	};
	lua["moveItem"] = [this](int fromX, int fromY, int _angle, double t)
	{
		const auto& p = pos + transformedPos(Point(fromX, fromY));
		if (factory->itemMap.at(p) != nullptr && factory->itemMap.at(p)->p == p)
		{
			factory->itemMap.at(p)->move((angle + _angle) % 4, t);
		}
	};
	lua["addKineticEnergy"] = [this](double value, int x, int y)
	{
		Point p(pos + transformedPos(Point(x, y)));
		if (p.x >= 0 && p.y >= 0 && p.x < factorySize().x && p.y < factorySize().y && factory->machineMap.at(p) != nullptr)
		{
			factory->keMap.at(p) += value;
		}
	};
	lua["getKineticEnergy"] = [this](int x, int y)
	{
		Point p(pos + transformedPos(Point(x, y)));
		if (p.x >= 0 && p.y >= 0 && p.x < factorySize().x && p.y < factorySize().y) return factory->keMap.at(p);
		else return 0.0;
	};
	lua["clearKineticEnergy"] = [this](int x, int y)
	{
		Point p(pos + transformedPos(Point(x, y)));
		if (p.x >= 0 && p.y >= 0 && p.x < factorySize().x && p.y < factorySize().y) factory->keMap.at(p) = 0;
	};
	lua["isItemPos"] = [this](int x, int y)
	{
		if (factory->itemMap.at(pos + transformedPos(Point(x, y))) != nullptr) return true;
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
	lua["isWallTile"] = [this](int x, int y)
	{
		auto p = pos + transformedPos(Point(x, y));
		return p.x == -1 || p.y == -1 || p.x == factorySize().x || p.y == factorySize().y;
	};

	lua["machineSizeX"] = blueprint->size.x;
	lua["machineSizeY"] = blueprint->size.y;
	lua["machineAngle"] = 0;

	if (lua["init"].get_type() == sol::type::function) lua["init"]();
}