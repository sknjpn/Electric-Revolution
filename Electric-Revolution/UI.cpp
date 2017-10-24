#include"UI.h"
#include"Game.h"

UI::UI(Game* _game)
	: uiMode(UIMode::None)
	, uiTexture(L"assets/ui.png")
	, game(_game)
	, selectedGroup(nullptr)
	, trashAreaMouseOver(false)
	, trashBox(L"assets/trashBox.png")
{}

void	UI::update()
{
	trashAreaMouseOver = false;

	auto* g = game;
	auto* f = game->mainFactory;
	{
		Rect(480, 48).draw(Color(80)).drawFrame(4, Color(60));
		g->font(32)(f->name).draw(2, 2, Color(240));
	}
	{
		Rect(0, 48, 240, 32).draw(Color(80)).drawFrame(4, Color(60));
		for (int i = 0; i < 3; i++)
		{
			Rect rect(16 + i * 48, 48, 32, 32);
			if (int(uiMode) - 1 == i || Rect(16 + i * 48, 48, 32, 32).mouseOver())
			{
				uiTexture(32 * i, 32, 32, 32).draw(rect.pos);

				if (rect.leftClicked())
				{
					if (int(uiMode) - 1 == i) uiMode = UIMode::None;
					else uiMode = UIMode(i + 1);
				}
			}
			else uiTexture(32 * i, 0, 32, 32).draw(rect.pos);
		}
	}
	switch (uiMode)
	{
	case UIMode::None:
		break;
	case UIMode::EditMachineMode:

		if (selectedGroup == nullptr) selectedGroup = &Machine::groups.front();
		for (int i = 0; i < int(Machine::groups.size()); i++)
		{
			auto& gr = Machine::groups[i];
			Rect rect1(0, 96 + i * 32, 240, 24);
			rect1.draw(rect1.mouseOver() ? Color(160) : &gr == selectedGroup ? Color(120) : Color(80)).drawFrame(2, Color(60));
			g->font(16)(gr.name).draw(rect1.pos.movedBy(0, 0));
			if (rect1.leftClicked()) selectedGroup = &gr;
			if (&gr == selectedGroup)
			{
				for (int j = 0; j < int(gr.blueprints.size()); j++)
				{
					auto& b = gr.blueprints[j];

					Rect rect2(256, 96 + i * 32 + j * 32, 240, 24);
					rect2.draw(rect2.mouseOver() ? Color(160) : Color(80)).drawFrame(2, Color(60));
					g->font(16)(b->name).draw(rect2.pos.movedBy(0, 0));
					if (rect2.leftClicked())
					{
						auto& m = g->mainFactory->machines.emplace_back(b, g->mainFactory);
						Machine::selectedMachine = &m;
						Machine::newMachineAngle = m.angle;
						Machine::newMachineRegion = m.region();
					}
				}
			}
		}

		if (Machine::selectedMachine != nullptr)
		{
			if (trashArea.mouseOver()) trashAreaMouseOver = true;
			trashArea = Rect(Window::Size().x - 128, 32, 96, 96);
			trashBox(64 * trashArea.mouseOver(), 0, 64, 64).resize(trashArea.size).draw(trashArea.pos);
		}

		break;
	case UIMode::EditWireMode:

		break;
	default:
		break;
	}
}