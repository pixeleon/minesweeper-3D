#include "glut.h"
#include "Point.h"
#include "Utils.h"

namespace MinesGame
{

	void Point::draw()
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, getAmbiColor());
		glMaterialfv(GL_FRONT, GL_DIFFUSE, getDiffColor());
		glMaterialfv(GL_FRONT, GL_SPECULAR, getSpecColor());
		glMaterialf(GL_FRONT, GL_SHININESS, GraphUtils::shininess);
		// Запис поточної матриці в стек
		// (збереження вмісту поточної матриці для подальшого використання):
		glPushMatrix();
		//glTranslatef(getXCenter(), getYCenter(), getZCenter());
		//glRotatef(180, 1, 0, 0);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(getXCenter() - getXSize(), getYCenter(), getZCenter() - getZSize());
		glVertex3f(getXCenter() + getXSize(), getYCenter(), getZCenter() - getZSize());
		glVertex3f(getXCenter() + getXSize(), getYCenter(), getZCenter() + getZSize());
		glVertex3f(getXCenter() - getXSize(), getYCenter(), getZCenter() + getZSize());
		/*glVertex3f(0.6, 0.5, -0.8);
		glVertex3f(0.8, 0.5, -0.8);
		glVertex3f(0.8, 0.5, -0.6);
		glVertex3f(0.6, 0.5, -0.6);*/ 
		glEnd();
		// Відновлення поточної матриці зі стека:
		glPopMatrix();
	}

}