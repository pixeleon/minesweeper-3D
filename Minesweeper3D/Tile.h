// Tile.h
#ifndef Tile_h
#define Tile_h

#include "Shape.h"

namespace MinesGame
{
	// ����, ���� ������� �� ��������� ������
	class Tile : public Shape
	{
	public:
		Tile(float xCenter, float yCenter, float zCenter,
			float xSize, float ySize, float zSize,
			float *diffColor, float *ambiColor, float *specColor)
			: Shape(xCenter, yCenter, zCenter,
				xSize, ySize, zSize,
				specColor, diffColor, ambiColor) { }
		virtual void draw();
	};

}
#endif
