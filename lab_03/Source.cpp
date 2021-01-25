#include <glut.h>
#include <iostream>
#include <locale.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

// ------- ѕј–јћ≈“–џ ќ Ќј ----------------------------
#define WIN_WIDTH 750 // ширина окна
#define WIN_HEIGHT 750 // высота окна

// -------------- ѕј–јћ≈“–џ ќ “јЁƒ–ј ------------------------
GLfloat octX = 100.0; // базисна¤ координата по ќх
GLfloat octY = 150.0; // базисна¤ координата по ќу
GLfloat octZ = 100.0; // базисна¤ координата по ќz

static bool autoRotateROctY = false; // автоматическое вращение по часовой
static bool autoRotateLOctY = false; // автоматическое вращение против часовой
GLint rotateOctCount = 0; // сколько раз нажата клавиша вращени¤

GLfloat rotateOctX = 20.0; // коэффициент поворота по Ox
GLfloat rotateOctY = 20.0; // коэффициент поворота по Oу

GLfloat sideShift = 10.0; // коэффицент раздвижени¤ граней

// массивы вершин граней октаэдра
GLfloat point1[3] = { 0.0, 0.0, 0.0 };
GLfloat point2[3] = { 0.0, 0.0, 0.0 };
GLfloat point3[3] = { 0.0, 0.0, 0.0 };

GLint colorMode = 0; // окрас граней (0 - цвета, 1 - одна текстура, 2 - 8 текстур)

static GLuint tex[9]; // массив текстур

static bool isBlend = false; // прозрачность

// -------------------- ѕј–јћ≈“–џ »—“ќ„Ќ» ј —¬≈“ј ---------------------

static bool autoRotateLightR = false;
static bool autoRotateLightL = false;
GLfloat rotateLight = 1.0; // угол вращени¤
GLint rotateSphereCount = 0; // сколько раз нажата клавиша вращени¤
static bool isLight = true; // освещение

// ----------------- DISPLAY LIST PARAM ---------------------

static bool isListOct = false; // display list
static GLuint list; // переменна¤ дл¤ списка
GLint stripes = 100; // количество полосок

// ----------------- ѕ–ќ„»≈ ѕј–јћ≈“–џ ----------------------

GLint speed = 1; // скорость вращени¤

// ------------------- Ќќ–ћ»–ќ¬јЌ»≈ ------------------------
// ‘ункци¤ сделана опира¤сь на статью "Calculating a Surface Normal"
// с сайта www.khronos.org/opengl/wiki
void getNormal(float p1[3], float p2[3], float p3[3], float vNormal[3])
{
    float v1[3], v2[3]; // дл¤ промежуточных вычислений
    float normal; // норма

    v1[0] = p2[0] - p1[0];
    v1[1] = p2[1] - p1[1];
    v1[2] = p2[2] - p1[2];

    v2[0] = p3[0] - p1[0];
    v2[1] = p3[1] - p1[1];
    v2[2] = p3[2] - p1[2];

    vNormal[0] = v1[1] * v2[2] - v1[2] * v2[1];
    vNormal[1] = v1[2] * v2[0] - v1[0] * v2[2];
    vNormal[2] = v1[0] * v2[1] - v1[1] * v2[0];

    normal = sqrt(vNormal[0] * vNormal[0] + vNormal[1] * vNormal[1] + vNormal[2] * vNormal[2]);
    // нормирование
    vNormal[0] /= normal;
    vNormal[1] /= normal;
    vNormal[2] /= normal;
}

// --------------------- ќ“–»—ќ¬ ј —‘≈–џ --------------------
void drawSphere()
{
    glColor3f(1.0, 1.0, 1.0); // сфера излучает белый свет
    glTranslatef(0.0, 0.0, 200.0); // положение сферы на сцене
    glutWireSphere(10.0, 10.0, 10.0); // стандартна¤ функци¤ задани¤ Wire Sphere
}

// ---------------- ќ“–»—ќ¬ ј √–јЌ≈… DISPLAY LIST -----------
void sideList()
{
    point1[0] = octX + sideShift;      
    point1[1] = point1[2] = sideShift;

    point2[0] = point2[2] = sideShift; 
    point2[1] = octY + sideShift;

    point3[0] = point3[1] = sideShift; 
    point3[2] = octZ + sideShift;

    float norm[3] = { 0.0 , 0.0 , 0.0 }; // массив дл¤ нормали
    getNormal(point1, point2, point3, norm); // получение нормали
    glNormal3fv(norm); //нормаль к поверхности останетс¤ неизменной

    // построение граней
    for (int i = 0; i < stripes; i++)
    {
        glBegin(GL_POLYGON);
            glVertex3f(point1[0] - i, point1[1] + i * 1.5, point1[2]);
            glVertex3f(point3[0], point3[1] + i * 1.5, point3[2] - i);
            glVertex3f(point1[0] - (i - 1), point1[1] + (i + 1) * 1.5, point1[2]);
            glVertex3f(point3[0], point3[1] + (i + 1) * 1.5, point3[2] - (i - 1));
            i += 0.1;
        glEnd();
    }
}

// --------------- ќ“–»—ќ¬ ј ќ “јЁƒ–ј DISPLAY LIST ----------
void drawOctList()
{
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    sideList(); // рисует грань

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    sideList();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    sideList();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(270, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 0.0);
    sideList();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 1.0);
    sideList();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    glColor3f(0.0, 1.0, 1.0);
    sideList();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(270, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.5, 0.0);
    sideList();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(360, 0.0, 1.0, 0.0);
    glColor3f(0.25, 0.25, 0.25);
    sideList();
    glPopMatrix();
    glPopMatrix();
}

// --------------------- ќ“–»—ќ¬ ј √–јЌ≈… -------------------
void side(int countSide = 0)
{
    glBegin(GL_TRIANGLES);

    point1[0] = octX + sideShift;
    point1[1] = point1[2] = sideShift;

    point2[0] = point2[2] = sideShift;
    point2[1] = octY + sideShift;

    point3[0] = point3[1] = sideShift;
    point3[2] = octZ + sideShift;

    float norm[3] = { 0.0 , 0.0 , 0.0 }; // массив дл¤ нормали
    getNormal(point1, point2, point3, norm);  // получение нормали
    glNormal3fv(norm); // нормаль к поверхности останетс¤ неизменной

    if (colorMode % 3) // если включен режим текстур
    {
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(point1);

        glTexCoord2f(1.0, 0.0);
        glVertex3fv(point2);

        glTexCoord2f(0.0, 1.0);
        glVertex3fv(point3);
    }
    else if (!(colorMode % 3) && countSide) // если нужно нарисовать трехцветную грань (цвета задаютс¤ здесь)
    {
        glColor3f(0.0, 0.0, 1.0);
        glVertex3fv(point1);

        glColor3f(0.0, 1.0, 0.0);
        glVertex3fv(point2);

        glColor3f(1.0, 0.0, 0.0);
        glVertex3fv(point3);
    }
    else // рисует обычную грань (цвета задаютс¤ в функции drawOct())
    {
        glVertex3fv(point1);
        glVertex3fv(point2);
        glVertex3fv(point3);
    }

    glEnd();
}

// --------------------- ќ“–»—ќ¬ ј ќ “јЁƒ–ј -----------------
void drawOct()
{
    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[0]); // прив¤зка текстуры к цели текстурировани¤
    if ((colorMode % 3) == 1) glBindTexture(GL_TEXTURE_2D, tex[8]); 
    glPushMatrix();
    if (!(colorMode % 3)) glColor3f(1.0, 0.0, 0.0);
    side(); // рисует грань
    
    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[1]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    if (!(colorMode % 3)) glColor3f(0.0, 1.0, 0.0);
    side();
    glPopMatrix();

    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[2]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    if (!(colorMode % 3)) glColor3f(0.0, 0.0, 1.0);
    side();
    glPopMatrix();

    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[3]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(270, 0.0, 1.0, 0.0);
    if (!(colorMode % 3)) glColor3f(1.0, 1.0, 0.0);
    side();
    glPopMatrix();

    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[4]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    if (!(colorMode % 3)) glColor3f(1.0, 0.0, 1.0);
    side();
    glPopMatrix();

    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[5]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(180, 0.0, 1.0, 0.0);
    if (!(colorMode % 3)) glColor3f(0.0, 1.0, 1.0);
    side();
    glPopMatrix();

    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[6]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(270, 0.0, 1.0, 0.0);
    if (!(colorMode % 3)) glColor3f(1.0, 0.5, 0.0);
    side();
    glPopMatrix();

    if ((colorMode % 3) == 2) glBindTexture(GL_TEXTURE_2D, tex[7]);
    glPushMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glRotatef(360, 0.0, 1.0, 0.0);
    side(7); // сюда можно передать любой параметр int, функци¤ side() поймЄт, что это трЄхцветна¤ грань
    glPopMatrix();
    glPopMatrix();
}

// -------------------- ќ“–»—ќ¬ ј —÷≈Ќџ ---------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очищает буферы цвета и глубины
    glLoadIdentity(); // замен¤ет текущую матрицу на единичную

    if (isLight) glEnable(GL_LIGHT0); // включает источник света GL_LIGHT0
    else glDisable(GL_LIGHT0); // выключает источник света GL_LIGHT0

    if (isBlend) // если включен режим прозрачности 
    {
        glEnable(GL_BLEND); // включает режим смешивани¤
        glDepthMask(GL_FALSE); // запрещает запись в буфер глубины
        glBlendFunc(GL_SRC_ALPHA, GL_ONE); // ‘ункци¤ смешивани¤ дл¤ непрозрачности, базирующа¤с¤ на значении альфы
    }
    else // если режим прозрачности выключен
    {
        glDepthMask(GL_TRUE); // разрешает запись в буфер глубины
        glDisable(GL_BLEND); // выключает режим смешивани¤
    }

    if (colorMode % 3) // если включен режим текстур
    {
        glEnable(GL_TEXTURE_2D); // разрешает наложение текстуры
        glPushMatrix();
        glRotatef(rotateOctX, 1.0, 0.0, 0.0);
        glRotatef(rotateOctY, 0.0, 1.0, 0.0);
        drawOct(); // рисует октаэдр
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
    else // если включен режим окрашивани¤
    {
        glPushMatrix();
        glRotatef(rotateOctX, 1.0, 0.0, 0.0); // поворот по Oх
        glRotatef(rotateOctY, 0.0, 1.0, 0.0); // поворот по Oу
        if (!isListOct)
            drawOct(); // рисует октаэдр
        else
            glCallList(list); // вызывает указанный список отображени¤
        glPopMatrix();
    }

    glRotatef(rotateLight, 0.0, 1.0, 0.0); // вращение сферы относительно 'Oy' под углом 1
    drawSphere(); // рисование сферы
    float lightpos[] = { 0.0, 0.0, 200.0, 0.0 }; // координаты GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos); // задание положени¤ источника света

    glutSwapBuffers(); // обменивает буфера, отобража¤ окно целиком после того, как оно уже сформировано
    glFlush(); // отправл¤ет все команды на выполненение, не дожида¤сь завершени¤ выполнени¤ команд. 
}

// ------------- ‘”Ќ ÷»я ѕ≈–≈—“–ќ… » »«ќЅ–ј∆≈Ќ»я -----------------
void reshape(int w, int h)
{
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT); // задание области вывода (0,0) - левый нижний угол, (w, h) - ширина и высота
    glMatrixMode(GL_PROJECTION); // задание перспективной проекции;
                                 // матрица проекций задает как будут проецироватьс¤ трехмерные объекты
                                 // на плоскость экрана (в оконные координаты)
    glLoadIdentity(); // замен¤ет текущую матрицу на единичную
    glOrtho(-WIN_WIDTH / 2, WIN_WIDTH / 2, -WIN_HEIGHT / 2, WIN_HEIGHT / 2, -500, 500); // задание ортографической (параллельной) проекции
    glMatrixMode(GL_MODELVIEW); // далее будет проводитьс¤ только преобразование объектов сцены
    glLoadIdentity();
}

// -------------- “ј…ћ≈– --------------------
void timer(int value)
{
    if (autoRotateROctY) // Ox
    {
        rotateOctY += speed;
        if (rotateOctY > 360) rotateOctY = speed;
    }
    else if (autoRotateLOctY) // Oy
    {
        rotateOctY -= speed;
        if (rotateOctY < -360) rotateOctY = -speed;
    }

    if (autoRotateLightR) // Ox
    {
        rotateLight += speed;
        if (rotateLight > 360) rotateLight = speed;
    }
    else if (autoRotateLightL) // Oy
    {
        rotateLight -= speed;
        if (rotateLight < -360) rotateLight = -speed;
    }

    glutPostRedisplay(); //перерисовка экрана
    glutTimerFunc(10, timer, 1); //10 - врем¤ в миллисекундах, 1 - идентификатор таймера
}

// -------------- Ќј«Ќј„≈Ќ»≈  Ћј¬»Ў --------------
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'r': // вращение октаэдра
            if (rotateOctCount == 0)
            {
                autoRotateROctY = true;
                rotateOctCount++;
            }
            else if (rotateOctCount == 1)
            {
                autoRotateROctY = false;
                autoRotateLOctY = true;
                rotateOctCount++;
            }
            else {
                autoRotateLOctY = false;
                rotateOctCount = 0;
            }
            break;

        case 'd': // поворот направо
            rotateOctY += 5;
            break;

        case 'a': // поворот налево
            rotateOctY -= 5;
            break;

        case 'w': // наклон вверх
            rotateOctX += 5;
            break;

        case 's': // наклон вниз
            rotateOctX -= 5;
            break;

        case 'z': // движение сферой влево
            rotateLight = (rotateLight >= 0) ? rotateLight + 5 : 360;
            break;

        case 'x': // движение сферой вправо
            rotateLight = (rotateLight <= 0) ? 360 : rotateLight - 5;
            break;

        case 'e': // вращение сферы
            if (rotateSphereCount == 0)
            {
                autoRotateLightR = true;
                rotateSphereCount++;
            }
            else if (rotateSphereCount == 1)
            {
                autoRotateLightR = false;
                autoRotateLightL = true;
                rotateSphereCount++;
            }
            else {
                autoRotateLightL = false;
                rotateSphereCount = 0;
            }
            break;
        
        case 'l' : // включение света
            isLight = isLight ? false : true;
            break;

        case 'b':
            isBlend = isBlend ? false : true;
            break;

        case 't':
            colorMode += 1;
            break;

        case 'q':
            isListOct = isListOct ? false : true;
            break;
        
        default:
            break;
    }

        glutPostRedisplay();
}

// ----------------- «јƒјЌ»≈ “≈ —“”– -----------------
void loadTexture()
{
    unsigned char* image[9]; // массив изображений
    int w[9], h[9], bpp[9]; // ширина, высота, количество 8-битных компонентов на пиксель

    image[0] = stbi_load("img/img1.png", &w[0], &h[0], &bpp[0], STBI_rgb_alpha);
    image[1] = stbi_load("img/img2.png", &w[1], &h[1], &bpp[1], STBI_rgb_alpha);
    image[2] = stbi_load("img/img3.png", &w[2], &h[2], &bpp[2], STBI_rgb_alpha);
    image[3] = stbi_load("img/img4.png", &w[3], &h[3], &bpp[3], STBI_rgb_alpha);
    image[4] = stbi_load("img/img5.png", &w[4], &h[4], &bpp[4], STBI_rgb_alpha);
    image[5] = stbi_load("img/img6.png", &w[5], &h[5], &bpp[5], STBI_rgb_alpha);
    image[6] = stbi_load("img/img7.png", &w[6], &h[6], &bpp[6], STBI_rgb_alpha);
    image[7] = stbi_load("img/img8.png", &w[7], &h[7], &bpp[7], STBI_rgb_alpha);
    image[8] = stbi_load("img/img9.png", &w[8], &h[8], &bpp[8], STBI_rgb_alpha);

    //“екстурирование
    for (int i = 0; i < 9; i++)
    {
        glGenTextures(1, &tex[i]); // создание идентификаторов текстур
        glBindTexture(GL_TEXTURE_2D, tex[i]); // создание и актуализаци¤ необходимой текстуры
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // указание, что выравнивание в массиве данных идЄт по байту
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w[i], h[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, image[i]); // указание, что полученное изображение - текстура
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // фильтраци¤
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // фильтраци¤
        stbi_image_free(image[i]); // то же самое что free()
    }
}

void init()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очистка буферов цвета и глубины
    glEnable(GL_DEPTH_TEST); // включение теста глубины. OpenGL будет автоматически сохран¤ть значени¤ глубины дл¤ всех прошедших тест фрагментов и отбрасывать не прошедшие
    glEnable(GL_LIGHTING); // включение глобального освещени¤
    glEnable(GL_LIGHT0); // включение источника света
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // двухстороннее освещение

    GLfloat light_col[] = { 1.0, 1.0, 1.0, 1.0 }; // цвет GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_col); // задание модели освещени¤ рассе¤нный свет
    glEnable(GL_COLOR_MATERIAL); // включает управление свойством материала с помощью текущего цвета

    list = glGenLists(1); // генерирует непрерывный набор пустых списков отображени¤
    glNewList(list, GL_COMPILE); // формирование списка; список будет только сформирован (GL_COMPILE)
    drawOctList(); // рисование октаэдра с помощью списка отображени¤
    glEndList(); // завершает работу со списком

    loadTexture(); // обрабатывает текстуры
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "RUSSIAN"); // установил русский ¤зык

    glutInit(&argc, argv); // инициализаци¤ функций библиотеки GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // выбор режима отображени¤
    // GLUT_DOUBLE - ¬ывод в окно осуществл¤етс¤ с использованием 2 буферов. ѕримен¤етс¤ дл¤ анимации, чтобы исключить эффект мерцани¤.
    // GLUT_RGB - ƒл¤ отображени¤ графической информации используютс¤ 3 компоненты цвета RGB.
    // GLUT_DEPTH - Ѕуфер глубины. Ётот буфер используетс¤ дл¤ отсечени¤ невидимых линий в 3D пространстве при выводе на плоский экран монитора.

    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT); // «адание параметров окна
    glutCreateWindow("Oct"); // —оздание окна

    // регистраци¤ вызываемых функций
    glutDisplayFunc(display);
    glutTimerFunc(10, timer, 1); // врем¤ в миллисекундах, функци¤, идентиикатор таймера
    init();
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    // инструкци¤ по работе с приложением
    cout << " лавиши:" << endl
        << "l: выкл./вкл. свет" << endl
        << "r: вращение октаэдра" << endl
        << "e: вращение источника света" << endl
        << "w: наклон октаэдра вверх" << endl
        << "a: поворот октаэдра влево" << endl
        << "s: наклон октаэдра вниз" << endl
        << "d: поворот октаэдра вправо" << endl
        << "z: поворот источника света влево" << endl
        << "x: поворот источника света вправо" << endl
        << "b: выкл./вкл. прозрачность" << endl
        << "t: параметр окраски октаэдра" << endl
        << "q: отображение через display list" << endl;

    glutMainLoop(); // контроль всех событий

    return 0;
}