#pragma once

struct Factory;
struct Pipe;

struct Plumber
{
	int	numRegistered;
	bool	enabled;
	double	mole;	//ï®éøó 
	double	volume;	//óeêœ
	Factory*	factory;

	Plumber(Factory* _factory);

	void	addPipe(Pipe* _pipe);
	void	removePipe(Pipe* _pipe);
	double	pressure() const;	//à≥óÕ
};

struct Pipe
{
	bool enabled;
	Point	pos;
	Plumber*	plumber;
	Factory*	factory;

	Pipe(Factory* _factory);
	void	setPos(const Point& _pos);
	Array<Pipe*>	nearPipes();
};