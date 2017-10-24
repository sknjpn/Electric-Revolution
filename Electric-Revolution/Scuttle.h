#pragma once

struct Button
{
	bool	isGlabed;
	Rect	rect;
	Texture	texture;


	Button(const Rect& _rect)
		: rect(_rect)
		, isGlabed(false)
	{}
	Button(const Rect& _rect, const Texture& _texture)
		: rect(_rect)
		, texture(_texture)
		, isGlabed(false)
	{}

	void	update()
	{
		if (rect.leftClicked()) isGlabed = true;
		if (MouseL.up()) isGlabed = false;
	}
	bool	isClicked() const
	{
		return MouseL.up() && rect.mouseOver();
	}
};

struct Scuttle
{
	bool	isGlabed;
	Rect	rect;
	Texture	texture;

	Array<Button>	buttons;

	Scuttle(const Rect& _rect)
		: isGlabed(false)
		, rect(_rect)
	{}
	Scuttle(const Rect& _rect, const Texture& _texture)
		: isGlabed(false)
		, rect(_rect)
		, texture(_texture)
	{}

	void	update()
	{
		for (auto& b : buttons) b.update();
	}
	bool	mouseOver() const
	{
		return rect.mouseOver();
	}
};