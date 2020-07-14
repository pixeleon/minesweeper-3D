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
		std::vector<Shape*> shapes; // "гнучкий" масив указівників на елементи гри
		std::vector<Point*> dots; //
		int button;           // кнопка миші (-1 - не натиснута, 0 - ліва, 1 - колесо, 2 - права)
		float angleX, angleY; // поточний кут повороту сцени 
		float mouseX, mouseY; // поточні координати
		float width, height;  // розміри вікна
		float distZ;          // відстань по осі Z до сцени
		bool finish;          // ознака того, що гру завершено
		bool win;			  // показник результату гри: 1 - перемога, 0 - поразка (підрив)
		int minesLeft;
		Tile *tiles [N][M];       // масив указівників на плитки
		float tileSize;   // розмір кожної плитки
		float dotSize = 0.015;
		int gameTime;             // поточний час у секуднах
		int fields[N][M];     // масив, у якому відображається розміщення мін:
							  // 0 - міни немає, 1 - є
		int flagged[N][M];	  //масив відображення плиток, що відмічені: 1 - відмічена, 0 - ні
		int revealed[N][M];  //масив відображення відкритих плиток: 1 - відкрита, 0 - ні
		int xIndex, zIndex;  //індекси плитки відносно курсору миші
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

