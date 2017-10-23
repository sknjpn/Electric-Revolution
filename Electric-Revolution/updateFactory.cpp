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
		bool flag = false;
		for (auto& m : _f->machines)
		{
			if (m.updateConnects()) flag = true;
		}
		for (auto& w : _f->wires)
		{
			if (w.update()) flag = true;
		}
		_f->wires.remove_if([](Wire& w) { return w.broken; });
		if (!flag) break;
	}

	//機械固有処理
	for (auto& m : _f->machines) m.updateSystem();
}