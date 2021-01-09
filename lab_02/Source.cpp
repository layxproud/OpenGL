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

// функция построения дома + земли через массив вершин

void drawHouse()
{
	GLfloat houseVertexArray[19][2];
	GLfloat houseColorArray[19][3];

	GLubyte quadsIndexArray[3][4];
	GLubyte trianglesIndexArray[1][3];
	GLubyte linesIndexArray[2][2];

	// земля
	houseVertexArray[0][0] = -1;
	houseVertexArray[0][1] = -1;

	houseVertexArray[1][0] = -1;
	houseVertexArray[1][1] = -0.5;

	houseVertexArray[2][0] = 1;
	houseVertexArray[2][1] = -0.5;

	houseVertexArray[3][0] = 1;
	houseVertexArray[3][1] = -1;

	for (int i = 0; i <= 3; i++) 
	{
		houseColorArray[i][0] = 0.1 * light();
		houseColorArray[i][1] = 0.6 * light();
		houseColorArray[i][2] = 0.0 * light();
	}

	quadsIndexArray[0][0] = 0;
	quadsIndexArray[0][1] = 1;
	quadsIndexArray[0][2] = 2;
	quadsIndexArray[0][3] = 3;

	// основание дома
	houseVertexArray[4][0] = -0.75;
	houseVertexArray[4][1] = -0.75;

	houseVertexArray[5][0] = -0.75;
	houseVertexArray[5][1] = -0.25;

	houseVertexArray[6][0] = -0.25;
	houseVertexArray[6][1] = -0.25;

	houseVertexArray[7][0] = -0.25;
	houseVertexArray[7][1] = -0.75;

	for (int i = 4; i <= 7; i++)
	{
		houseColorArray[i][0] = 0.3 * light();
		houseColorArray[i][1] = 0.2 * light();
		houseColorArray[i][2] = 0.05 * light();
	}

	quadsIndexArray[1][0] = 4;
	quadsIndexArray[1][1] = 5;
	quadsIndexArray[1][2] = 6;
	quadsIndexArray[1][3] = 7;

	// крыша
	houseVertexArray[8][0] = -0.8;
	houseVertexArray[8][1] = -0.25;

	houseVertexArray[9][0] = -0.5;
	houseVertexArray[9][1] = 0.0;

	houseVertexArray[10][0] = -0.2;
	houseVertexArray[10][1] = -0.25;

	for (int i = 8; i <= 10; i++)
	{
		houseColorArray[i][0] = 0.9 * light();
		houseColorArray[i][1] = 0.05 * light();
		houseColorArray[i][2] = 0.05 * light();
	}

	trianglesIndexArray[0][0] = 8;
	trianglesIndexArray[0][1] = 9;
	trianglesIndexArray[0][2] = 10;

	// окно
	houseVertexArray[11][0] = -0.6;
	houseVertexArray[11][1] = -0.6;

	houseVertexArray[12][0] = -0.6;
	houseVertexArray[12][1] = -0.4;

	houseVertexArray[13][0] = -0.4;
	houseVertexArray[13][1] = -0.4;

	houseVertexArray[14][0] = -0.4;
	houseVertexArray[14][1] = -0.6;

	if (isDay == true)
	{
		for (int i = 11; i <= 14; i++)
		{
			houseColorArray[i][0] = 1;
			houseColorArray[i][1] = 1;
			houseColorArray[i][2] = 1;
		}
	}
	else
	{
		for (int i = 11; i <= 14; i++)
		{
			houseColorArray[i][0] = 0.9;
			houseColorArray[i][1] = 0.8;
			houseColorArray[i][2] = 0;
		}
	}

	quadsIndexArray[2][0] = 11;
	quadsIndexArray[2][1] = 12;
	quadsIndexArray[2][2] = 13;
	quadsIndexArray[2][3] = 14;

	// оконная рама
	houseVertexArray[15][0] = -0.5;
	houseVertexArray[15][1] = -0.6;

	houseVertexArray[16][0] = -0.5;
	houseVertexArray[16][1] = -0.4;

	houseVertexArray[17][0] = -0.6;
	houseVertexArray[17][1] = -0.5;

	houseVertexArray[18][0] = -0.4;
	houseVertexArray[18][1] = -0.5;

	for (int i = 15; i <= 18; i++)
	{
		houseColorArray[i][0] = 0;
		houseColorArray[i][1] = 0;
		houseColorArray[i][2] = 0;
	}

	linesIndexArray[0][0] = 15;
	linesIndexArray[0][1] = 16;

	linesIndexArray[1][0] = 17;
	linesIndexArray[1][1] = 18;

	glVertexPointer(2, GL_FLOAT, 0, houseVertexArray);
	glColorPointer(3, GL_FLOAT, 0, houseColorArray);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawElements(GL_QUADS, 12, GL_UNSIGNED_BYTE, quadsIndexArray);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, trianglesIndexArray);
	glDrawElements(GL_LINES, 4, GL_UNSIGNED_BYTE, linesIndexArray);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
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
	drawHouse();
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
	printf("Laba 2");
	glutTimerFunc(25, timer, 0);
	glutMainLoop();

	return 0;
}

