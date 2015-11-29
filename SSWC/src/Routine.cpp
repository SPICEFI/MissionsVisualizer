#include <Windows.h>

#undef GetObject

#include <stdio.h>

#include "GL\glut.h"
#include <gl\GL.h>
#include <gl\GLU.h>
#include <time.h>
#include "Program.h"
#include "GL\ARB_Multisample.h"


#include "App.h"
//#include "Date.h"

#include "Planet.h"
#include "Scene.h"

#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking
#pragma comment( lib, "winmm.lib" )								// Search For WinMM Library While Linking

#ifndef		CDS_FULLSCREEN										// CDS_FULLSCREEN Is Not Defined By Some
#define		CDS_FULLSCREEN 4									// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

int centerX;
int centerY;

glCamera g_Camera;
GL_Window*	g_window;
Keys*		g_keys;

App app;
Scene SolarSystem;
Date t;
TGA* skyTexture;

int id = -1;
float g_LightPosition[4] = { 0, 0, 0, 1 };

bool orientationMode = true;
std::string planetName;

BOOL Initialize(GL_Window* window, Keys* keys)					// Any OpenGL Initialization Goes Here
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);

	app.SetLoggingFile("error.log");
	app.SetReferenceFrame(Frame::ECLIPJ2000);
	app.SetDefaultUnits(App::UT_DEFAULT);

	app.LoadKernel("data/meta.tm");
	app.LoadChildren(SpaceObject("Solar system barycenter"), true, true);

	TGA* sunTexture = new TGA("Images\\SUN.tga");
	skyTexture = new TGA("Images\\STARS.tga");

	size_t count = app.GetObjectsLength();
	for (int i = 0; i < count; i++)
	{
		const SpaceObject& obj = app.GetObjectByIndex(i);
		try
		{
			if (SpaceObject::IsPlanet(obj.GetSpiceId()) /*|| obj.GetSpiceId() == SUN_SPICE_ID*/)
			{
				std::string path = "Images\\" + obj.GetName() + ".tga";

				if(TGA* Texture = new TGA(path.c_str()))
				{
					const SpaceBody& body = dynamic_cast<const SpaceBody&>(obj);
					SolarSystem.addPlanet(Planet(body, Texture));
				}
				else
				{
					const SpaceBody& body = dynamic_cast<const SpaceBody&>(obj);
					SolarSystem.addPlanet(Planet(body, sunTexture));
				}
			}
		}
		catch (const std::bad_cast&)
		{
		}
	}


 	g_window = window;
	g_keys = keys;
	g_Camera.PositionCamera(Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1));

	RECT rect;
	GetClientRect(g_window->hWnd, &rect);

	centerX = (rect.right - rect.left) >> 1;
	centerY = (rect.bottom - rect.top) >> 1;

	SetCursorPos(centerX, centerY);

	Date t("Aug 17 2000 15:51:01 UTC-5");

	//Color setup
	glClearColor(0, 0, 0, 1);

	float g_LightPosition[4] = { 1, 1, 0, 1 };
	float ambience[4] = { 0.0f, 0.0f, 0.0f };
	float diffuse[4] = { 1.0f, 0.9f, 0.9f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambience);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	
	glLightfv(GL_LIGHT0, GL_POSITION, g_LightPosition);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	
	return TRUE;												// Return TRUE (Initialization Successful)
}


void Update(DWORD milliseconds)									// Perform Motion Updates Here
{
	if (g_keys->keyDown[VK_ESCAPE])								// Is ESC Being Pressed?
	{
		TerminateApplication(g_window);						// Terminate The Program
	}

	if (g_keys->keyDown[VK_F1])									// Is F1 Being Pressed?
	{
		ToggleFullscreen(g_window);							// Toggle Fullscreen Mode
	}

	/*if (g_keys->keyDown[VK_F2])
	{
		ChangeMode(g_window);
	}*/

	if (g_keys->keyDown['W'])
	{
		if (orientationMode)
			g_Camera.MoveCamera(0.03f);
	}

	if (g_keys->keyDown['S'])
	{
		if (orientationMode)
			g_Camera.MoveCamera(-0.03f);
	}

	if (g_keys->keyDown['A'])
	{
		if (orientationMode)
			g_Camera.Strafe(-0.03f);
	}

	if (g_keys->keyDown['D'])
	{
		if (orientationMode)
			g_Camera.Strafe(0.03f);
	}

	if (LEFT_MOUSE_BUTTON_DOWN)
	{
		if (!orientationMode)
		id = g_Camera.RetrieveObjectID(mouse_x, mouse_y, centerX * 2, centerY * 2, SolarSystem, t, app);
		LEFT_MOUSE_BUTTON_DOWN = false;
		if (id != 0)
		{
			Planet obj = SolarSystem.FindObjectWithID(id);
			MessageBox(NULL, obj.body.GetSpiceName().c_str(), "Information", MB_OK);
		}
	}

	t += Time(1.0, Units::Common::minutes);
}

void CreateSkyBox(float x, float y, float z, float width, float height, float length)
{
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;

	glBindTexture(GL_TEXTURE_2D, skyTexture->getTextureHandle());

	
	glBegin(GL_QUADS);

	//BACK
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);

	//FACE
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);

	//BOTTOM
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);

	//UP
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);

	//LEFT
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);

	//RIGHT
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);

	glEnd();
}

void Draw(void)													// Draw Our Scene
{
	glEnable(GL_MULTISAMPLE_ARB);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer										
	glEnable(GL_DEPTH_TEST);
	// Reset The Modelview Matrix
	glLoadIdentity();

	//glViewport(0, 0, 640, 720);
	g_Camera.Update(centerX, centerY, orientationMode);
	CreateSkyBox(0, 0, 0, 400, 400, 400);
	SolarSystem.render(t, app);

	//glViewport(640, 0, 640, 720);
	//glLightfv(GL_LIGHT0, GL_POSITION, g_LightPosition);
	//g_Camera.Update(centerX, centerY);
	//CreateSkyBox(0, 0, 0, 400, 400, 400);
	//SolarSystem.render(t, app);

	glLightfv(GL_LIGHT0, GL_POSITION, g_LightPosition);

	glFlush();													// Flush The GL Rendering Pipeline
}