#pragma once

enum struct UIMode
{
	None,
	EditWireMode,
	EditMachineMode,
};

struct UI
{
	UIMode	uiMode;

	UI();

	void	update();
};