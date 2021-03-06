// GenerationRunner.cpp : Definiert den Einstiegspunkt der Anwendung.
//

#include "stdafx.h"
#include "GenerationRunner.h"

#include <gl\gl.h>                                // Header File For The OpenGL32 Library
#include <gl\glu.h>                               // Header File For The GLu32 Library

#include "..\geneticworld\WorldMap.h"
#include "..\geneticworld\MapTile.h"
#include "..\geneticworld\Population.h"

#define MAX_LOADSTRING 100

// Globale Variablen:
HINSTANCE hInst;                                // Aktuelle Instanz
WCHAR szTitle[MAX_LOADSTRING];                  // Titelleistentext
WCHAR szWindowClass[MAX_LOADSTRING];            // Klassenname des Hauptfensters

int sizeX, sizeY;
int zoom = 0;
float dX = 0.0;
float dY =0.0;
float rX = 0.0;
float rY = 0.0;

WorldMap map;
Population pop;

void
display2()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
	/* rotate a triangle around */
	//glTranslatef(1.5f, 0.0f, -7.0f);  // Move right and into the screen
	glBegin(GL_QUADS);
/*	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2i(0, 1);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2i(-1, -1);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2i(1, -1);
*/

	glVertex3i(0, 1, 0);
	glVertex3i(1, 1, 0);
	glVertex3i(1, 1, 1);
	glVertex3i(0, 1, 1);
	glEnd();
	glFlush();
}

void display3()
{
	float fov = 90.0f;
	float f_near = 1.0f;
	float f_far = 1000.0f;

	float depth = 10.0f;

	unsigned int GridSizeX = 16;
	unsigned int GridSizeY = 16;
	unsigned int SizeX = 8;
	unsigned int SizeY = 8;



	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(GridSizeX*SizeX / 2, GridSizeY*SizeY / 2, 0.0f,
		GridSizeX*SizeX / 2, GridSizeY*SizeY / 2, 2.0f,
		0.0f, 1.0f, 0.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, sizeX / sizeY, f_near, f_far);

	glBegin(GL_QUADS);
	for (unsigned int x = 0; x<GridSizeX; ++x)
		for (unsigned int y = 0; y<GridSizeY; ++y)
		{
			if ((x + y) & 0x00000001) //modulo 2
				glColor3f(1.0f, 1.0f, 1.0f); //white
			else
				glColor3f(0.0f, 0.0f, 0.0f); //black

			glVertex3f(float(x*SizeX), float(y*SizeY), depth);
			glVertex3f(float((x + 1)*SizeX), float(y*SizeY), depth);
			glVertex3f(float((x + 1)*SizeX), float((y + 1)*SizeY), depth);
			glVertex3f(float(x*SizeX), float((y + 1)*SizeY), depth);

		}
	glEnd();
	glFlush();
}

void displayMap()
{
	glBegin(GL_QUADS);
	for (int px = 0; px < map.sizeX; px++) {
		for (int py = 0; py < map.sizeY; py++) {
			MapTile *t = map.tiles[px*map.sizeX + py];
			float *c = t->getColor();
			if ((px == (map.sizeX >> 1)) && (py == (map.sizeY >> 1))) {
				glColor3f(0.0, 1.0, 0.0);
			}
			else {
				glColor3fv(c);
			}
			glVertex3i(0 + px, 0, 0 + py);
			glVertex3i(1 + px, 0, 0 + py);
			glVertex3i(1 + px, 0, 1 + py);
			glVertex3i(0 + px, 0, 1 + py);
		}
	}
	glEnd();
}

GLUquadric *sphereQ = NULL;
GLUquadric *cylQ = NULL;
void displayItems()
{
	int n = map.findNearestItem(float(map.sizeX >> 1), float(map.sizeY >> 1));
	if (!sphereQ)sphereQ = gluNewQuadric();
	for(MapItem* var :map.items)
	{

		glPushMatrix();
		glTranslatef(var->pX+0.5f, 0.4f, var->pY+0.5f);
		if (var == map.items[n]) {
			glColor3f(1.0f, 0.0f, 1.0f);
		}
		else {
			glColor3f(0.8f, 0.1f, 0.0f); 
		}
		gluSphere(sphereQ, 0.3f, 32, 32);
		glPopMatrix();
	}
}

void displayPopulation()
{
	if (!cylQ)cylQ = gluNewQuadric();
	for (Creature* var : pop.pop)
	{

		glPushMatrix();
			glTranslatef(var->pX + 0.5f, 0.4f, var->pY + 0.5f);
			glRotatef(90.0f,-1.0f, 0.0f, 0.0f);
			glColor3f(0.0f, float(var->food)/2.0f, float(var->stamina)/20.0f);
			gluCylinder(cylQ, 0.6f, 0.0f, 1.0f,16,1);
		//gluSphere(sphereQ, 0.3, 32, 32);
		glPopMatrix();
	}
}

void displayTexts()
{
	char buf[64];
	buf[0] = '\0';
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-20.0, sizeX, 0.0, sizeY);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(100, sizeY-100);
	glListBase(1000);
	_itoa_s<sizeof(buf)>(pop.step, buf, 10);
	glCallLists(strlen(buf), GL_UNSIGNED_BYTE, buf);

	glRasterPos2i(100, sizeY - 150);
	_itoa_s<sizeof(buf)>(pop.pop.size(), buf, 10);
	glCallLists(strlen(buf), GL_UNSIGNED_BYTE,buf);

	glRasterPos2i(100, sizeY - 200);
	_itoa_s<sizeof(buf)>(pop.changeNewFood(0), buf, 10);
	glCallLists(strlen(buf), GL_UNSIGNED_BYTE, buf);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

}

float brightLight[] = { 1.0f,1.0f,1.0f };
float lightPos[] = { 0.0f,50.0f,0.0f };
void display()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-2.0f, 2.0f, -1.5, 1.5f, 1.0f, 100.0f);
	

	// Set up transforms so that the tetrahedron which is defined right at
	// the origin will be rotated and moved into the view volume.  First we
	// rotate 70 degrees around y so we can see a lot of the left side.
	// Then we rotate 50 degrees around x to "drop" the top of the pyramid
	// down a bit.  Then we move the object back 3 units "into the screen".
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-10.0f+dX, -5.0f+dY, -20.0f+zoom);
	glRotatef(50.0f+rX, 1.0f, 0.0f, 0.0f);
	glRotatef(70.0f+rY, 0.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, brightLight);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	displayMap();
	displayItems();
	displayPopulation();
	displayTexts();
	// display a string:  
	// indicate start of glyph display lists  

	
/*
	glBegin(GL_LINES);
	for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
		glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
		glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
	}
	glEnd();
*/
	glFlush();
}
LONG WINAPI
WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	static int omX, omY;
	switch (uMsg) {
	case WM_PAINT:
		display();
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_SIZE:
		glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		sizeX = LOWORD(lParam);
		sizeY = HIWORD(lParam);
		PostMessage(hWnd, WM_PAINT, 0, 0);
		return 0;

	case WM_CHAR:
		switch (wParam) {
		case 27:			/* ESC key */
			PostQuitMessage(0);
			break;
		case 'R':
			map.reitemize(100);
			PostMessage(hWnd, WM_PAINT, 0, 0);
			break;
		case 'T':
			SetTimer(hWnd, NULL, 100, NULL);
			break;
		case 'S':
			pop.simulateStep();
			PostMessage(hWnd, WM_PAINT, 0, 0);
			return 0;
		case VK_UP:
			dY++;
			break;
		case VK_DOWN:
			dY--;
			break;
		case VK_LEFT:
			dX--;
			break;
		case VK_RIGHT:
			dX++;
			break;
		case 'N' :
			pop.changeNewFood(-20);
			break;
		case 'M' :
			pop.changeNewFood(20);
			break;

		}
		PostMessage(hWnd, WM_PAINT, 0, 0);
		return 0;
	case WM_TIMER:
		pop.simulateStep();
		SetTimer(hWnd, NULL, 50, NULL);
		PostMessage(hWnd, WM_PAINT, 0, 0);
		return 0;
	case WM_MOUSEWHEEL: 
	{
		int delta = int(short(HIWORD(wParam)));
		zoom += int(delta / 120);
		PostMessage(hWnd, WM_PAINT, 0, 0);
		return 0;
	}
	case WM_MOUSEMOVE: 
	{
		if (wParam&MK_LBUTTON) {
			dX += float(LOWORD(lParam) - omX)*0.05f;
			dY -= float(HIWORD(lParam) - omY)*0.05f;
			PostMessage(hWnd, WM_PAINT, 0, 0);
		}else if (wParam&MK_RBUTTON) {
			rY += float(LOWORD(lParam) - omX)*0.05f;
			rX -= float(HIWORD(lParam) - omY)*0.05f;
			PostMessage(hWnd, WM_PAINT, 0, 0);
		}
		omX = LOWORD(lParam);
		omY = HIWORD(lParam);
		return 0;
	}

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND
CreateOpenGLWindow(char* title, int x, int y, int width, int height,
	BYTE type, DWORD flags)
{
	int         pf;
	HDC         hDC;
	HWND        hWnd;
	WNDCLASS    wc;
	PIXELFORMATDESCRIPTOR pfd;
	static HINSTANCE hInstance = 0;

	/* only register the window class once - use hInstance as a flag. */
	if (!hInstance) {
		hInstance = GetModuleHandle(NULL);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"OpenGL";

		if (!RegisterClass(&wc)) {
			MessageBox(NULL, L"RegisterClass() failed:  "
				L"Cannot register window class.", L"Error", MB_OK);
			return NULL;
		}
	}

	hWnd = CreateWindow( L"OpenGL" , L"Generationrunner S:Singlestep,T:Timer,R:Reset less food " , WS_OVERLAPPEDWINDOW |
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		x, y, width, height, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL) {
		MessageBox(NULL, L"CreateWindow() failed:  Cannot create a window.",
			L"Error", MB_OK);
		return NULL;
	}

	hDC = GetDC(hWnd);

	/* there is no guarantee that the contents of the stack that become
	the pfd are zeroed, therefore _make sure_ to clear these bits. */
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags;
	pfd.iPixelType = type;
	pfd.cColorBits = 32;

	pf = ChoosePixelFormat(hDC, &pfd);
	if (pf == 0) {
		MessageBox(NULL, L"ChoosePixelFormat() failed:  "
			L"Cannot find a suitable pixel format.", L"Error", MB_OK);
		return 0;
	}

	if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
		MessageBox(NULL, L"SetPixelFormat() failed:  "
			L"Cannot set format specified.", L"Error", MB_OK);
		return 0;
	}

	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	


	ReleaseDC(hWnd,hDC);

	return hWnd;
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	HDC hDC;				/* device context */
	HGLRC hRC;				/* opengl context */
	HWND  hWnd;				/* window */
	MSG   msg;				/* message */
	sizeX = 1024;
	sizeY = 768;

	map.setSize(100, 100);
	pop.spawn(50, &map);

	hWnd = CreateOpenGLWindow( (char*)"title", 0, 0, sizeX, sizeY, PFD_TYPE_RGBA, 0);
	if (hWnd == NULL)
		exit(1);

	hDC = GetDC(hWnd);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
	wglUseFontBitmaps(hDC, 0, 255, 1000);
	ShowWindow(hWnd, nShowCmd);

	while (GetMessage(&msg, hWnd, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	wglMakeCurrent(NULL, NULL);
	ReleaseDC(hWnd, hDC);
	wglDeleteContext(hRC);
	DestroyWindow(hWnd);

	return msg.wParam;
}