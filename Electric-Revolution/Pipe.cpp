#include"Pipe.h"
#include"Factory.h"

Plumber::Plumber(Factory* _factory)
	: enabled(true)
	, mole(0.0)
	, volume(0.0)
	, factory(_factory)
	, numRegistered(0)
{}

void	Plumber::addPipe(Pipe* _pipe)
{
	if (_pipe->plumber == this) return;
	if (_pipe->plumber != nullptr)
	{
		mole += _pipe->plumber->pressure()*10.0;
		_pipe->plumber->mole -= _pipe->plumber->pressure()*10.0;
		_pipe->plumber->numRegistered--;
	}
	enabled = true;
	_pipe->plumber = this;
	numRegistered++;
	volume += 10.0;
}

void	Plumber::removePipe(Pipe* _pipe)
{
	numRegistered--;
	mole -= pressure()*10.0;
	volume -= 10.0;
	_pipe->plumber = nullptr;
	if (numRegistered == 0) enabled = false;
}
double	Plumber::pressure() const
{
	return mole / volume;
}

Pipe::Pipe(Factory* _factory)
	: enabled(true)
	, factory(_factory)
{}

void	Pipe::setPos(const Point& _pos)
{
	pos = _pos;

	factory->getTile(pos)->pipe = this;

	//Tile‚É“o˜^&Plumber‚ğ“o˜^
	auto pipes = nearPipes();
	for (auto* np : pipes)
	{
		if (plumber != nullptr)
		{
			auto* tarPlumber = np->plumber;
			for (auto& p : factory->pipes)
			{
				if (p.enabled && p.plumber == tarPlumber) plumber->addPipe(&p);
			}
		}
		else np->plumber->addPipe(this);
	}
	if (plumber == nullptr)
	{
		auto* p = factory->newPlumber();
		p->enabled = true;
		p->addPipe(this);
	}
}
Array<Pipe*>	Pipe::nearPipes()
{
	Array<Pipe*> pipes;
	Tile* tile = nullptr;
	tile = factory->getTile(pos.movedBy(1, 0));
	if (tile != nullptr && tile->pipe != nullptr) pipes.emplace_back(tile->pipe);
	tile = factory->getTile(pos.movedBy(0, 1));
	if (tile != nullptr && tile->pipe != nullptr) pipes.emplace_back(tile->pipe);
	tile = factory->getTile(pos.movedBy(-1, 0));
	if (tile != nullptr && tile->pipe != nullptr) pipes.emplace_back(tile->pipe);
	tile = factory->getTile(pos.movedBy(0, -1));
	if (tile != nullptr && tile->pipe != nullptr) pipes.emplace_back(tile->pipe);
	return pipes;
}