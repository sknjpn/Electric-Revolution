#include"Game.h"

void	Game::updateFactory(Factory* _f)
{
	//信号初期化処理
	for (auto& m : _f->machines)
	{
		for (auto& n : m.nodes)
		{
			if (!n.fixed) n.state = NodeState::None;
		}
	}

	//信号伝達処理
	for (;;)
	{
		for (auto& m : _f->machines)
		{
			if (m.lua["updateConnects"].get_type() == sol::type::function) m.lua["updateConnects"]();
		}

		_f->wires.remove_if([](Wire& w) { return w.n0->state != w.n1->state && (w.n0->state != NodeState::None && w.n1->state != NodeState::None); });

		bool flag = true;
		for (auto& w : _f->wires)
		{
			if (w.n0->state == w.n1->state) continue;
			if (w.n0->state == NodeState::None) w.n0->state = w.n1->state;
			else w.n1->state = w.n0->state;
			flag = false;
		}
		if (flag) break;
	}

	//Gearbox
	Array<Gearbox*>	gs;
	gs.reserve(1024);
	for (auto& m : _f->machines)
	{
		for (auto& g : m.gearboxes)
		{
			g.gain = 0.0;
		}
	}
	for (auto& m : _f->machines)
	{
		for (auto& g : m.gearboxes)
		{
			if (g.pressure > 0.0)
			{
				gs.clear();
				double totalR = g.resistance;
				double totalP = g.pressure;
				g.pressure = 0;
				gs.emplace_back(&g);
				for (;;)
				{
					bool flag = true;
					for (auto* s : gs)
					{
						for (auto* cg : s->connectedGearbox)
						{
							if (!gs.include(cg))
							{
								totalR += cg->resistance;
								totalP += cg->pressure;
								cg->pressure = 0.0;
								gs.emplace_back(cg);
								flag = false;
							}
						}
					}
					if (flag)
					{
						for (auto* s : gs) s->gain += s->resistance * totalP / totalR;
						break;
					}
				}
			}
		}
	}

	//機械固有処理
	for (auto& m : _f->machines)
	{
		if (m.lua["updateSystem"].get_type() == sol::type::function) m.lua["updateSystem"]();
	}
}