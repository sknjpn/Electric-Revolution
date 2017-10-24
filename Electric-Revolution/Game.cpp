#include"Game.h"
#include"Factory.h"

Game::Game()
	: mainFactory(nullptr)
	, camera()
	, ui(this)
{
	Window::Resize(1280, 720);
	//Graphics::SetFullScreen(true, Graphics::EnumOutputs().front().displayModes.back().size);
	Window::SetTitle(L"Electric Revolution");
	Graphics::SetBackground(Palette::Skyblue);

	auto dc = FileSystem::DirectoryContents(L"assets/machines/");
	Machine::blueprints.reserve(dc.size());
	Machine::groups.reserve(dc.size());
	for (auto c : dc)
	{
		if (c.includes(L"config.ini"))
		{
			auto& b = Machine::blueprints.emplace_back(c.removed(L"config.ini"));

			INIReader ini(c);
			for (auto& g : Machine::groups)
			{
				if (g.name == ini.get<String>(L"Base.group"))
				{
					g.blueprints.emplace_back(&b);
					b.group = &g;
					break;
				}
			}
			if (b.group == nullptr)
			{
				b.group = &Machine::groups.emplace_back(ini.get<String>(L"Base.group"));
				b.group->blueprints.emplace_back(&b);
			}
		}
	}
	for (auto c : FileSystem::DirectoryContents(L"assets/items/")) Item::textureAssets.emplace_back(c);

	for (auto& b : Machine::blueprints) Output << b.group->name << b.name;

	timer.start();
	bgm = Audio(L"assets/bgm/星くずの行進.mp3");
	bgm.setLoop(true);
	bgm.play();

	scene[GameState::Title] = [this]()
	{
		font(128)(L"Electric Revolution").drawAt(Window::ClientRect().center());

		if (MouseL.down())
		{
			bgm = Audio(L"assets/bgm/アーケード街.mp3");
			bgm.setLoop(true);
			bgm.play();
			scene.changeState(GameState::MapView);

			camera.restrictedRegion.set(Rect(1280, 720));
			camera.drawingRegion.set(camera.restrictedRegion);
			camera.smoothDrawingRegion.set(camera.drawingRegion);
			camera.magnificationMax = 4.0;
			camera.magnificationMin = 1.0;
		}
	};
	scene[GameState::MapView] = [this]()
	{
		camera.update();
		auto t = camera.createTransformer2D();

		ClearPrint();

		map.texture.draw();

		for (auto& u : map.urbans)
		{

			if (Circle(u.pos, 6).mouseOver())
			{
				Circle(u.pos, 4).draw(Palette::Red).drawFrame(1, Palette::Black);

				if (MouseL.down())
				{
					scene.changeState(GameState::FactoryView);
					ui.uiMode = UIMode::None;

					mainFactory = &u.factory;
					mainFactory->isMain = true;

					int ow = 30;	//芝生の長さ
					camera.restrictedRegion.set(Size(-ow, -ow), mainFactory->size + Size(ow * 2, ow * 2));
					camera.drawingRegion.set(RectF(Window::ClientRect()).scaledAt(Vec2::Zero(), 0.05).setCenter(mainFactory->size / 2));
					camera.smoothDrawingRegion.set(camera.drawingRegion);
					camera.magnificationMax = 64.0;
					camera.magnificationMin = 16.0;
				}
			}
			else
			{
				Circle(u.pos, 4).draw(Palette::White).drawFrame(1, Palette::Black);
			}
			font(16)(u.name).drawAt(u.pos.movedBy(0, -12), Palette::Black);
		}
	};
	scene[GameState::FactoryView] = [this]()
	{
		ClearPrint();

		//カメラ処理
		camera.update();

		for (auto& u : map.urbans)
		{
			updateFactory(&u.factory);
		}
		drawFactory(mainFactory);

		ui.update();

		if (ui.uiMode == UIMode::ReturnToMap)
		{
			mainFactory->isMain = false;
			mainFactory = nullptr;
			Machine::selectedMachine = nullptr;
			scene.changeState(GameState::MapView);

			camera.restrictedRegion.set(Rect(1280, 720));
			camera.drawingRegion.set(camera.restrictedRegion);
			camera.smoothDrawingRegion.set(camera.drawingRegion);
			camera.magnificationMax = 4.0;
			camera.magnificationMin = 1.0;
		}
	
	};
}
Font&	Game::font(int _size)
{
	for (; int(fontAssets.size()) <= _size;)
	{
		fontAssets.emplace_back(int(fontAssets.size()) + 1);
	}
	return fontAssets[_size];
}