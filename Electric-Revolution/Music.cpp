#include"Music.h"


Music::Music()
{}

void	Music::setMusic(const FilePath& _path)
{
	audio = Audio(_path);
	audio.setLoop(true);
	audio.play();
}

void	Music::resetMusic()
{
	audio.stop();
}