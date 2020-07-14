//Scene.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "glut.h"
#include "Scene.h"
#include "Utils.h"
#include "Board.h"
#include "Tile.h"
#include "Point.h"

namespace MinesGame
{
	using namespace GraphUtils;

	Scene::Scene(float tileSize)
	{
		this->tileSize = tileSize;
		shapes.push_back(new Board(0.0, 0.0, 0.0, M * tileSize, 0.1, N * tileSize, diffGray, ambiGray, specGray));
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
					shapes.push_back(tiles[i][j] = new Tile(allocX(j), 0.1, allocZ(i), tileSize - 0.01, 0.1, tileSize - 0.01, diffGray, ambiGray, specGray));
			}
		}
		initialize();
	}


	Scene::~Scene()
	{
		for (int i = 0; i < shapes.size(); i++)
		{
			delete shapes[i];
		}
		for (int i = 0; i < dots.size(); i++)
		{
			delete dots[i];
		}
	}

	void Scene::on_display(void)
	{
		char text[128];

		if (finish)
		{
			if (win)
			{
				sprintf(text, " Congratulations! You`ve won! :^)          F2: Restart        ESC: Exit          Your time: %d secs", gameTime);
			}
			else
			{
				sprintf(text, " BOOM! Better luck next time :(          F2: Restart        ESC: Exit          Your time: %d secs", gameTime);
			}
		}
		else
		{
			sprintf(text, " F2: Restart        ESC: Exit          Mines left: %d          Time: %d secs", minesLeft, gameTime);
		}
		// Встановлюємо область перегляду таку, щоб вона вміщувала все вікно:
		glViewport(0, 0, width, height);

		// Ініціалізуємо параметри матеріалів і джерела світла:
		float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // колір фонового освітлення 
		float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // колір дифузного освітлення 
		float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };// колір дзеркального відображення
		float lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };// розташування джерела світла

		// Встановлюємо параметри джерела світла:
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		if (finish)
		{
			if (win)
				glClearColor(0, 0.5, 0.3, 0);
			else
				glClearColor(0.6, 0, 0.1, 0);
		}
		else
			glClearColor(0, 0.3, 0.5, 0);
	
		// Очищуємо буфери :
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Для відображення тексту, краще використовувати ортографічну проекцію:
		glOrtho(0, 1, 0, 1, -1, 1);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(0, 0, 0); // text
		drawString(GLUT_BITMAP_HELVETICA_18, text, 0.01, 0.95);
		glPopMatrix();

		// Включаємо режим роботи з матрицею проекцій:
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Задаємо усічений конус видимості в лівосторонній системі координат, 
		// 60 - кут видимості в градусах по осі у,
		// width/height - кут видимості уздовж осі x,
		// 1 и 100 - відстань від спостерігача до площин відсікання по глибині:
		gluPerspective(60, width / height, 1, 100);

		// Включаємо режим роботи з видовою матрицею:
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0, 0, distZ);	// камера з початку координат зсувається на distZ, 

		glRotatef(angleX, 0.0f, 1.0f, 0.0f);  // потім повертається по осі Oy
		glRotatef(angleY, 1.0f, 0.0f, 0.0f);  // потім повертається по осі Ox
		glEnable(GL_NORMALIZE);
		glEnable(GL_DEPTH_TEST);	// включаємо буфер глибини (для відсікання невидимих частин зображення)

		// Включаємо режим для установки освітлення:
		glEnable(GL_LIGHTING);

		// Додаємо джерело світла № 0 (їх може бути до 8), зараз воно світить з "очей":
		glEnable(GL_LIGHT0);

		for (int i = 0; i < shapes.size(); i++)
		{
			shapes[i]->draw();
		}

		for (int i = 0; i < dots.size(); i++)
		{
			dots[i]->draw();
		}

		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		glFlush();
		// показуємо вікно:
		glutSwapBuffers(); // перемикання буферів
	}

	void Scene::on_size(int width, int height)
	{
		this->width = width;
		if (height == 0)
			height = 1;
		this->height = height;
	}

	// Оброблювач подій, пов'язаних з натисканням кнопок миші
	void Scene::on_mouse(int button, int state, int x, int y)
	{
		// Зберігаємо поточні координати миші:
		mouseX = x;
		mouseY = y;
		if ((state == GLUT_UP)) // кнопка віджата
		{
			this->button = -1;  // ніяка кнопка не натиснута
			return;
		}
		this->button = button;  // зберігаємо інформацію про кнопки
		if (finish)
		{
			return;
		}
		// Вибираємо плитку, яку слід відкрити:
		if (button == 0 && findNearest(x, y, xIndex, zIndex))
		{
			if (flagged[zIndex][xIndex])
				return;
			while (!firstClick && fields[zIndex][xIndex])
			{
				GraphUtils::shuffle((int *)fields, (M * N));
			}
			if (fields[zIndex][xIndex])
			{
				tiles[zIndex][xIndex]->setColors(diffRed, ambiRed, specRed);
				finish = 1;
				win = 0;
				return;
			}
			if (!firstClick)
			{
				firstClick = true;
			}
			reveal(xIndex, zIndex);
		}
		if (button == 2 && findNearest(x, y, xIndex, zIndex))
		{
			flag(xIndex, zIndex);
		}
		
		// Перевірка закінчення гри:
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (!fields[i][j] && !revealed[i][j])
				{
					goto notyet;
				}
			}
		}
		win = 1;
		finish = 1;
		saveScore();
		return;
	notyet:
		return;
	}

	// Оброблювач подій, пов'язаних з пересуванням миші з натисненою кнопкою
	void Scene::on_motion(int x, int y)
	{
		if (button == 1) // колесо - обертання сцени
		{
			angleX += x - mouseX;
			angleY += y - mouseY;
			mouseX = x;
			mouseY = y;
		}
	}
	
	void Scene::on_special(int key, int x, int y)
	{
		switch (key) {
		case GLUT_KEY_UP:   // наближення
			if (distZ > -1.7)
			{
				break;
			}
			distZ += 0.1;
			break;
		case GLUT_KEY_DOWN: // віддалення
			distZ -= 0.1;
			break;
		case GLUT_KEY_F2:   // нова гра
			initialize();
			break;
		}
	}

	int tick = 0;
	void Scene::on_timer(int value)
	{
		tick++;
		if (tick >= 40) // нарахували наступну секунду
		{
			if (!finish && firstClick)// секунди нарощуються, якщо гру не закінчено
			{
				gameTime++;
			}
			tick = 0;   // скинули лічильник
		}
		on_display();     // здійснюємо перемалювання вікна
	}

	void Scene::initialize()
	{
		resetArr();      // початкове заповнення масиву fields
		//				 // "Тасування" масиву. Оскільки двовимірний масив у C++ зберігається як 
		//				 // одновимірний, здійснюємо його приведення до типу одновимірного масиву:
		GraphUtils::shuffle((int *)fields, (M * N));
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				revealed[i][j] = flagged[i][j] = 0;
			}
		}
		if (gameTime != 0)
			resetTiles();
		// Ініціалізація елементів даних:
		distZ = -2.2;
		angleX = 0;
		angleY = 90;
		gameTime = 0;
		finish = false;
		win = false;
		firstClick = false;
	}

	void Scene::resetArr()
	{
		minesLeft = 0;
		for (int j = 0; j < N; j++)
		{
			fields[0][j] = 1;
			minesLeft++;
		}

		for (int i = 1; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				fields[i][j] = 0;
			}
		}

		while (dots.size())
		{
			dots.pop_back();
		}
	}

	void Scene::resetTiles()
	{
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				tiles[i][j]->setCoords(allocX(j), 0.1, allocZ(i));
				tiles[i][j]->setColors(diffGray, ambiGray, specGray);
			}
		}
	}

	void Scene::saveScore()
	{
		struct tm *u;
		const time_t timer = time(NULL);
		u = localtime(&timer);
		char s[40];
		int length = strftime(s, 40, "%d.%m.%Y %H:%M:%S", u);
		std::ofstream fout("ScoreTable.txt", std::ios_base::app);
		fout << '\n' << s << ": " << gameTime << " secs";
		fout.close();
	}

	// Перерахування індексу масиву fields в координату x
	float Scene::allocX(int i)
	{
		return  tileSize * i - (N - 1) * tileSize / 2;
	}

	// Перерахування індексу масиву fields в координату z
	float Scene::allocZ(int i)
	{
		return  tileSize * i - (M - 1) * tileSize / 2;
	}

	void Scene::reveal(int x, int z)
	{
		if (x < 0 || z < 0 || x >= N || z >= M)
		{
			return;
		}
		if (revealed[z][x] || flagged[z][x] || fields[z][x])
		{
			return;
		}
		revealed[z][x] = 1;
		tiles[z][x]->setYCenter(1000.0);
		int nearM = nearMines(z, x);
		if (nearM != 0)
		{
			switch (nearM)
			{
				case 1:
					dots.push_back(new Point(allocX(x), 0.06, allocZ(z), dotSize, 0.001, dotSize, diffBlue, ambiBlue, specBlue));
					break;
				case 2:
					dots.push_back(new Point(allocX(x) + 0.05, 0.06, allocZ(z), dotSize, 0.001, dotSize, diffGreen, ambiGreen, specGreen));
					dots.push_back(new Point(allocX(x) - 0.05, 0.06, allocZ(z), dotSize, 0.001, dotSize, diffGreen, ambiGreen, specGreen));
					break;
				case 3:
					dots.push_back(new Point(allocX(x), 0.06, allocZ(z), dotSize, 0.001, dotSize, diffOrange, ambiOrange, specOrange));
					dots.push_back(new Point(allocX(x) + 0.07, 0.06, allocZ(z), dotSize, 0.001, dotSize, diffOrange, ambiOrange, specOrange));
					dots.push_back(new Point(allocX(x) - 0.07, 0.06, allocZ(z), dotSize, 0.001, dotSize, diffOrange, ambiOrange, specOrange));
					break;
				case 4:
					dots.push_back(new Point(allocX(x) - 0.05, 0.06, allocZ(z) - 0.05, dotSize, 0.001, dotSize, diffViolet, ambiViolet, specViolet));
					dots.push_back(new Point(allocX(x) + 0.05, 0.06, allocZ(z) - 0.05, dotSize, 0.001, dotSize, diffViolet, ambiViolet, specViolet));
					dots.push_back(new Point(allocX(x) + 0.05, 0.06, allocZ(z) + 0.05, dotSize, 0.001, dotSize, diffViolet, ambiViolet, specViolet));
					dots.push_back(new Point(allocX(x) - 0.05, 0.06, allocZ(z) + 0.05, dotSize, 0.001, dotSize, diffViolet, ambiViolet, specViolet));
					break;
				case 5:
					dots.push_back(new Point(allocX(x), 0.06, allocZ(z), dotSize, 0.001, dotSize, diffRed, ambiRed, specRed));
					dots.push_back(new Point(allocX(x) - 0.05, 0.06, allocZ(z) - 0.05, dotSize, 0.001, dotSize, diffRed, ambiRed, specRed));
					dots.push_back(new Point(allocX(x) + 0.05, 0.06, allocZ(z) - 0.05, dotSize, 0.001, dotSize, diffRed, ambiRed, specRed));
					dots.push_back(new Point(allocX(x) + 0.05, 0.06, allocZ(z) + 0.05, dotSize, 0.001, dotSize, diffRed, ambiRed, specRed));
					dots.push_back(new Point(allocX(x) - 0.05, 0.06, allocZ(z) + 0.05, dotSize, 0.001, dotSize, diffRed, ambiRed, specRed));
					break;
				case 6:
					dots.push_back(new Point(allocX(x) + 0.05, 0.06, allocZ(z), dotSize, 0.001, dotSize, diffLightBlue, ambiLightBlue, specLightBlue));
					dots.push_back(new Point(allocX(x) - 0.05, 0.06, allocZ(z), dotSize, 0.001, dotSize, diffLightBlue, ambiLightBlue, specLightBlue));
					dots.push_back(new Point(allocX(x) - 0.05, 0.06, allocZ(z) - 0.05, dotSize, 0.001, dotSize, diffLightBlue, ambiLightBlue, specLightBlue));
					dots.push_back(new Point(allocX(x) + 0.05, 0.06, allocZ(z) - 0.05, dotSize, 0.001, dotSize, diffLightBlue, ambiLightBlue, specLightBlue));
					dots.push_back(new Point(allocX(x) + 0.05, 0.06, allocZ(z) + 0.05, dotSize, 0.001, dotSize, diffLightBlue, ambiLightBlue, specLightBlue));
					dots.push_back(new Point(allocX(x) - 0.05, 0.06, allocZ(z) + 0.05, dotSize, 0.001, dotSize, diffLightBlue, ambiLightBlue, specLightBlue));
					break;
				case 7:
					dots.push_back(new Point(allocX(x), 0.06, allocZ(z), dotSize, 0.001, dotSize, diffYellow, ambiYellow, specYellow));
					dots.push_back(new Point(allocX(x) + 0.07, 0.06, allocZ(z), dotSize, 0.001, dotSize, diffYellow, ambiYellow, specYellow));
					dots.push_back(new Point(allocX(x) - 0.07, 0.06, allocZ(z), dotSize, 0.001, dotSize, diffYellow, ambiYellow, specYellow));
					dots.push_back(new Point(allocX(x) - 0.07, 0.06, allocZ(z) - 0.07, dotSize, 0.001, dotSize, diffYellow, ambiYellow, specYellow));
					dots.push_back(new Point(allocX(x) + 0.07, 0.06, allocZ(z) - 0.07, dotSize, 0.001, dotSize, diffYellow, ambiYellow, specYellow));
					dots.push_back(new Point(allocX(x) + 0.07, 0.06, allocZ(z) + 0.07, dotSize, 0.001, dotSize, diffYellow, ambiYellow, specYellow));
					dots.push_back(new Point(allocX(x) - 0.07, 0.06, allocZ(z) + 0.07, dotSize, 0.001, dotSize, diffYellow, ambiYellow, specYellow));
					break;
				case 8:
					dots.push_back(new Point(allocX(x) + 0.07, 0.06, allocZ(z), dotSize, 0.001, dotSize, diffBlack, ambiBlack, specBlack));
					dots.push_back(new Point(allocX(x) - 0.07, 0.06, allocZ(z), dotSize, 0.001, dotSize, diffBlack, ambiBlack, specBlack));
					dots.push_back(new Point(allocX(x) - 0.07, 0.06, allocZ(z) - 0.07, dotSize, 0.001, dotSize, diffBlack, ambiBlack, specBlack));
					dots.push_back(new Point(allocX(x) + 0.07, 0.06, allocZ(z) - 0.07, dotSize, 0.001, dotSize, diffBlack, ambiBlack, specBlack));
					dots.push_back(new Point(allocX(x) + 0.07, 0.06, allocZ(z) + 0.07, dotSize, 0.001, dotSize, diffBlack, ambiBlack, specBlack));
					dots.push_back(new Point(allocX(x) - 0.07, 0.06, allocZ(z) + 0.07, dotSize, 0.001, dotSize, diffBlack, ambiBlack, specBlack));
					dots.push_back(new Point(allocX(x), 0.06, allocZ(z) - 0.05, dotSize, 0.001, dotSize, diffBlack, ambiBlack, specBlack));
					dots.push_back(new Point(allocX(x), 0.06, allocZ(z) + 0.05, dotSize, 0.001, dotSize, diffBlack, ambiBlack, specBlack));
			}
			return;
		}
		reveal(x - 1, z - 1);
		reveal(x - 1, z);
		reveal(x - 1, z + 1);
		reveal(x, z + 1);
		reveal(x + 1, z + 1);
		reveal(x + 1, z);
		reveal(x + 1, z - 1);
		reveal(x, z - 1);
	}
	
	int Scene::nearMines(int z, int x)
	{
		int mines = 0;
		
		for (int zz = -1; zz <= 1; zz++) 
		{
			for (int xx = -1; xx <= 1; xx++) {
				if (xx+x < 0 || zz+z < 0 || zz+z >= N || xx+x >= M) continue;
				mines += fields[zz + z][xx + x];
			}
		}
		return mines;
	}

	void Scene::flag(int x, int z)
	{
		if (flagged[z][x])
		{
			flagged[z][x] = 0;
			tiles[zIndex][xIndex]->setColors(diffGray, ambiGray, specGray);
			minesLeft++;
		}
		else
		{
			flagged[z][x] = 1;
			tiles[zIndex][xIndex]->setColors(diffLightBlue, ambiLightBlue, specLightBlue);
			minesLeft--;
		}
	}

	bool Scene::findNearest(int x, int y, int &x1, int &z1)
	{
		int viewport[4];
		int iMin = -1, jMin = -1;
		double mvMatrix[16], projMatrix[16];
		double minDist = 2000;
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{

				// Світові x, y, z поточногї плитки:
				double wx = allocX(j);
				double wy = 0.1;
				double wz = allocZ(i);

				// Заповнюємо масив viewport поточною областю перегляду:
				glGetIntegerv(GL_VIEWPORT, viewport);

				// Заповнюємо масиви поточними матрицями:
				glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
				glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

				// Світові x, y, z координати, що повертаються:
				double dx, dy, dz;

				// Отримуємо координати точки, на яку спроектовано поточну плитку:
				gluProject(wx, wy, wz, mvMatrix, projMatrix, viewport, &dx, &dy, &dz);
				dy = viewport[3] - dy - 1; // dy необхідно перерахувати
				double d = (x - dx) * (x - dx) + (y - dy) * (y - dy); // квадрат відстані
				if (d < minDist) // знайшли ближчу плитку
				{
					minDist = d;
					iMin = i;
					jMin = j;
				}
			}
		}
		if (minDist < 1000) // знайшли найближчу плитку
		{
			x1 = jMin;
			z1 = iMin;
			return true;
		}
		else
		{
			return false;
		}
	}
}

