#include"UI.h"
#include"Blueprint.h"
#include"Factory.h"

Forklift::Forklift(Factory* _factory)
	: enabled(false)
	, angle(0)
	, region(0, 0, 0, 0)
	, selectedMachine(nullptr)
	, factory(_factory)
	, baseSize(0, 0)
{}

void	Forklift::update()
{
	if (!enabled) return;

	if (KeyR.down())
	{
		angle = (angle + 1) % 4;
		region.set(region.pos, region.h, region.w);
	}
	if (MouseL.up())
	{
		if (trashboxRegion().intersects(Graphics2D::GetTransformLocal().transform(Cursor::PosF())))
		{
			if (selectedMachine != nullptr) selectedMachine->remove();
		}
		else if (canPutMachine())
		{
			if (selectedMachine != nullptr) selectedMachine->moveTo(region.pos, angle);
			else factory->newMachine()->set(blueprint, region.pos, angle);
		}
		enabled = false;
		return;
	}

	region = RectF(region).setCenter(Cursor::PosF().movedBy(0.5, 0.5));
	baseTexture.resize(baseSize).rotate(angle * 90_deg).drawAt(region.pos + region.size / 2.0, Color(Palette::White, 128));

	if (canPutMachine()) region.draw(Color(Palette::Orange, 128)).drawFrame(1 / 16.0, Palette::Orange);
	else region.draw(Color(Palette::Red, 128)).drawFrame(1 / 16.0, Palette::Red);

}

void	Forklift::set(Machine* _sm)
{
	blueprint = _sm->blueprint;
	angle = _sm->angle;
	enabled = true;
	baseSize = _sm->baseSize;
	region.set(_sm->region());
	baseTexture = _sm->baseTexture;
	selectedMachine = _sm;
}

void	Forklift::set(Blueprint* _blueprint)
{
	blueprint = _blueprint;
	angle = 0;
	enabled = true;
	baseSize = blueprint->baseSize();
	region.set(Point(0, 0), baseSize);
	baseTexture = blueprint->baseTexture();
	selectedMachine = nullptr;
}

bool	Forklift::canPutMachine() const
{
	for (auto p : step(region.pos, region.size))
	{
		if (p.x < 0 || p.y < 0 || p.x >= factory->size.x || p.y >= factory->size.y ||
			!factory->tiles[p.y][p.x].canPutMachine ||
			(factory->tiles[p.y][p.x].machine != nullptr && factory->tiles[p.y][p.x].machine != selectedMachine))
		{
			return false;
		}
	}
	return true;
}

UI::UI(Factory* _factory)
	: uiMode(UIMode::None)
	, uiTexture(L"assets/uiTexture.png")
	, factory(_factory)
	, selectedGroup(nullptr)
	, mouseOver(false)
{}