//Scene.h
#ifndef Scene_h
#define Scene_h

#include "Shape.h"
#include "Tile.h"
#include "Point.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>


namespace MinesGame
{
	const int N = 9;
	const int M = 9;

	class Scene
	{
		std::vector<Shape*> shapes; // "�������" ����� ��������� �� �������� ���
		std::vector<Point*> dots; //
		int button;           // ������ ���� (-1 - �� ���������, 0 - ���, 1 - ������, 2 - �����)
		float angleX, angleY; // �������� ��� �������� ����� 
		float mouseX, mouseY; // ������ ����������
		float width, height;  // ������ ����
		float distZ;          // ������� �� �� Z �� �����
		bool finish;          // ������ ����, �� ��� ���������
		bool win;			  // �������� ���������� ���: 1 - ��������, 0 - ������� (�����)
		int minesLeft;
		Tile *tiles [N][M];       // ����� ��������� �� ������
		float tileSize;   // ����� ����� ������
		float dotSize = 0.015;
		int gameTime;             // �������� ��� � ��������
		int fields[N][M];     // �����, � ����� ������������ ��������� ��:
							  // 0 - ��� ����, 1 - �
		int flagged[N][M];	  //����� ����������� ������, �� ������: 1 - �������, 0 - �
		int revealed[N][M];  //����� ����������� �������� ������: 1 - �������, 0 - �
		int xIndex, zIndex;  //������� ������ ������� ������� ����
		bool firstClick;

	public:
		Scene(float tileSize);
		~Scene();
		void on_display();
		void on_size(int width, int height);
		void on_mouse(int button, int state, int x, int y);
		void on_motion(int x, int y);
		void on_special(int key, int x, int y);
		void on_timer(int value);
	private:
		void initialize();
		void resetArr();
		void resetTiles();
		void saveScore();
		float allocX(int i);
		float allocZ(int i);
		void reveal(int x, int z);
		int nearMines(int x, int z);
		void flag(int x, int z);
		bool findNearest(int x, int y, int &x1, int &z1);
	};
}
#endif

