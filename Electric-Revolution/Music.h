#pragma once

struct Music
{
	Audio	audio;

	Music();

	void	setMusic(const FilePath& _path);
	void	resetMusic();
};