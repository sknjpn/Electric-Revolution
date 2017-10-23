#include"Game.h"
#include"Factory.h"

Game::Game()
	: mainFactory(nullptr)
	, camera()
{
	//Window::Resize(1280, 720);
	Graphics::SetFullScreen(true, Graphics::EnumOutputs().front().displayModes.back().size);
	Window::SetTitle(L"Electric Revolution");
	Graphics::SetBackground(Palette::Darkkhaki);

	for (auto c : FileSystem::DirectoryContents(L"assets/machines/"))
	{
		if (c.includes(L"config.ini")) Machine::blueprints.emplace_back(c.removed(L"config.ini"));
	}
	for (auto c : FileSystem::DirectoryContents(L"assets/items/")) Item::textureAssets.emplace_back(c);

	Size factorySize(64, 64);
	mainFactory = &factories.emplace_back(factorySize);
	camera.restrictedRegion.set(Size(-10, -10), factorySize + Size(20, 20));
	camera.drawingRegion.set(RectF(Window::ClientRect()).scaledAt(Vec2::Zero(), 0.05).setCenter(factorySize / 2));
	camera.smoothDrawingRegion.set(camera.drawingRegion);

}

void	Game::update()
{
	//Graphics2D::SetSamplerState(SamplerState::ClampAniso);
	ClearPrint();
	Print << L"‘I‘ð’†‚ÉRƒL[‚Å‰ñ“]‚Å‚«‚Ü‚·B";
	Print << L"ƒJƒEƒ“ƒ^‚Í¶‰º‚Ìƒsƒ“‚ª“ü—Í‚Å‚·";
	Print << L"‹@ŠB‚ðˆÚ“®‚·‚é‚Æ‚«‚ÍShift‚ð‰Ÿ‚µ‚È‚ª‚ç¶ƒNƒŠƒbƒN‚µ‚Ä‚­‚¾‚³‚¢";
	//ƒJƒƒ‰ˆ—
	camera.update();
	{
		auto t = camera.createTransformer2D();
		auto* f = mainFactory;
		//M†‰Šú‰»ˆ—
		for (auto& m : f->machines)
		{
			for (auto& n : m.nodes)
			{
				if (!n.fixed) n.state = NodeState::None;
			}
		}

		//M†“`’Bˆ—
		for (;;)
		{
			bool flag = false;
			for (auto& m : f->machines)
			{
				if (m.updateConnects()) flag = true;
			}
			for (auto& w : f->wires)
			{
				if (w.update()) flag = true;
			}
			f->wires.remove_if([](Wire& w) { return w.broken; });
			if (!flag) break;
		}

		//‹@ŠBŒÅ—Lˆ—
		for (auto& m : f->machines) m.updateSystem();

		//‹@ŠB‚Ì‰¹—Ê§Œä
		for (auto& m : f->machines)
		{
			Vec3 pos1(camera.drawingRegion.center().x, camera.drawingRegion.center().y, camera.drawingRegion.w / 2.0);
			Vec3 pos2(m.region().center().x, m.region().center().y, 0.0);
			for (auto& a : m.audioAssets)
			{
				a.second.setVolume(Min(0.5, 100.0 / (pos1 - pos2).lengthSq()));
			}
		}


		//”wŒi•`‰æ
		{
			const auto& r = camera.smoothDrawingRegion;
			Point drawPoint(int(r.x) - 1, int(r.y) - 1);
			Point drawSize(int(r.w) + 3, int(r.h) + 3);
			Print << drawPoint;
			for (auto p : step(drawPoint, drawSize))
			{
				if (p.x < -1 || p.y < -1 || p.x > f->size.x || p.y > f->size.y)
				{
					f->tile(32, 0, 16, 16).resize(1.0, 1.0).draw(p);
				}
				else if (p.x == -1 || p.y == -1 || p.x == f->size.x || p.y == f->size.y)
				{
					f->tile(16, 0, 16, 16).resize(1.0, 1.0).draw(p);
				}
				else f->tile(0, 0, 16, 16).resize(1.0, 1.0).draw(p);
			}
		}

		//‹@ŠB•`‰æ
		for (auto& m : f->machines) m.draw();

		//ƒAƒCƒeƒ€•`‰æ
		for (auto& i : f->items) i.draw();

		for (auto& m : f->machines)
		{
			for (auto& ta : m.blueprint->textureAssets)
			{
				if (ta.first == L"coverImage.png")
				{
					ta.second
						.resize(m.blueprint->size)
						.rotate(m.angle * 90_deg)
						.drawAt(m.region().center());
					break;
				}
			}
		}
		//ƒm[ƒh‚à‚µ‚­‚Í‹@ŠB‚Ì‘I‘ð
		if (MouseL.down())
		{
			if (KeyShift.pressed())
			{
				for (auto& m : f->machines)
				{
					if (m.region().leftClicked())
					{
						Machine::newMachineRegion = m.region();
						Machine::newMachineAngle = m.angle;
						Machine::selectedMachine = &m;
					}
				}
			}
			else Node::selectedNode = f->mouseOverNode();
		}

		//‘I‘ð‚³‚ê‚½‹@ŠB‚Ìˆ—
		if (Machine::selectedMachine != nullptr)
		{
			auto* sm = Machine::selectedMachine;
			Machine::newMachineRegion = RectF(Machine::newMachineRegion).setCenter(Cursor::PosF().movedBy(0.5, 0.5));

			if (KeyR.down())
			{
				Machine::newMachineRegion.size.set(Machine::newMachineRegion.size.y, Machine::newMachineRegion.size.x);
				Machine::newMachineAngle = (Machine::newMachineAngle + 1) % 4;
			}
			if (MouseL.up() || KeyShift.up())
			{
				bool flag = !Machine::newMachineRegion.pos.intersects(Rect(f->size)) || !Machine::newMachineRegion.br().movedBy(-1, -1).intersects(Rect(f->size));
				for (auto& m : f->machines)
				{
					if (&m != sm && m.region().intersects(Machine::newMachineRegion)) flag = true;
				}

				if (!flag)
				{

					for (auto& p : step(sm->region().pos, sm->region().size))
					{
						if (f->itemMap.at(p) != nullptr) f->itemMap.at(p)->erase();
						f->machineMap.at(p) = nullptr;
						f->keMap.at(p) = 0;
					}
					sm->angle = Machine::newMachineAngle;
					sm->pos = Machine::newMachineRegion.pos;

					for (auto& p : step(sm->region().pos, sm->region().size))
					{
						f->machineMap.at(p) = sm;
						f->keMap.at(p) = 0;
					}
				}
				Machine::selectedMachine = nullptr;
			}
			else
			{
				bool flag = !Machine::newMachineRegion.pos.intersects(Rect(f->size)) || !Machine::newMachineRegion.br().movedBy(-1, -1).intersects(Rect(f->size));
				for (auto& m : f->machines)
				{
					if (&m != sm && m.region().intersects(Machine::newMachineRegion)) flag = true;
				}

				sm->blueprint->texture(L"image.png")
					->resize(sm->blueprint->size)
					.rotate(Machine::newMachineAngle * 90_deg)
					.drawAt(Machine::newMachineRegion.center(), Color(255, 128));

				if (flag) Machine::newMachineRegion.draw(Color(Palette::Red, 128)).drawFrame(0.1, 0, Palette::Red);
				else Machine::newMachineRegion.draw(Color(Palette::Orange, 128)).drawFrame(0.1, 0, Palette::Orange);
			}
		}
		else if (KeyShift.pressed())
		{
			//‹@ŠB‚É‘I‘ð—\’è‚Ì•\Ž¦
			for (auto& m : f->machines)
			{
				if (m.region().mouseOver()) m.region().draw(Color(Palette::Green, 128));
			}
		}
		for (auto& w : f->wires) w.draw();

		//‘I‘ð‚³‚ê‚½ƒm[ƒh‚Ìˆ—
		if (Node::selectedNode != nullptr)
		{
			if (MouseL.up())
			{
				auto* node = f->mouseOverNode();

				if (node != nullptr && node != Node::selectedNode)
				{
					f->wires.emplace_back(node, Node::selectedNode);
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

		//”züíœ
		if (MouseR.down()) rightClickedPoint = Cursor::PosF();
		if (MouseR.up())
		{
			for (auto& w : f->wires)
			{
				if (Line(w.from->pos(), w.to->pos()).intersects(Line(rightClickedPoint, Cursor::PosF()))) w.broken = true;
			}
		}
		if (MouseR.pressed()) Line(rightClickedPoint, Cursor::PosF()).draw(0.125, Palette::Red);

	}

	//Shift‚Ì‰Ÿ‚³‚ê‚Ä‚¢‚é‚Æ‚«
	if (KeyShift.pressed()) Window::ClientRect().drawFrame(32.0, Color(Palette::Red, 128));
}
