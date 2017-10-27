#include"Camera.h"

Camera::Camera(RectF restrictedRegion, RectF drawingRegion, double mMax, double mMin)
	: restrictedRegion(restrictedRegion)
	, targetDrawingRegion(drawingRegion)
	, smoothDrawingRegion(drawingRegion)
	, magnificationMax(mMax)
	, magnificationMin(mMin)
{}

Camera::Camera(RectF _restrictedRegion, double mMax, double mMin)
	: restrictedRegion(_restrictedRegion)
	, targetDrawingRegion(_restrictedRegion)
	, smoothDrawingRegion(_restrictedRegion)
	, magnificationMax(mMax)
	, magnificationMin(mMin)
{}

void	Camera::update()
{
	if (!Window::GetState().focused) return;

	//ズームイン、アウト処理
	{
		auto t = createTransformer();

		double delta = Mouse::Wheel();
		targetDrawingRegion = targetDrawingRegion.scaledAt(Cursor::PosF(), 1.0 + 0.1*delta);

		auto m = Window::Size().y / targetDrawingRegion.h;

		//拡大度の調節
		double mMin = Max(magnificationMin, Max(Window::Size().x / restrictedRegion.w, Window::Size().y / restrictedRegion.h));
		double mMax = magnificationMax;
		if (m > mMax) targetDrawingRegion = targetDrawingRegion.scaledAt(Cursor::PosF(), m / mMax);
		if (m < mMin) targetDrawingRegion = targetDrawingRegion.scaledAt(Cursor::PosF(), m / mMin);
	}

	//描画領域の調節
	{
		if (targetDrawingRegion.x < restrictedRegion.x) targetDrawingRegion.x = restrictedRegion.x;
		if (targetDrawingRegion.y < restrictedRegion.y) targetDrawingRegion.y = restrictedRegion.y;
		if (targetDrawingRegion.br().x > restrictedRegion.br().x) targetDrawingRegion.x -= targetDrawingRegion.br().x - restrictedRegion.br().x;
		if (targetDrawingRegion.br().y > restrictedRegion.br().y) targetDrawingRegion.y -= targetDrawingRegion.br().y - restrictedRegion.br().y;

		const double followingSpeed = 0.2;
		smoothDrawingRegion.pos = smoothDrawingRegion.pos*(1.0 - followingSpeed) + targetDrawingRegion.pos*followingSpeed;
		smoothDrawingRegion.size = smoothDrawingRegion.size*(1.0 - followingSpeed) + targetDrawingRegion.size*followingSpeed;

	}

	//視点移動処理
	{
		const double slidingSpeed = (targetDrawingRegion.size.y / 180_deg)*0.05;
		const bool useKeyViewControl = true;

		if ((useKeyViewControl && KeyA.pressed()) || Cursor::Pos().x <= 0) targetDrawingRegion.pos.x -= slidingSpeed;
		if ((useKeyViewControl && KeyW.pressed()) || Cursor::Pos().y <= 0) targetDrawingRegion.pos.y -= slidingSpeed;
		if ((useKeyViewControl && KeyD.pressed()) || Cursor::Pos().x >= Window::Size().x - 1) targetDrawingRegion.pos.x += slidingSpeed;
		if ((useKeyViewControl && KeyS.pressed()) || Cursor::Pos().y >= Window::Size().y - 1) targetDrawingRegion.pos.y += slidingSpeed;
	}
}


RectF	Camera::getDrawingRegion() const
{
	return smoothDrawingRegion;
}

Transformer2D	Camera::createTransformer() const
{
	auto mat3x2 = Mat3x2::Translate(-smoothDrawingRegion.center()).scaled(Window::Size().y / smoothDrawingRegion.size.y).translated(Window::ClientRect().center());

	return Transformer2D(mat3x2, true);
}