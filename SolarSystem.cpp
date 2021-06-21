#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include "loadTexture.h"

// 定义窗口的标题、宽度、高度、全屏布尔变量
#define WIN_TITLE "太阳系模型动画"
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
BOOL isFullScreen = FALSE;                     // 初始不为全屏

#define  DEG_TO_RAD 0.017453
float angle = 0.0;
static GLdouble viewer[] = { 0,0,0,0,0,-5,0,0.1,0 };              // 初始化视角

GLUquadricObj* quadric;                        // 建立二次曲面对象
GLfloat angle_Z;                               // 星空旋转角度
bool   g_bOrbitOn = true;                       // 控制转动暂停
float  g_fSpeedmodifier = 1.0f;
// 时间控制
float  g_fElpasedTime;
double g_dCurrentTime;
double g_dLastTime;

GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 0.0f };        // 环境光参数
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 0.0f };         // 漫射光参数
GLfloat LightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };         // 光源的位置
// 纹理图象
TEXTUREIMAGE  skyImg;
TEXTUREIMAGE  sunImg;
TEXTUREIMAGE  rayImg;
TEXTUREIMAGE  mercuImg;
TEXTUREIMAGE  venusImg;
TEXTUREIMAGE  earthImg;
TEXTUREIMAGE  marsImg;
TEXTUREIMAGE  jupiterImg;
TEXTUREIMAGE  saturnImg;
TEXTUREIMAGE  uranusImg;
TEXTUREIMAGE  neptuneImg;
TEXTUREIMAGE  moonImg;
GLuint  texture[12];                                // 纹理数组

// 星球速度定义
static float fSunSpin = 0.0f;                      // 太阳自转速度

static float fMercuSpin = 0.0f;                      // 水星自转速度
static float fMercuOrbit = 0.0f;                       // 水星公转速度

static float fVenusSpin = 0.0f;                      // 金星自转速度
static float fVenusOrbit = 0.0f;                       // 金星公转速度

static float fEarthSpin = 0.0f;                       // 地球自转速度
static float fEarthOrbit = 0.0f;                        // 地球公转速度

static float fMarsSpin = 0.0f;                       // 火星自转速度
static float fMarsOrbit = 0.0f;                       // 火星公转速度

static float fJupiterSpin = 0.0f;                      // 木星自转速度
static float fJupiterOrbit = 0.0f;                      // 木星公转速度

static float fSaturnSpin = 0.0f;                      // 土星自转速度
static float fSaturnOrbit = 0.0f;                      // 土星公转速度

static float fUranusSpin = 0.0f;                      // 天王星自转速度
static float fUranusOrbit = 0.0f;                      // 天王星公转速度

static float fNeptuneSpin = 0.0f;                     // 海王星自转速度
static float fNeptuneOrbit = 0.0f;                      // 海王星公转速度

static float fMoonSpin = 0.0f;                       // 月亮自转速度
static float fMoonOrbit = 0.0f;                        // 月亮公转速度

// 初始化OpenGL
void InitGL(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(2.0f);
	glEnable(GL_DEPTH_TEST);                       // 深度测试，如果输入的深度值小于或等于参考值，则通过
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);                        // 差值着色
	glEnable(GL_CULL_FACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);    // 设置环境光
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     // 设置漫反射光
	glEnable(GL_LIGHTING);                           // 打开光照
	glEnable(GL_LIGHT1);                             // 打开光源1

	// 载入纹理
	glEnable(GL_TEXTURE_2D);                       // 开启2D纹理映射
	MakeTexture(skyImg, &texture[0]);
	MakeTexture(sunImg, &texture[1]);
	MakeTexture(rayImg, &texture[2]);
	MakeTexture(mercuImg, &texture[3]);
	MakeTexture(venusImg, &texture[4]);
	MakeTexture(earthImg, &texture[5]);
	MakeTexture(marsImg, &texture[6]);
	MakeTexture(jupiterImg, &texture[7]);
	MakeTexture(saturnImg, &texture[8]);
	MakeTexture(uranusImg, &texture[9]);
	MakeTexture(neptuneImg, &texture[10]);
	MakeTexture(moonImg, &texture[11]);

	quadric = gluNewQuadric();                  // 建立一个曲面对象指针
	gluQuadricTexture(quadric, GLU_TRUE);      // 建立纹理坐标
	gluQuadricDrawStyle(quadric, GLU_FILL);     // 面填充
}
void Display(void)
{
	glLoadIdentity();
	// 设置观察点的位置和观察的方向
	gluLookAt(viewer[0], viewer[1], viewer[2], viewer[3], viewer[4], viewer[5], viewer[6], viewer[7], viewer[8]);

	// 获得系统时间使太阳系有动态效果
	g_dCurrentTime = timeGetTime();
	g_fElpasedTime = (float)((g_dCurrentTime - g_dLastTime) * 0.0005);
	g_dLastTime = g_dCurrentTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glTranslatef(0.0f, 0.0f, -5.0f);              // 将坐标系移入屏幕5.0f
	glRotatef(10, 1.0f, 0.0f, 0.0f);              // 将坐标系绕x轴旋转10度
	glEnable(GL_LIGHT0);                   // 打开光源0

	/**********************************绘制背景星空********************************************/
	glPushMatrix();                         // 当前模型矩阵入栈
	glTranslatef(-10.0f, 3.0f, 0.0f);
	glRotatef(angle_Z, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);     // 星空纹理
	// 绘制立方体,此处有问题,失败--!
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, -50.0f, -50.0f);
	glTexCoord2f(6.0f, 0.0f); glVertex3f(50.0f, -50.0f, -50.0f);
	glTexCoord2f(6.0f, 6.0f); glVertex3f(50.0f, 50.0f, -50.0f);
	glTexCoord2f(0.0f, 6.0f); glVertex3f(-50.0f, 50.0f, -50.0f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(6.0f, 6.0f); glVertex3f(-50.0f, -50.0f, 50.0f);
	glTexCoord2f(0.0f, 6.0f); glVertex3f(50.0f, -50.0f, 50.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(50.0f, 50.0f, 50.0f);
	glTexCoord2f(6.0f, 0.0f); glVertex3f(-50.0f, 50.0f, 50.0f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, -50.0f, -50.0f);
	glTexCoord2f(6.0f, 6.0f); glVertex3f(50.0f, -50.0f, 50.0f);
	glTexCoord2f(6.0f, 0.0f); glVertex3f(50.0f, -50.0f, -50.0f);
	glTexCoord2f(0.0f, 6.0f); glVertex3f(-50.0f, -50.0f, 50.0f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(6.0f, 6.0f); glVertex3f(-50.0f, 50.0f, -50.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(50.0f, 50.0f, 50.0f);
	glTexCoord2f(0.0f, 6.0f); glVertex3f(50.0f, 50.0f, -50.0f);
	glTexCoord2f(6.0f, 0.0f); glVertex3f(-50.0f, 50.0f, 50.0f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, -50.0f, -50.0f);
	glTexCoord2f(6.0f, 6.0f); glVertex3f(-50.0f, 50.0f, 50.0f);
	glTexCoord2f(0.0f, 6.0f); glVertex3f(-50.0f, -50.0f, 50.0f);
	glTexCoord2f(6.0f, 0.0f); glVertex3f(-50.0f, 50.0f, -50.0f);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(6.0f, 6.0f); glVertex3f(50.0f, -50.0f, -50.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(50.0f, 50.0f, 50.0f);
	glTexCoord2f(6.0f, 0.0f); glVertex3f(50.0f, -50.0f, 50.0f);
	glTexCoord2f(0.0f, 6.0f); glVertex3f(50.0f, 50.0f, -50.0f);
	glEnd();
	glPopMatrix();                             // 当前模型矩阵出栈

	/**********************************绘制太阳************************************************/
	glBindTexture(GL_TEXTURE_2D, texture[2]);        // 光晕纹理
	glEnable(GL_BLEND);                           // 开启混合
	glDisable(GL_DEPTH_TEST);                     // 关闭深度测试
	// 绘制太阳光晕
	glDisable(GL_LIGHTING);                        // 关闭光照
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);           // 半透明混合函数
	glColor4f(1.0f, 0.5f, 0.0f, 0.4f);                     // 设置RGBA值
	/*glBegin(GL_QUADS);
	glNormal3f( 0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();*/
	glDisable(GL_BLEND);                           // 关闭混合
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);                        // 开启光照
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); // 设置光源1位置
	glBindTexture(GL_TEXTURE_2D, texture[1]);        // 太阳纹理
	// 将坐标系绕Y轴旋转fSunSpin角度,控制太阳自转
	glRotatef(fSunSpin, 0.0, 1.0, 0.0);
	gluSphere(quadric, 0.3f, 32, 32);                    // 绘制太阳球体


	/**********************************绘制地球************************************************/
	glDisable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);                     // 开启纹理
	glPushMatrix();                            // 当前模型视图矩阵入栈
	// 将坐标系绕Y轴旋转fEarthOrbit角度,控制地球公转
	glRotatef(fEarthOrbit, 0.0f, 1.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);// 将坐标系绕X轴旋转-90度
	glTranslatef(1.1f, 0.0f, 0.0f);                      // 将坐标系右移1.1f
	glBindTexture(GL_TEXTURE_2D, texture[5]);        // 地球纹理
	// 将坐标系绕Z轴旋转fEarthSpin角度,控制地球自转
	glRotatef(fEarthSpin, 0.0f, 0.0f, 1.0f);
	gluSphere(quadric, 0.08f, 32, 32);                   // 地球球体
	glPopMatrix();
	glBegin(GL_LINE_LOOP);
	for (angle = 0; angle <= 360; angle++)
		glVertex3f(1.1f * sin(DEG_TO_RAD * angle), 0, 1.1f * cos(DEG_TO_RAD * angle));
	glEnd();


	/**********************************绘制月亮************************************************/
	glBindTexture(GL_TEXTURE_2D, texture[11]);        // 月亮纹理
	glPushMatrix();
	// 将坐标系绕Y轴旋转fEarthOrbit角度,控制月亮跟随地球
	glRotatef(fEarthOrbit, 0.0f, 1.0f, 0.0f);
	glTranslatef(1.1f, 0.0f, 0.0f);                       // 将坐标系右移1.1f
	// 将坐标系绕Y轴旋转fMoonOrbit角度,控制月亮公转
	glRotatef(fMoonOrbit, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.15f, 0.0f, 0.0f);
	// 将坐标系绕Y轴旋转fMoonSpin角度,控制月亮自转
	glRotatef(fMoonSpin, 0.0f, 1.0f, 0.0f);
	gluSphere(quadric, 0.02, 32, 32);                     // 月亮球体
	glPopMatrix();

	// 控制速度
	if (g_bOrbitOn == true)
	{
		fSunSpin -= g_fSpeedmodifier * (g_fElpasedTime * 10.0f);

		fMercuSpin -= g_fSpeedmodifier * (g_fElpasedTime * 15.0f);
		fMercuOrbit -= g_fSpeedmodifier * (g_fElpasedTime * 40.0f);

		fVenusSpin -= g_fSpeedmodifier * (g_fElpasedTime * 10.0f);
		fVenusOrbit -= g_fSpeedmodifier * (g_fElpasedTime * 30.0f);

		fEarthSpin -= g_fSpeedmodifier * (g_fElpasedTime * 100.0f);
		fEarthOrbit -= g_fSpeedmodifier * (g_fElpasedTime * 20.0f);

		fMarsSpin -= g_fSpeedmodifier * (g_fElpasedTime * 30.0f);
		fMarsOrbit -= g_fSpeedmodifier * (g_fElpasedTime * 50.0f);

		fJupiterSpin -= g_fSpeedmodifier * (g_fElpasedTime * 90.0f);
		fJupiterOrbit -= g_fSpeedmodifier * (g_fElpasedTime * 35.0f);

		fSaturnSpin -= g_fSpeedmodifier * (g_fElpasedTime * 90.0f);
		fSaturnOrbit -= g_fSpeedmodifier * (g_fElpasedTime * 25.0f);

		fUranusSpin -= g_fSpeedmodifier * (g_fElpasedTime * 70.0f);
		fUranusOrbit -= g_fSpeedmodifier * (g_fElpasedTime * 15.0f);

		fNeptuneSpin -= g_fSpeedmodifier * (g_fElpasedTime * 40.0f);
		fNeptuneOrbit -= g_fSpeedmodifier * (g_fElpasedTime * 5.0f);

		fMoonSpin -= g_fSpeedmodifier * (g_fElpasedTime * 50.0f);
		fMoonOrbit -= g_fSpeedmodifier * (g_fElpasedTime * 200.0f);
	}
	angle_Z += 0.01f;                                   // 星空旋转
	glutSwapBuffers();                                  // 交换双缓存
	glFlush();
}

void Reshape(int width, int height)
{
	if (height == 0)
		height = 1;                                      // 改变窗口
	glViewport(0, 0, width, height);                         // 设置视口，位置和大小
	// 设置投影矩阵
	glMatrixMode(GL_PROJECTION);     // 对投影（PROJECTION）进行操作，将当前矩阵指定为投影矩阵
	glLoadIdentity();                // 把矩阵设为单位矩阵
	// gluPersperctive函数，第一个参数理解为视角的大小，第二个宽高比，第三个为近裁剪面距离，第四个是远裁剪面距离
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);   
	glMatrixMode(GL_MODELVIEW);   // 调整到模型视图矩阵
	glLoadIdentity();            //调整为单位矩阵
	//gluLookAt函数，前三个参数为摄像机位置，中间三个为目标点位置，后三个为相机向上在世界坐标系的方向
	gluLookAt(viewer[0], viewer[1], viewer[2], viewer[3], viewer[4], -5, 0, 1, 0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'd': case 'D':                            // 视点上下左右移动
		if (viewer[0] <= 4.5)
			viewer[0] += 1;
		break;
	case 'a': case 'A':
		if (viewer[0] >= -10.0)
			viewer[0] -= 1;
		break;
	case 'w': case 'W':
		//if(viewer[6]<=10)
		viewer[1] += 0.1;
		break;
	case 's': case 'S':
		//if(viewer[6]>=-10)
		viewer[1] -= 0.1;
		break;
	case 'q': case 'Q':                            // 视点上下左右移动
		if (viewer[2] <= 10.5)
			viewer[2] += 0.3;
		break;
	case 'e': case 'E':
		if (viewer[2] >= -10.5)
			viewer[2] -= 0.3;
		break;

	case'+':                                 // 加速,减速,暂停
		++g_fSpeedmodifier;
		glutPostRedisplay();
		break;
	case ' ':
		g_bOrbitOn = !g_bOrbitOn;
		glutPostRedisplay();
		break;
	case'-':
		--g_fSpeedmodifier;
		glutPostRedisplay();
		break;

	case VK_ESCAPE:                         // 按ESC键时退出
		exit(0);
		break;

	default:
		break;
	}
}

void special_keys(int s_keys, int x, int y)
{
	switch (s_keys) {
	case GLUT_KEY_F1:             // 按F1键时切换窗口/全屏模式
		if (isFullScreen)
		{
			glutReshapeWindow(WIN_WIDTH, WIN_HEIGHT);
			glutPositionWindow(30, 30);
			isFullScreen = FALSE;
		}
		else
		{
			glutFullScreen();
			isFullScreen = TRUE;
		}
		break;

	case GLUT_KEY_RIGHT:                   // 视角上下左右旋转
		if (viewer[3] <= 4.5)
			viewer[3] += 0.1;
		break;
	case GLUT_KEY_LEFT:
		if (viewer[3] >= -3.0)
			viewer[3] -= 0.1;
		break;
	case GLUT_KEY_UP:
		if (viewer[4] <= 4.5)
			viewer[4] += 0.1;
		break;
	case GLUT_KEY_DOWN:
		if (viewer[4] >= -4.5)
			viewer[4] -= 0.1;
		break;

	default:
		break;
	}
}
void TimerFunc(int value)
{
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunc, 1);
}
//void mouse(int btn, int state, int x, int y)                 // 远近视角
//{
//	if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
//		viewer[2]+=0.3;
//	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN&&viewer[2]>=-3.9)
//		viewer[2]-=0.3;
//}

// 程序主函数
void main(int argc, char** argv)
{
	LoadBmp("Data//Sky.bmp", &skyImg);
	LoadBmp("Data//Sun.bmp", &sunImg);
	//LoadBmp("Data//Ray.bmp"         , &rayImg);

	LoadBmp("Data//Mercu.bmp", &mercuImg);
	LoadBmp("Data//Venus.bmp", &venusImg);
	LoadBmp("Data//Earth.bmp", &earthImg);
	LoadBmp("Data//Mars.bmp", &marsImg);
	LoadBmp("Data//Jupiter.bmp", &jupiterImg);
	LoadBmp("Data//Saturn.bmp", &saturnImg);
	LoadBmp("Data//Uranus.bmp", &uranusImg);
	LoadBmp("Data//Neptune.bmp", &neptuneImg);

	LoadBmp("Data//Moon.bmp", &moonImg);
	

	glutInit(&argc, argv);                           // 初始化GLUT库
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);          // 初始化显示模式
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);    // 初始化窗口大小
	glutInitWindowPosition(30, 30);                     // 初始化窗口位置
	glutCreateWindow(WIN_TITLE);                   // 建立窗口

	InitGL();                                       // 初始化OpenGL
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);

	glutTimerFunc(250, TimerFunc, 1); //自动动画 计时器  

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_keys);
	/*glutMouseFunc(mouse);*/
   // glutIdleFunc(Display);                   // 设置窗口空闲时的处理函数
	glutMainLoop();                        // 进入事件处理循环
}