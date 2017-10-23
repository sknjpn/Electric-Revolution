#include"UI.h"

UI::UI()
	: uiMode(UIMode::None)
{}

void	UI::update()
{
	if (KeyW.down())
	{
		if (uiMode == UIMode::EditWireMode) uiMode = UIMode::None;
		else uiMode = UIMode::EditWireMode;
	}
	if (KeyE.down())
	{
		if (uiMode == UIMode::EditMachineMode) uiMode = UIMode::None;
		else uiMode = UIMode::EditMachineMode;
	}

	switch (uiMode)
	{
	case UIMode::None:
	{

	}
	break;
	case UIMode::EditWireMode:
	{
		Window::ClientRect().draw(Color(Palette::Red, 32));
	}
	break;
	case UIMode::EditMachineMode:
	{
		Window::ClientRect().draw(Color(Palette::Green, 32));

	}
	break;
	default:
		break;
	}
}