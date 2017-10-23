#include"Game.h"

void	Game::drawFactory(Factory* _f)
{
	{
		//Ž‹“_ˆÚ“®
		auto t = camera.createTransformer2D();

		//‹@ŠB‚Ì‰¹—Ê§Œä
		for (auto& m : _f->machines)
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
			for (auto p : step(drawPoint, drawSize))
			{
				if (p.x < -1 || p.y < -1 || p.x > _f->size.x || p.y > _f->size.y)
				{
					_f->tile(32, 0, 16, 16).resize(1.0, 1.0).draw(p);
				}
				else if (p.x == -1 || p.y == -1 || p.x == _f->size.x || p.y == _f->size.y)
				{
					_f->tile(16, 0, 16, 16).resize(1.0, 1.0).draw(p);
				}
				else _f->tile(0, 0, 16, 16).resize(1.0, 1.0).draw(p);
			}
		}

		//‹@ŠB•`‰æ
		for (auto& m : _f->machines)
		{
			m.draw();

			if (ui.uiMode == UIMode::EditWireMode)
			{
				for (auto& n : m.nodes) n.draw();
			}
		}


		//ƒAƒCƒeƒ€•`‰æ
		for (auto& i : _f->items) i.draw();

		for (auto& m : _f->machines)
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
			if (ui.uiMode == UIMode::EditMachineMode)
			{
				//‘I‘ð‚Ì•`‰æ
				if (Machine::selectedMachine != nullptr)
				{
					Machine::selectedMachine->region().draw(Color(Palette::Orange, 128));
				}
				for (auto& m : _f->machines)
				{
					if (m.region().leftClicked())
					{
						Machine::newMachineRegion = m.region();
						Machine::newMachineAngle = m.angle;
						Machine::selectedMachine = &m;
					}
				}
			}
			if (ui.uiMode == UIMode::EditWireMode)
			{
				Node::selectedNode = _f->mouseOverNode();
			}
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
			if (MouseL.up() || ui.uiMode != UIMode::EditMachineMode)
			{
				bool flag = !Machine::newMachineRegion.pos.intersects(Rect(_f->size)) || !Machine::newMachineRegion.br().movedBy(-1, -1).intersects(Rect(_f->size));
				for (auto& m : _f->machines)
				{
					if (&m != sm && m.region().intersects(Machine::newMachineRegion)) flag = true;
				}

				if (!flag)
				{

					for (auto& p : step(sm->region().pos, sm->region().size))
					{
						if (_f->itemMap.at(p) != nullptr) _f->itemMap.at(p)->erase();
						_f->machineMap.at(p) = nullptr;
						_f->keMap.at(p) = 0;
					}
					sm->angle = Machine::newMachineAngle;
					sm->pos = Machine::newMachineRegion.pos;

					for (auto& p : step(sm->region().pos, sm->region().size))
					{
						_f->machineMap.at(p) = sm;
						_f->keMap.at(p) = 0;
					}
				}
				Machine::selectedMachine = nullptr;
			}
			else
			{
				bool flag = !Machine::newMachineRegion.pos.intersects(Rect(_f->size)) || !Machine::newMachineRegion.br().movedBy(-1, -1).intersects(Rect(_f->size));
				for (auto& m : _f->machines)
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
		else if (ui.uiMode == UIMode::EditMachineMode)
		{
			//‹@ŠB‚É‘I‘ð—\’è‚Ì•\Ž¦
			for (auto& m : _f->machines)
			{
				if (m.region().mouseOver()) m.region().draw(Color(Palette::Green, 128));
			}
		}
		for (auto& w : _f->wires) w.draw();

		//‘I‘ð‚³‚ê‚½ƒm[ƒh‚Ìˆ—
		if (Node::selectedNode != nullptr)
		{
			auto* sn = Node::selectedNode;
			if (MouseL.up() || ui.uiMode != UIMode::EditWireMode)
			{
				auto* node = _f->mouseOverNode();

				if (node != nullptr && node != sn)
				{
					_f->wires.emplace_back(node, sn);
				}
				Node::selectedNode = nullptr;
			}
			else
			{
				Color color = Color(0, 0);
				switch (sn->state)
				{
				case NodeState::Hi: color = Palette::Red;	break;
				case NodeState::Low: color = Palette::Blue;	break;
				case NodeState::None: color = Palette::White;	break;
				}

				const auto& p0 = sn->pos();
				const auto& p1 = sn->pos() + sn->dv.rotated(sn->parentMachine->angle * 90_deg);
				const auto& p2 = Cursor::PosF();
				const auto& p3 = Cursor::PosF();
				Bezier3(p0, p1, p2, p3).draw(0.25, Palette::Black);
				Bezier3(p0, p1, p2, p3).draw(0.125, color);
			}
		}

		//”züíœ
		if (ui.uiMode == UIMode::EditWireMode)
		{
			if (MouseR.pressed())
			{
				for (auto& w : _f->wires)
				{
					auto ls = w.line().getLineString();
					for (int i = 0; i < int(ls.size() - 1); i++)
					{
						if (Line(ls[i], ls[i + 1]).intersects(Line(rightClickedPoint, Cursor::PosF())))
						{
							w.line().draw(0.250, Palette::Red);
							break;
						}
					}
				}
			}
			if (MouseR.down()) rightClickedPoint = Cursor::PosF();
			if (MouseR.up())
			{
				for (auto& w : _f->wires)
				{
					auto ls = w.line().getLineString();
					for (int i = 0; i < int(ls.size() - 1); i++)
					{
						if (Line(ls[i], ls[i + 1]).intersects(Line(rightClickedPoint, Cursor::PosF())))
						{
							w.broken = true;
							break;
						}
					}
				}
			}
			if (MouseR.pressed()) Line(rightClickedPoint, Cursor::PosF()).draw(0.125, Palette::Red);
		}
	}


}
