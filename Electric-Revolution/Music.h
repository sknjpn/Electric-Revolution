#pragma once

class Music
{
	Audio	audio;
	double	volume;

public:
	Music();

	void	setMusic(const FilePath& _path);
	void	resetMusic();
	void	setVolume(double _volume);
};