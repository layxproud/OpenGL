#define _USE_MATH_DEFINES
#include <iostream>     
#include <glut.h>
#include <math.h>
using namespace std;

float shift = 0.0; // коэффициента смещения
bool isDay = true; // флаг времени суток

// таймер движения
void timer(int value)
{
	if (shift >= 2.4) // солнце/луна за экраном
	{
		isDay = !isDay; // смена флага
		shift = 0; // обнуление коэффициента смещения
	}

	shift += 0.01;
	glutPostRedisplay(); // помечает активное окно, которое должно быть перерисовано, для вызова основного цикла
	glutTimerFunc(25, timer, 0);
}

// коэффициент освещённости
float light() 
{ 
	if (isDay) {
		if (shift <= 0.8) // утро
		{
			return (shift + 0.4);
		}
		if ((shift > 0.8) && (shift <= 1.2)) // полдень
		{
			return (1.2);
		}
		return(2.4 - shift); // вечер
	}

	else
	{
		if (shift >= 1.2) // светает
		{
			return(0.4);
		}
		return((shift / 3)); // ночь
	}
}

// функция построения земли
void drawTerrain()
{
	glColor3f(0.1 * light(), 0.6 * light(), 0 * light());
	glBegin(GL_QUADS);
	glVertex2f(-1, -1);
	glVertex2f(-1, -0.5);
	glVertex2f(1, -0.5);
	glVertex2f(1, -1);
	glEnd();
}

// функция построения основания дома
void drawHouse()
{
	glColor3f(0.3 * light(), 0.2 * light(), 0.05 * light());
	glBegin(GL_QUADS);
	glVertex2f(-0.75, -0.75);
	glVertex2f(-0.75, -0.25);
	glVertex2f(-0.25, -0.25);
	glVertex2f(-0.25, -0.75);
	glEnd();
}

// функция построения крыши дома
void drawRoof()
{
	glColor3f(0.9 * light(), 0.05 * light(), 0.05 * light());
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.8, -0.25);
	glVertex2f(-0.5, 0);
	glVertex2f(-0.2, -0.25);
	glEnd();
}

// функция построения окна дома
void drawWindow()
{
	// стекло
	if (isDay == true) // днём света нет
	{
		glColor3f(1, 1, 1);
	}
	else glColor3f(0.8, 1, 0); // ночью горит свет

	glBegin(GL_QUADS);
	glVertex2f(-0.6, -0.6);
	glVertex2f(-0.6, -0.4);
	glVertex2f(-0.4, -0.4);
	glVertex2f(-0.4, -0.6);
	glEnd();

	// оконная рама
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(-0.5, -0.6);
	glVertex2f(-0.5, -0.4);
	glVertex2f(-0.6, -0.5);
	glVertex2f(-0.4, -0.5);
	glEnd();
}

// функция построения солнца
void drawSun() 
{
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * M_PI; i += M_PI / 12) // i от 0 до 2 пи; третий параметр показывает сколько в полуоукружности сегментов
		glVertex2f(cos(i) * 0.2 - 1.2, sin(i) * 0.2 + 0.5);
	glEnd();
}

// функция построения луны
void drawMoon()
{
	// окружность жёлтого цвета
	glColor3f(1.0, 1.0, 0.0); 
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * 3.14; i += 3.14 / 12)
		glVertex2f(cos(i) * 0.2 - 1.2, sin(i) * 0.2 + 0.5);
	glEnd();

	// окружность цвета неба
	glColor3f(0.1 + (light() * 0.2), 0.1 + (light() * 0.2), 0.3 + (light() * 0.7));
	glBegin(GL_POLYGON);
	for (double i = 0; i < 2 * 3.14; i += 3.14 / 12)
		glVertex2f(cos(i) * 0.2 - 1.3, sin(i) * 0.2 + 0.5);
	glEnd();
}

// функция рисования
void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // очистка буфера цвета
	glClearColor(0.1 + (light() * 0.2), 0.1 + (light() * 0.2), 0.3 + (light() * 0.7), 1.0); 
	glLoadIdentity();
	drawTerrain();
	drawHouse();
	drawRoof();
	drawWindow();
	glTranslatef(shift, 0.0, 0.0);
	if (isDay) 
	{ 
		drawSun(); 
	}
	else 
	{ 
		drawMoon(); 
	}
	glFlush(); // отрисовка экрана
}

// функция для "перестройки" изображения при изменении размеров окна
void reshape(int Width, int Height)
{
	if (Height == 0)
	{
		Height = 1;
	}

	glViewport(0, 0, (GLsizei)Width, (GLsizei)Height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)Width, 0.0, (GLdouble)Height);

	display();
}

// главная функция
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(500, 200);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Main window");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	printf("Laba 1");
	glutTimerFunc(25, timer, 0);
	glutMainLoop();

	return 0;
}

