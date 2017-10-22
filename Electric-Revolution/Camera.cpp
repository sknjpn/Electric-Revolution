#include"Camera.h"

Camera::Camera()
	: restrictedRegion(Vec2(-2048, -2048), 3 * Vec2(2048, 2048))
	, drawingRegion(0, 0, 32, 32)
	, smoothDrawingRegion(drawingRegion)
{}
void	Camera::update()
{
	{
		auto t = createTransformer2D();

		double delta = Mouse::Wheel();
		drawingRegion = drawingRegion.scaledAt(Cursor::PosF(), 1.0 + 0.1*delta);
		const double followingSpeed = 0.2;
		smoothDrawingRegion.pos = smoothDrawingRegion.pos*(1.0 - followingSpeed) + drawingRegion.pos*followingSpeed;
		smoothDrawingRegion.size = smoothDrawingRegion.size*(1.0 - followingSpeed) + drawingRegion.size*followingSpeed;
		
	}

	if (Window::GetState().focused)
	{
		const double slidingSpeed = (drawingRegion.size.y / 180_deg)*0.05;
		const bool useKeyViewControl = true;

		if ((useKeyViewControl && KeyA.pressed()) || Cursor::Pos().x <= 0) drawingRegion.pos.x -= slidingSpeed;
		if ((useKeyViewControl && KeyW.pressed()) || Cursor::Pos().y <= 0) drawingRegion.pos.y -= slidingSpeed;
		if ((useKeyViewControl && KeyD.pressed()) || Cursor::Pos().x >= Window::Size().x - 1) drawingRegion.pos.x += slidingSpeed;
		if ((useKeyViewControl && KeyS.pressed()) || Cursor::Pos().y >= Window::Size().y - 1) drawingRegion.pos.y += slidingSpeed;
	}
}
Transformer2D	Camera::createTransformer2D(double _magnification) const
{
	auto mat3x2 = Mat3x2::Scale(_magnification).translated(-smoothDrawingRegion.center()).scaled(Window::Size().y / smoothDrawingRegion.size.y).translated(Window::ClientRect().center());

	return Transformer2D(mat3x2, true);
}