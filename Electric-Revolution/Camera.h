#pragma once

//Ž‹“_ˆÚ“®
class Camera
{
	RectF	targetDrawingRegion;
	RectF	smoothDrawingRegion;
	RectF	restrictedRegion;
	double	magnificationMin;
	double	magnificationMax;

public:
	Camera(RectF __restrictedRegion, RectF _drawingRegion, double _mMax, double _mMin);
	Camera(RectF __restrictedRegion, double _mMax, double _mMin);

	void	update();
	RectF	getDrawingRegion() const;
	Transformer2D	createTransformer() const;
};
