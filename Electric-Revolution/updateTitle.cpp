#include"Game.h"

void	Game::updateTitle()
{
	font(128)(L"Electric Revolution").drawAt(Window::Center());

	if (MouseL.down())
	{
		music.setMusic(L"assets/music/アーケード街.mp3");
		sceneState = SceneState::MapView;
	}
}