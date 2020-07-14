#include "glut.h"
#include "Scene.h"

using MinesGame::Scene;

Scene *scene;

void on_display()
{
	scene->on_display();
}

void on_size(int width, int height)
{
	scene->on_size(width, height);
}


void on_mouse(int button, int state, int x, int y)
{
	scene->on_mouse(button, state, x, y);
}

void on_motion(int x, int y)
{
	scene->on_motion(x, y);
}

void on_keyboard(unsigned char key, int x, int y)
{
	// Обробка подій від клавіатури:
	if (key == 27)
		exit(0);
}

void on_special(int key, int x, int y)
{
	scene->on_special(key, x, y);
}

void on_timer(int value)
{
	scene->on_timer(value);
	glutTimerFunc(25, on_timer, 0); // зa 25 мс викличеться ця функція
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);       
	scene = new Scene(0.2);         
	glutInitWindowSize(1024, 768);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH|GLUT_DOUBLE);    
	glutCreateWindow("Minesweeper 3D");   
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_size);
	glutMotionFunc(on_motion);     // реєструємо функцію, яка відповідає за переміщення миші з натиснутою кнопкою
	glutMouseFunc(on_mouse);       // реєструємо функцію, яка відповідає за натискання на кнопку миші
	glutKeyboardFunc(on_keyboard);
	glutSpecialFunc(on_special);
	glutTimerFunc(25, on_timer, 0);
	glutMainLoop();              
	delete scene;                
	return(0);
}