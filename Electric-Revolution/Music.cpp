#include"Music.h"


Music::Music()
	: volume(1.0)
{}

void	Music::setMusic(const FilePath& _path)
{
	audio = Audio(_path);
	audio.setLoop(true);
	audio.setVolume(volume);
	audio.play();
}

void	Music::resetMusic()
{
	audio.stop();
}

void	Music::setVolume(double _volume)
{
	volume = _volume;
	audio.setVolume(volume);
}