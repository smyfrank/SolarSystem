#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include "loadTexture.h"

// ���崰�ڵı��⡢��ȡ��߶ȡ�ȫ����������
#define WIN_TITLE "̫��ϵģ�Ͷ���"
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
BOOL isFullScreen = FALSE;                     // ��ʼ��Ϊȫ��

#define  DEG_TO_RAD 0.017453
float angle = 0.0;
static GLdouble viewer[] = { 0,0,0,0,0,-5,0,0.1,0 };              // ��ʼ���ӽ�

GLUquadricObj* quadric;                        // ���������������
GLfloat angle_Z;                               // �ǿ���ת�Ƕ�
bool   g_bOrbitOn = true;                       // ����ת����ͣ
float  g_fSpeedmodifier = 1.0f;
// ʱ�����
float  g_fElpasedTime;
double g_dCurrentTime;
double g_dLastTime;

GLfloat LightAmbient[] = { 1.0f, 1.0f, 1.0f, 0.0f };        // ���������
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 0.0f };         // ��������
GLfloat LightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };         // ��Դ��λ��
// ����ͼ��
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
GLuint  texture[12];                                // ��������

// �����ٶȶ���
static float fSunSpin = 0.0f;                      // ̫����ת�ٶ�

static float fMercuSpin = 0.0f;                      // ˮ����ת�ٶ�
static float fMercuOrbit = 0.0f;                       // ˮ�ǹ�ת�ٶ�

static float fVenusSpin = 0.0f;                      // ������ת�ٶ�
static float fVenusOrbit = 0.0f;                       // ���ǹ�ת�ٶ�

static float fEarthSpin = 0.0f;                       // ������ת�ٶ�
static float fEarthOrbit = 0.0f;                        // ����ת�ٶ�

static float fMarsSpin = 0.0f;                       // ������ת�ٶ�
static float fMarsOrbit = 0.0f;                       // ���ǹ�ת�ٶ�

static float fJupiterSpin = 0.0f;                      // ľ����ת�ٶ�
static float fJupiterOrbit = 0.0f;                      // ľ�ǹ�ת�ٶ�

static float fSaturnSpin = 0.0f;                      // ������ת�ٶ�
static float fSaturnOrbit = 0.0f;                      // ���ǹ�ת�ٶ�

static float fUranusSpin = 0.0f;                      // ��������ת�ٶ�
static float fUranusOrbit = 0.0f;                      // �����ǹ�ת�ٶ�

static float fNeptuneSpin = 0.0f;                     // ��������ת�ٶ�
static float fNeptuneOrbit = 0.0f;                      // �����ǹ�ת�ٶ�

static float fMoonSpin = 0.0f;                       // ������ת�ٶ�
static float fMoonOrbit = 0.0f;                        // ������ת�ٶ�

// ��ʼ��OpenGL
void InitGL(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(2.0f);
	glEnable(GL_DEPTH_TEST);                       // ��Ȳ��ԣ������������ֵС�ڻ���ڲο�ֵ����ͨ��
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);                        // ��ֵ��ɫ
	glEnable(GL_CULL_FACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);    // ���û�����
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     // �����������
	glEnable(GL_LIGHTING);                           // �򿪹���
	glEnable(GL_LIGHT1);                             // �򿪹�Դ1

	// ��������
	glEnable(GL_TEXTURE_2D);                       // ����2D����ӳ��
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

	quadric = gluNewQuadric();                  // ����һ���������ָ��
	gluQuadricTexture(quadric, GLU_TRUE);      // ������������
	gluQuadricDrawStyle(quadric, GLU_FILL);     // �����
}
void Display(void)
{
	glLoadIdentity();
	// ���ù۲���λ�ú͹۲�ķ���
	gluLookAt(viewer[0], viewer[1], viewer[2], viewer[3], viewer[4], viewer[5], viewer[6], viewer[7], viewer[8]);

	// ���ϵͳʱ��ʹ̫��ϵ�ж�̬Ч��
	g_dCurrentTime = timeGetTime();
	g_fElpasedTime = (float)((g_dCurrentTime - g_dLastTime) * 0.0005);
	g_dLastTime = g_dCurrentTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glTranslatef(0.0f, 0.0f, -5.0f);              // ������ϵ������Ļ5.0f
	glRotatef(10, 1.0f, 0.0f, 0.0f);              // ������ϵ��x����ת10��
	glEnable(GL_LIGHT0);                   // �򿪹�Դ0

	/**********************************���Ʊ����ǿ�********************************************/
	glPushMatrix();                         // ��ǰģ�;�����ջ
	glTranslatef(-10.0f, 3.0f, 0.0f);
	glRotatef(angle_Z, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);     // �ǿ�����
	// ����������,�˴�������,ʧ��--!
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
	glPopMatrix();                             // ��ǰģ�;����ջ

	/**********************************����̫��************************************************/
	glBindTexture(GL_TEXTURE_2D, texture[2]);        // ��������
	glEnable(GL_BLEND);                           // �������
	glDisable(GL_DEPTH_TEST);                     // �ر���Ȳ���
	// ����̫������
	glDisable(GL_LIGHTING);                        // �رչ���
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);           // ��͸����Ϻ���
	glColor4f(1.0f, 0.5f, 0.0f, 0.4f);                     // ����RGBAֵ
	/*glBegin(GL_QUADS);
	glNormal3f( 0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();*/
	glDisable(GL_BLEND);                           // �رջ��
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);                        // ��������
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); // ���ù�Դ1λ��
	glBindTexture(GL_TEXTURE_2D, texture[1]);        // ̫������
	// ������ϵ��Y����תfSunSpin�Ƕ�,����̫����ת
	glRotatef(fSunSpin, 0.0, 1.0, 0.0);
	gluSphere(quadric, 0.3f, 32, 32);                    // ����̫������


	/**********************************���Ƶ���************************************************/
	glDisable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);                     // ��������
	glPushMatrix();                            // ��ǰģ����ͼ������ջ
	// ������ϵ��Y����תfEarthOrbit�Ƕ�,���Ƶ���ת
	glRotatef(fEarthOrbit, 0.0f, 1.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);// ������ϵ��X����ת-90��
	glTranslatef(1.1f, 0.0f, 0.0f);                      // ������ϵ����1.1f
	glBindTexture(GL_TEXTURE_2D, texture[5]);        // ��������
	// ������ϵ��Z����תfEarthSpin�Ƕ�,���Ƶ�����ת
	glRotatef(fEarthSpin, 0.0f, 0.0f, 1.0f);
	gluSphere(quadric, 0.08f, 32, 32);                   // ��������
	glPopMatrix();
	glBegin(GL_LINE_LOOP);
	for (angle = 0; angle <= 360; angle++)
		glVertex3f(1.1f * sin(DEG_TO_RAD * angle), 0, 1.1f * cos(DEG_TO_RAD * angle));
	glEnd();


	/**********************************��������************************************************/
	glBindTexture(GL_TEXTURE_2D, texture[11]);        // ��������
	glPushMatrix();
	// ������ϵ��Y����תfEarthOrbit�Ƕ�,���������������
	glRotatef(fEarthOrbit, 0.0f, 1.0f, 0.0f);
	glTranslatef(1.1f, 0.0f, 0.0f);                       // ������ϵ����1.1f
	// ������ϵ��Y����תfMoonOrbit�Ƕ�,����������ת
	glRotatef(fMoonOrbit, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.15f, 0.0f, 0.0f);
	// ������ϵ��Y����תfMoonSpin�Ƕ�,����������ת
	glRotatef(fMoonSpin, 0.0f, 1.0f, 0.0f);
	gluSphere(quadric, 0.02, 32, 32);                     // ��������
	glPopMatrix();

	// �����ٶ�
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
	angle_Z += 0.01f;                                   // �ǿ���ת
	glutSwapBuffers();                                  // ����˫����
	glFlush();
}

void Reshape(int width, int height)
{
	if (height == 0)
		height = 1;                                      // �ı䴰��
	glViewport(0, 0, width, height);                         // �����ӿڣ�λ�úʹ�С
	// ����ͶӰ����
	glMatrixMode(GL_PROJECTION);     // ��ͶӰ��PROJECTION�����в���������ǰ����ָ��ΪͶӰ����
	glLoadIdentity();                // �Ѿ�����Ϊ��λ����
	// gluPersperctive��������һ���������Ϊ�ӽǵĴ�С���ڶ�����߱ȣ�������Ϊ���ü�����룬���ĸ���Զ�ü������
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);   
	glMatrixMode(GL_MODELVIEW);   // ������ģ����ͼ����
	glLoadIdentity();            //����Ϊ��λ����
	//gluLookAt������ǰ��������Ϊ�����λ�ã��м�����ΪĿ���λ�ã�������Ϊ�����������������ϵ�ķ���
	gluLookAt(viewer[0], viewer[1], viewer[2], viewer[3], viewer[4], -5, 0, 1, 0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'd': case 'D':                            // �ӵ����������ƶ�
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
	case 'q': case 'Q':                            // �ӵ����������ƶ�
		if (viewer[2] <= 10.5)
			viewer[2] += 0.3;
		break;
	case 'e': case 'E':
		if (viewer[2] >= -10.5)
			viewer[2] -= 0.3;
		break;

	case'+':                                 // ����,����,��ͣ
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

	case VK_ESCAPE:                         // ��ESC��ʱ�˳�
		exit(0);
		break;

	default:
		break;
	}
}

void special_keys(int s_keys, int x, int y)
{
	switch (s_keys) {
	case GLUT_KEY_F1:             // ��F1��ʱ�л�����/ȫ��ģʽ
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

	case GLUT_KEY_RIGHT:                   // �ӽ�����������ת
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
//void mouse(int btn, int state, int x, int y)                 // Զ���ӽ�
//{
//	if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
//		viewer[2]+=0.3;
//	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN&&viewer[2]>=-3.9)
//		viewer[2]-=0.3;
//}

// ����������
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
	

	glutInit(&argc, argv);                           // ��ʼ��GLUT��
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);          // ��ʼ����ʾģʽ
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);    // ��ʼ�����ڴ�С
	glutInitWindowPosition(30, 30);                     // ��ʼ������λ��
	glutCreateWindow(WIN_TITLE);                   // ��������

	InitGL();                                       // ��ʼ��OpenGL
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);

	glutTimerFunc(250, TimerFunc, 1); //�Զ����� ��ʱ��  

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_keys);
	/*glutMouseFunc(mouse);*/
   // glutIdleFunc(Display);                   // ���ô��ڿ���ʱ�Ĵ�����
	glutMainLoop();                        // �����¼�����ѭ��
}