// Point.h
#ifndef Point_h
#define Point_h

#include "Shape.h"

namespace MinesGame
{
	// Клас, який відповідає за малювання чисельних крапок 
	class Point : public Shape
	{
	public:
		Point(float xCenter, float yCenter, float zCenter,
			float xSize, float ySize, float zSize,
			float *diffColor, float *ambiColor, float *specColor)
			: Shape(xCenter, yCenter, zCenter,
				xSize, ySize, zSize,
				specColor, diffColor, ambiColor) { }
		virtual void draw();
	};

}
#endif
