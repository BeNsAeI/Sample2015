#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctype.h>
#include <vector>
#include <math.h>
#include <cmath>
#include <cstring>
#include <string>
#include <fstream>
#include <time.h>


#ifdef WIN32
#include <windows.h>
#include <al.h>
#include <alc.h>
//#include <alut.h>
#include <omp.h>
#include "glew.h"

#pragma warning(disable:4996)
#endif

#ifndef WIN32
#include <GL/glew.h>
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alc.h>
//#include <AL/alut.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <time.h>
#include <GLFW/glfw3native.h>
#include "glut.h"
#include "glslprogram.h"

#include "const.h"
#include "struct.h"
#include "loader.h"
#include "Map.h"
#include "AI.h"
#include "SimpleAI.h"
#include "neuron.h"
#include "Globals.h"
#include "Renderer.h"
#include "Material.h"
#include "Utility.h"



#ifdef WIN32
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	int w_argc = 0;
	LPWSTR* w_argv = CommandLineToArgvW(GetCommandLineW(), &w_argc);
	if (w_argv)
	{
		char** my_argv = new char*[w_argc];
		int my_argc = 0;

		for (int i = 0; i < w_argc; ++i)
		{
			int w_len = lstrlenW(w_argv[i]);
			int len = WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, NULL, 0, NULL, NULL);
			my_argv[my_argc] = new char[len + 1];
			WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, my_argv[my_argc], len + 1, NULL, NULL);
			++my_argc;
		}

		main(my_argc, my_argv);

		for (int i = 0; i < my_argc; ++i)
			delete[] my_argv[i];
		delete[] my_argv;

		LocalFree(w_argv);
	}
}
#endif
int main(int argc, char *argv[])
{
	srand(time(NULL));
	// Do main menu:
	loading = true;
	isInMenu = true;
	backgroundRand = rand() % 10;
	backgroundRand = (backgroundRand + rand()) % 10;
	mapName = "M";

	glutInit(&argc, argv);

	//initiate draw function
	
	// setup all the graphics stuff:

	InitGraphics();


	// create the display structures that will not change:

	InitLists();


	// init all the global variables used by Display( ):
	// this will also post a redisplay


	Reset();


	// setup all the user interface stuff:

	//InitMenus();


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow(MainWindow);
	glutMainLoop();


	// this is here to make the compiler happy:
	for(int i = 0; i < NUMMODEL; i++)
		glDeleteBuffers(1, &ModelIDList[i]);
	alDeleteSources(1, &mainMusic1);
	alDeleteSources(1, &mainMusic2);
	alDeleteSources(1, &mainMusic3);
	alDeleteSources(1, &mainMusic4);
	alDeleteSources(1, &mainMusic5);
	alDeleteSources(1, &mainMusic6);
	alDeleteSources(1, &mainMusic7);
	alDeleteSources(1, &tankShellFire);
	alDeleteSources(1, &tankShellBounce);
	alDeleteSources(1, &tankExplode);
	alDeleteSources(1, &tankBulletFire);
	alDeleteSources(1, &A_10);
	alDeleteBuffers(1, Buffers);

	if (myAIKB.isAI)
	{
		delete(myAIKB.agent);
		myAIKB.agent = NULL;
	}
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	return 0;
}
void aTenEventHandler()
{
	if (aTenActive)
	{
		int currentTime = time(NULL);

		if (!drawATenActive)
		{
			drawATenActive = true;
			drawATenActiveTime = clock();
		}
		if (aTenTimer)
		{
			if (currentTime - aTenStart > aTenWait)
			{
				if (aTenTarget == 1)
				{
					for (int i = 0; i < 6; i++)
					{
						drawSparks(AbramXY[0], AbramXY[1]);
						aTenTargetCoord = AbramXY[0];
						struct Smoke tmpSmoke;
						tmpSmoke.smokeIDBuffer = Time;
						tmpSmoke.smokeCoordBuffer[0] = AbramXY[0] - 5 + rand() % 10;
						tmpSmoke.smokeCoordBuffer[1] = AbramXY[1] - 5 + rand() % 10;
						tmpSmoke.smokeDurBuffer = 0.03;
						tmpSmoke.smokeAngleBuffer = rand() % 360;
						tmpSmoke.smokeIDBufferSet = true;
						tmpSmoke.smokeActive = true;
						Smokes.push_back(tmpSmoke);
						AbramSmokeBudget = 0;
					}
					AbramHP -= 3.9;
				}
				if (aTenTarget == 2)
				{
					for (int i = 0; i < 6; i++)
					{
						drawSparks(IS3XY[0], IS3XY[1]);
						aTenTargetCoord = IS3XY[0];
						struct Smoke tmpSmoke;
						tmpSmoke.smokeIDBuffer = Time;
						tmpSmoke.smokeCoordBuffer[0] = IS3XY[0] - 5 + rand() % 10;
						tmpSmoke.smokeCoordBuffer[1] = IS3XY[1] - 5 + rand() % 10;
						tmpSmoke.smokeDurBuffer = 0.03;
						tmpSmoke.smokeAngleBuffer = rand() % 360;
						tmpSmoke.smokeIDBufferSet = true;
						tmpSmoke.smokeActive = true;
						Smokes.push_back(tmpSmoke);
						IS3SmokeBudget = 0;
					}
					IS3HP -= 3.9;
				}
				aTenStart = 0;
				aTenActive = false;
				aTenTimer = false;
				aTenTarget = 0;
			}
		}
		else
		{
			aTenStart = currentTime;
			aTenTimer = true;
		}
	}
}
void Display()
{
	if (!clicked && Time > 0.05)
	{
		#ifdef WIN32
		INPUT Inputs[3] = { 0 };

		Inputs[0].type = INPUT_MOUSE;
		Inputs[0].mi.dx = 0; // desired X coordinate
		Inputs[0].mi.dy = 0; // desired Y coordinate
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

		Inputs[1].type = INPUT_MOUSE;
		Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		Inputs[2].type = INPUT_MOUSE;
		Inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

		SendInput(3, Inputs, sizeof(INPUT));
		#endif
		clicked = true;
		loading = false;
	}
	else {
		if (!clicked)
			loading = true;
	}
	{
		glutSetCursor(GLUT_CURSOR_NONE);
		// set which window we want to do the graphics into:

		glutSetWindow(MainWindow);


		// erase the background:

		glDrawBuffer(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		// specify shading to be flat:

		//glShadeModel(GL_FLAT);


		// set the viewport to a square centered in the window:

	}
	GLsizei vx = glutGet(GLUT_WINDOW_WIDTH);
	GLsizei vy = glutGet(GLUT_WINDOW_HEIGHT);
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	v = vx > vy ? vx : vy;
	GLint xl = (vx - v) / 2;
	GLint yb = (vy - v) / 2;
	glViewport(xl, yb, v, v);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90., 1., 0.1, 1000.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// set the eye position, look-at position, and up-vector:
	if (isInMenu)
	{
		if(menuState == 3)
			gluLookAt(eyex, eyey, eyez, targetx, targety, targetz, upx, upy, upz);
		else 
			if(menuState == 6)
				gluLookAt(20, 15, 0, 0, 0, 0, 0, 1, 0);
			else
				gluLookAt(10, 10, 0, 0, 3, -3, 0, 1, 0);

		glRotatef((GLfloat)Yrot, 0., 1., 0.);
		glRotatef((GLfloat)Xrot, 1., 0., 0.);
	}
	else
		gluLookAt(eyex, eyey, eyez, targetx, targety, targetz, upx, upy, upz);

	// Costume polys for each frame (instapoly):__________________________________________________________________________________________________________________________
	
	if (res && isInMenu)
		drawMenu();
	if (res && !isInMenu)//__________________________________________________________________________________________________________________________

	{
		PatternGrass->Use();
		//draw grass
		glCallList(BoxList);
		PatternGrass->Use(0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		if (myAIKB.isAI)
		{
			myAIKB.agent->getMove(myAIKB.AIID, keyBuffer,Crates);
		}

		gamepad();
		KeyHandler();
		aTenEventHandler();
		if (drawATenActive)
		{
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(-2.5f, -2.5f);
			glLineWidth(OUTLINE);
			PatternSilh->Use();
			drawA_Ten();
			PatternSilh->Use(0);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(-2.5f, -2.5f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glShadeModel(GL_FLAT);
			drawA_Ten();
			glPopAttrib();
		}

		if (shake)
		{
			if ((Time - shakeStartTime) < shakeDuration)
			{
				if (!playExplosionSound)
				{
					alSourcePlay(Sources[10]);
					playExplosionSound = true;
				}
					
				eyex += sin((Time - shakeStartTime) * 5000) / 2;
				eyey += sin((Time - shakeStartTime) * 5000) / 2;
				if (AbramHP <= 0)
				{
					// Push the GL attribute bits so that we don't wreck any settings
					glPushAttrib(GL_ALL_ATTRIB_BITS);
					// Enable polygon offsets, and offset filled polygons forward by 2.5
					// Set the render mode to be line rendering with a thick line width
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDisable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(-2.5f, -2.5f);
					glLineWidth(OUTLINE);
					// Set the colour to be white
					glColor3f(.5, .5, .5);
					// Render the object
					PatternSilh->Use();
					drawExplosion(AbramXY[0], AbramXY[1], 0, 0, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 60, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 120, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 180, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 240, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 300, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					PatternSilh->Use(0);
					glEnable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(-2.5f, -2.5f);
					// Set the polygon mode to be filled triangles 
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_FLAT);
					//glEnable(GL_LIGHTING);
					////SetPointLight(GL_LIGHT1, 20, 50, 35, 0.75, 0.75, 0.75);
					Pattern->Use();
					glColor3f(0.0f, 0.0f, 0.0f);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 0, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 60, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 120, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 180, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 240, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(AbramXY[0], AbramXY[1], 0, 300, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					glPopAttrib();
					Pattern->Use(0);
					//glDisable(GL_LIGHT1);
					//glDisable(GL_LIGHTING);

				}
				if (IS3HP <= 0)
				{
					// Push the GL attribute bits so that we don't wreck any settings
					glPushAttrib(GL_ALL_ATTRIB_BITS);
					// Enable polygon offsets, and offset filled polygons forward by 2.5
					// Set the render mode to be line rendering with a thick line width
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDisable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(-2.5f, -2.5f);
					glLineWidth(OUTLINE);
					// Set the colour to be white
					glColor3f(.5, .5, .5);
					// Render the object
					PatternSilh->Use();
					drawExplosion(IS3XY[0], IS3XY[1], 0, 0, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 60, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 120, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 180, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 240, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 300, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					PatternSilh->Use(0);
					glEnable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(-2.5f, -2.5f);
					// Set the polygon mode to be filled triangles 
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_FLAT);
					//glEnable(GL_LIGHTING);
					////SetPointLight(GL_LIGHT1, 20, 50, 35, 0.75, 0.75, 0.75);
					Pattern->Use();
					glColor3f(0.0f, 0.0f, 0.0f);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 0, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 60, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 120, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 180, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 240, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					drawExplosion(IS3XY[0], IS3XY[1], 0, 300, 0.5, 1 - fabs(sin((Time - shakeStartTime) * 500)), 0.75 - 3 * fabs(sin((Time - shakeStartTime) * 500)) / 4, 0, shakeStartTime, shakeDuration / 2);
					glPopAttrib();
					Pattern->Use(0);
					//glDisable(GL_LIGHT1);
					//glDisable(GL_LIGHTING);
				}
			}
			else
			{
				eyex = CAMX;
				eyey = CAMY;
				shake = false;
				playExplosionSound = false;
			}
		}
		// Draw shell UI
		// Push the GL attribute bits so that we don't wreck any settings
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		// Enable polygon offsets, and offset filled polygons forward by 2.5
		// Set the render mode to be line rendering with a thick line width
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		glLineWidth(OUTLINE);
		// Set the colour to be white
		glColor3f(.5, .5, .5);
		// Render the object
		PatternSilh->Use();
		glBegin(GL_LINE_STRIP);
		switch (PlayerOne)
		{
		case 0:
			glColor3f(0.5, 0.5, 0);
			break;
		case 1:
			glColor3f(0, 0, 1);
			break;
		case 2:
			glColor3f(0.38, 0.2, 0.01);
			break;
		case 3:
			glColor3f(1, 0.1, 0.5);
			break;
		case 4:
			glColor3f(0, 1, 0);
			break;
		}
		glVertex3f(MAPEDGEX + 20, 3, -MAPEDGEY);
		glVertex3f(MAPEDGEX + 20, 3, -MAPEDGEY + AbramSmoke * 7);
		glEnd();
		glBegin(GL_LINE_STRIP);
		switch (PlayerTwo)
		{
		case 0:
			glColor3f(0.5, 0.5, 0);
			break;
		case 1:
			glColor3f(0, 0, 1);
			break;
		case 2:
			glColor3f(0.38, 0.2, 0.01);
			break;
		case 3:
			glColor3f(1, 0.1, 0.5);
			break;
		case 4:
			glColor3f(0, 1, 0);
			break;
		}
		glVertex3f(MAPEDGEX + 20, 3, MAPEDGEY);
		glVertex3f(MAPEDGEX + 20, 3, MAPEDGEY - IS3Smoke * 7);
		glEnd();
		PatternSilh->Use(0);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		glLineWidth(OUTLINE);
		if (AbramSmoke > 0)
			drawSmokeCrate(MAPEDGEX + 22, -MAPEDGEY + AbramSmoke * 7 + 1, 90);
		if (IS3Smoke > 0)
			drawSmokeCrate(MAPEDGEX + 22, MAPEDGEY - IS3Smoke * 7 - 1, 270);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_FLAT);
		//glEnable(GL_LIGHTING);
		////SetPointLight(GL_LIGHT1, 20, 50, 35, 0.75, 0.75, 0.75);
		Pattern->Use();
		glColor3f(0.0f, 0.0f, 0.0f);
		if (AbramSmoke > 0)
			drawSmokeCrate(MAPEDGEX + 22, -MAPEDGEY + AbramSmoke * 7 + 1, 90);
		if (IS3Smoke > 0)
			drawSmokeCrate(MAPEDGEX + 22, MAPEDGEY - IS3Smoke * 7 - 1, 270);
		glPopAttrib();
		Pattern->Use(0);
		//glDisable(GL_LIGHT1);
		//glDisable(GL_LIGHTING);
		// Push the GL attribute bits so that we don't wreck any settings
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		// Enable polygon offsets, and offset filled polygons forward by 2.5
		// Set the render mode to be line rendering with a thick line width
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		glLineWidth(10.0f);
		// Set the colour to be white
		glColor3f(.5, .5, .5);
		// Render the object
		PatternSilh->Use();
		glBegin(GL_LINE_STRIP);
		switch (PlayerOne)
		{
		case 0:
			glColor3f(0.5, 0.5, 0);
			break;
		case 1:
			glColor3f(0, 0, 1);
			break;
		case 2:
			glColor3f(0.38, 0.2, 0.01);
			break;
		case 3:
			glColor3f(1, 0.1, 0.5);
			break;
		case 4:
			glColor3f(0, 1, 0);
			break;
		}
		glVertex3f(MAPEDGEX + 15, 3, -MAPEDGEY);
		glVertex3f(MAPEDGEX + 15, 3, -MAPEDGEY + AbramShells * 2);
		glEnd();
		glBegin(GL_LINE_STRIP);
		switch (PlayerTwo)
		{
		case 0:
			glColor3f(0.5, 0.5, 0);
			break;
		case 1:
			glColor3f(0, 0, 1);
			break;
		case 2:
			glColor3f(0.38, 0.2, 0.01);
			break;
		case 3:
			glColor3f(1, 0.1, 0.5);
			break;
		}
		glVertex3f(MAPEDGEX + 15, 3, MAPEDGEY);
		glVertex3f(MAPEDGEX + 15, 3, MAPEDGEY - IS3Shells * 2);
		glEnd();
		PatternSilh->Use(0);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		glLineWidth(OUTLINE);
		if (AbramShells > 0)
			drawShell(MAPEDGEX + 15, -MAPEDGEY + AbramShells * 2, 180, 4);
		if (IS3Shells > 0)
			drawShell(MAPEDGEX + 15, MAPEDGEY - IS3Shells * 2, 0, 4);
		// Set the polygon mode to be filled triangles 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_FLAT);
		glEnable(GL_LIGHTING);
		////SetPointLight(GL_LIGHT1, 20, 50, 35, 0.75, 0.75, 0.75);
		Pattern->Use();
		glColor3f(0.0f, 0.0f, 0.0f);
		if (AbramShells > 0)
			drawShell(MAPEDGEX + 15, -MAPEDGEY + AbramShells * 2, 180, 4);
		if (IS3Shells > 0)
			drawShell(MAPEDGEX + 15, MAPEDGEY - IS3Shells * 2, 0, 4);
		glPopAttrib();
		Pattern->Use(0);
		//glDisable(GL_LIGHT1);
		//glDisable(GL_LIGHTING);


		// Draw Tanks
		// Push the GL attribute bits so that we don't wreck any settings
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		// Enable polygon offsets, and offset filled polygons forward by 2.5
		// Set the render mode to be line rendering with a thick line width
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		glLineWidth(OUTLINE);
		// Set the colour to be white
		glColor3f(.5, .5, .5);
		// Render the object
		PatternSilh->Use();
		if (AbramHP > 0)
		{
			DrawPointer1(AbramXY[0], -0.25, AbramXY[1], AbramHullAngle, AbramTurretAngle);
		}
			
		else
		{
			DrawPointer1d(AbramXY[0], -0.25, AbramXY[1], AbramHullAngle, AbramTurretAngle);
			if (!shakeOnce)
			{
				shake = true;
				shakeOnce = true;
				shakeStartTime = Time;
			}
		}
		if (IS3HP > 0)
		{
			DrawPointer2(IS3XY[0], -0.25, IS3XY[1], IS3HullAngle, IS3TurretAngle);
		}
		else
		{
			DrawPointer2d(IS3XY[0], -0.25, IS3XY[1], IS3HullAngle, IS3TurretAngle);
			if (!shakeOnce)
			{
				shake = true;
				shakeOnce = true;
				shakeStartTime = Time;
			}
		}
		PatternSilh->Use(0);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		// Set the polygon mode to be filled triangles 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//glShadeModel(GL_FLAT);
		//glEnable(GL_LIGHTING);

		//SetPointLight(GL_LIGHT0, 20, 50, 35, 0.25, 0.25, 0.25);

		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		PatternCamo->Use();
		// Set the colour to the background
		glColor3f(0.0f, 0.0f, 0.0f);
		// Render the object
		if (AbramHP > 0)
		{
			PatternCamo->SetUniformVariable((char *)"uTol", (float)0);
			DrawPointer1(AbramXY[0], -0.25, AbramXY[1], AbramHullAngle, AbramTurretAngle);
		}
		else
		{
			PatternCamo->SetUniformVariable((char *)"uTol", (float)0.25);
			DrawPointer1d(AbramXY[0], -0.25, AbramXY[1], AbramHullAngle, AbramTurretAngle);
		}
			
		if (IS3HP > 0)
		{
			PatternCamo->SetUniformVariable((char *)"uTol", (float)0);
			DrawPointer2(IS3XY[0], -0.25, IS3XY[1], IS3HullAngle, IS3TurretAngle);
		}
		else
		{
			PatternCamo->SetUniformVariable((char *)"uTol", (float)0.25);
			DrawPointer2d(IS3XY[0], -0.25, IS3XY[1], IS3HullAngle, IS3TurretAngle);
		}
		// Pop the state changes off the attribute stack
		// to set things back how they were
		glPopAttrib();
		PatternCamo->Use(0);
		//glDisable(GL_LIGHT0);
		//glDisable(GL_LIGHTING);
		//glDisable(GL_TEXTURE_2D);

		Pattern->Use();
		for (int i = 0; i < (2 * MAPEDGEX) / CUBESIZE + 2; i++)
			drawCube(-MAPEDGEX - CUBESIZE / 2 - 2 + i*CUBESIZE, -MAPEDGEY - CUBESIZE, 0, 0.5, 0.5, 0.5);
		for (int i = 0; i < (2 * MAPEDGEX) / CUBESIZE + 2; i++)
			drawCube(-MAPEDGEX - CUBESIZE / 2 - 2 + i*CUBESIZE, MAPEDGEY + CUBESIZE, 0, 0.5, 0.5, 0.5);
		for (int i = 0; i < (2 * MAPEDGEY) / CUBESIZE + 2; i++)
			drawCube(-MAPEDGEX - CUBESIZE, -MAPEDGEY - CUBESIZE + i*CUBESIZE, 0, 0.5, 0.5, 0.5);
		for (int i = 0; i < (2 * MAPEDGEY) / CUBESIZE + 2; i++)
			drawCube(MAPEDGEX + CUBESIZE, -MAPEDGEY - CUBESIZE + i*CUBESIZE, 0, 0.5, 0.5, 0.5);
		Pattern->Use(0);
		//test ammo power up
		// Draw Map
		for (int j = 0; j < 14; j++)
		{
			for (int i = 0; i < 24; i++)
			{
				if (myMap.MCM[i][j] && myMap.isSolid[i][j] && (myMap.color[i][j][0] != 7))
				{
					if (destructionTimeBuffer[i][j] > 0 && (Time - destructionTimeBuffer[i][j]) > 0 && (Time - destructionTimeBuffer[i][j]) < 0.0025)
						myMap.coord[i][j][2] -= (Time - destructionTimeBuffer[i][j]) * 500;
					if (destructionTimeBuffer[i][j] > 0 && (Time - destructionTimeBuffer[i][j]) >= 0.0025)
					{
						myMap.MCM[i][j] = false;
						myMap.isSolid[i][j] = false;
					}
					// Push the GL attribute bits so that we don't wreck any settings
					glPushAttrib(GL_ALL_ATTRIB_BITS);
					// Enable polygon offsets, and offset filled polygons forward by 2.5
					// Set the render mode to be line rendering with a thick line width
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDisable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(-2.5f, -2.5f);
					glLineWidth(OUTLINE);
					// Set the colour to be white
					glColor3f(.5, .5, .5);
					// Render the object
					// draw map border
					//DO TOON SHADING
					PatternSilh->Use();
					drawCube(myMap.coord[i][j][0], myMap.coord[i][j][1], myMap.coord[i][j][2], myMap.color[i][j][0], myMap.color[i][j][1], myMap.color[i][j][2]);
					PatternSilh->Use(0);
					glEnable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(-2.5f, -2.5f);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					//glShadeModel(GL_FLAT);
					//glEnable(GL_LIGHTING);
					//SetPointLight(GL_LIGHT1, 0, 60, 90, 0.65, 0.5, 0.5);
					//glColor3f(0.0f, 0.0f, 0.0f);
					Pattern->Use();
					drawCube(myMap.coord[i][j][0], myMap.coord[i][j][1], myMap.coord[i][j][2], myMap.color[i][j][0], myMap.color[i][j][1], myMap.color[i][j][2]);
					glPopAttrib();
					//glDisable(GL_LIGHT1);
					//glDisable(GL_LIGHTING);
					Pattern->Use(0);

				}
				if ((myMap.MCM[i][j] && !myMap.isSolid[i][j]) || (myMap.color[i][j][0] == 7))
				{

					// Push the GL attribute bits so that we don't wreck any settings
					glPushAttrib(GL_ALL_ATTRIB_BITS);
					// Enable polygon offsets, and offset filled polygons forward by 2.5
					// Set the render mode to be line rendering with a thick line width
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDisable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(-2.5f, -2.5f);
					glLineWidth(OUTLINE);
					// Set the colour to be white
					glColor3f(.5, .5, .5);
					// Render the object
					PatternSilh->Use();
					drawTreeCube(myMap.coord[i][j][0], myMap.coord[i][j][1], myMap.angle[i][j], myMap.color[i][j][0]);
					PatternSilh->Use(0);
					glEnable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(-2.5f, -2.5f);
					// Set the polygon mode to be filled triangles
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_FLAT);
					//glEnable(GL_LIGHTING);
					//SetPointLight(GL_LIGHT1, 20, 50, 35, 0.9, 0.9, 0.9);
					glColor3f(0.0f, 0.0f, 0.0f);
					PatternTree->Use();
					drawTreeCube(myMap.coord[i][j][0], myMap.coord[i][j][1], myMap.angle[i][j], myMap.color[i][j][0]);
					PatternTree->Use(0);
					glPopAttrib();
					//glDisable(GL_LIGHT1);
					//glDisable(GL_LIGHTING);

				}
			}
		}

		//Smoke
		for (std::vector<Smoke>::iterator it = Smokes.begin(); it != Smokes.end();) {
			if (it->smokeActive) {
				// Push the GL attribute bits so that we don't wreck any settings
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				// Enable polygon offsets, and offset filled polygons forward by 2.5
				// Set the render mode to be line rendering with a thick line width
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(-2.5f, -2.5f);
				glLineWidth(OUTLINE);
				// Set the colour to be white
				glColor3f(.5, .5, .5);
				// Render the object
				PatternSilh->Use();
				drawSmoke(it->smokeCoordBuffer[0], it->smokeCoordBuffer[1], 0, it->smokeAngleBuffer, 0.05, 0.59, 0.52, 0.48, it->smokeIDBuffer, it->smokeDurBuffer);
				PatternSilh->Use(0);
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(-2.5f, -2.5f);
				// Set the polygon mode to be filled triangles 
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glShadeModel(GL_FLAT);
				glEnable(GL_LIGHTING);
				SetPointLight(GL_LIGHT1, 20, 50, 35, 0.75, 0.75, 0.75);
				glColor3f(0.0f, 0.0f, 0.0f);
				drawSmoke(it->smokeCoordBuffer[0], it->smokeCoordBuffer[1], 0, it->smokeAngleBuffer, 0.05, 0.59, 0.52, 0.48, it->smokeIDBuffer, it->smokeDurBuffer);
				glPopAttrib();
				glDisable(GL_LIGHT1);
				glDisable(GL_LIGHTING);
				if ((Time - it->smokeIDBuffer) > it->smokeDurBuffer)
				{
					it->smokeActive = false;
					it = Smokes.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

		//draw crates
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		// Enable polygon offsets, and offset filled polygons forward by 2.5
		// Set the render mode to be line rendering with a thick line width
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		glLineWidth(OUTLINE);
		// Set the colour to be white
		glColor3f(.5, .5, .5);
		// Render the object
		PatternSilh->Use();
		for (std::vector<Crate>::iterator it = Crates.begin(); it != Crates.end(); ++it)
		{

			if (it->isActive)
				switch (it->type)
				{
				case 0:
					drawAmmo(it->X, it->Y);
					break;
				case 1:
					drawSmokeCrate(it->X, it->Y);
					break;
				case 2:
					drawHPCrate(it->X, it->Y);
					break;
				case 3:
					drawMine(it->X, it->Y);
					break;
				case 4:
					drawRadioCrate(it->X, it->Y);
					break;
				}
		}

		PatternSilh->Use(0);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		// Set the polygon mode to be filled triangles 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_FLAT);
		//glEnable(GL_LIGHTING);
		//SetPointLight(GL_LIGHT1, 20, 50, 35, 0.75, 0.75, 0.75);
		Pattern->Use();
		glColor3f(0.0f, 0.0f, 0.0f);
		for (std::vector<Crate>::iterator it = Crates.begin(); it != Crates.end(); ++it)
		{

			if (it->isActive)
				switch (it->type)
				{
				case 0:
					drawAmmo(it->X, it->Y);
					break;
				case 1:
					drawSmokeCrate(it->X, it->Y);
					break;
				case 2:
					drawHPCrate(it->X, it->Y);
					break;
				case 3:
					drawMine(it->X, it->Y);
					break;
				case 4:
					drawRadioCrate(it->X, it->Y);
					break;
				}
		}

		glPopAttrib();
		Pattern->Use(0);
		//glDisable(GL_LIGHT1);
		//glDisable(GL_LIGHTING);
		// draw shell
		for (int i = 0; i < SHELLMAX; i++)
		{
			if ((Time - Shells[i].startTime) < SHELLDURATION && Shells[i].active)
			{
				// Push the GL attribute bits so that we don't wreck any settings
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				// Enable polygon offsets, and offset filled polygons forward by 2.5
				// Set the render mode to be line rendering with a thick line width
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(-2.5f, -2.5f);
				glLineWidth(OUTLINE);
				// Set the colour to be white
				glColor3f(.5, .5, .5);
				// Render the object
				PatternSilh->Use();
				drawShell(Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180.0)), Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180.0)), Shells[i].angle);
				PatternSilh->Use(0);
				glEnable(GL_POLYGON_OFFSET_FILL);
				glPolygonOffset(-2.5f, -2.5f);
				// Set the polygon mode to be filled triangles 
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glShadeModel(GL_FLAT);
				//glEnable(GL_LIGHTING);
				SetPointLight(GL_LIGHT1, 20, 50, 35, 0.75, 0.75, 0.75);
				glColor3f(0.0f, 0.0f, 0.0f);
				drawShell(Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180.0)), Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180.0)), Shells[i].angle);
				glPopAttrib();
				glDisable(GL_LIGHT1);
				glDisable(GL_LIGHTING);
				// Calculate trajectory:
				if (
					((Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180.0)) < IS3XY[0] + BODY) && (Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180.0)) > IS3XY[0] - BODY)) &&
					((Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180.0)) < IS3XY[1] + BODY) && (Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180.0)) > IS3XY[1] - BODY)) &&
					Shells[i].shooterId == ABRAMID
					)
				{
					//spark
					//Dammage:
					IS3HP -= 2 * fabs(fabs(sin((Shells[i].angle - IS3HullAngle)* PI / 180.0)) - fabs(cos((Shells[i].angle - IS3HullAngle)* PI / 180.0)));
					// Bounce!
					if (fabs(fabs(sin((Shells[i].angle - IS3HullAngle)* PI / 180.0)) - fabs(cos((Shells[i].angle - IS3HullAngle)* PI / 180.0))) < BOUNCETHRESH)
					{
						alSourcePlay(Sources[9]);
						if (IS3XY[0] > Shells[i].x)
						{
							if (IS3XY[1] > Shells[i].y)
								if (IS3XY[0] - Shells[i].x >= IS3XY[1] - Shells[i].y)
									if (fabs(sin((Shells[i].angle - IS3HullAngle)* PI / 180.0)) - fabs(cos((Shells[i].angle - IS3HullAngle)* PI / 180.0)) >= 0)
										Shells[i].angle += 90;
									else
										Shells[i].angle -= 90;
								else
									if (fabs(sin((Shells[i].angle - IS3HullAngle)* PI / 180.0)) - fabs(cos((Shells[i].angle - IS3HullAngle)* PI / 180.0)) >= 0)
										Shells[i].angle -= 90;
									else
										Shells[i].angle += 90;
							else
								if (IS3XY[0] - Shells[i].x >= IS3XY[1] - Shells[i].y)
									if (fabs(sin((Shells[i].angle - IS3HullAngle)* PI / 180.0)) - fabs(cos((Shells[i].angle - IS3HullAngle)* PI / 180.0)) >= 0)
										Shells[i].angle -= 90;
									else
										Shells[i].angle += 90;
								else
									if (fabs(sin((Shells[i].angle - IS3HullAngle)* PI / 180.0)) - fabs(cos((Shells[i].angle - IS3HullAngle)* PI / 180.0)) >= 0)
										Shells[i].angle += 90;
									else
										Shells[i].angle -= 90;
						}
						else
						{
							if (IS3XY[1] > Shells[i].y)
								if (IS3XY[0] - Shells[i].x >= IS3XY[1] - Shells[i].y)
									Shells[i].angle -= 90;
								else
									Shells[i].angle += 90;
							else
								if (IS3XY[0] - Shells[i].x >= IS3XY[1] - Shells[i].y)
									Shells[i].angle += 90;
								else
									Shells[i].angle -= 90;
						}
						Shells[i].startTime = Time;
						Shells[i].shooterId = IS3ID;
						Shells[i].x = IS3XY[0];
						Shells[i].y = IS3XY[1];
					}
					else
					{
						Shells[i].active = false;
					}
				}
				if (
					((Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180.0)) < AbramXY[0] + BODY) && (Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180.0)) > AbramXY[0] - BODY)) &&
					((Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180.0)) < AbramXY[1] + BODY) && (Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180.0)) > AbramXY[1] - BODY)) &&
					Shells[i].shooterId == IS3ID
					)
				{
					//spark
					//Dammage:
					AbramHP -= 2 * fabs(fabs(sin((Shells[i].angle - AbramHullAngle)* PI / 180.0)) - fabs(cos((Shells[i].angle - AbramHullAngle)* PI / 180.0)));
					// Bounce!
					if (fabs(fabs(sin((Shells[i].angle - AbramHullAngle)* PI / 180.0)) - fabs(cos((Shells[i].angle - AbramHullAngle)* PI / 180.0))) < BOUNCETHRESH)
					{
						alSourcePlay(Sources[9]);
						if (AbramXY[0] > Shells[i].x)
						{
							if (AbramXY[1] > Shells[i].y)
								if (AbramXY[0] - Shells[i].x >= AbramXY[1] - Shells[i].y)
									if (fabs(sin((Shells[i].angle - AbramHullAngle)* PI / 180.0)) - fabs(cos((Shells[i].angle - AbramHullAngle)* PI / 180.0)) >= 0)
										Shells[i].angle += 90;
									else
										Shells[i].angle -= 90;
								else
									if (fabs(sin((Shells[i].angle - AbramHullAngle)* PI / 180.0)) - fabs(cos((Shells[i].angle - AbramHullAngle)* PI / 180.0)) >= 0)
										Shells[i].angle -= 90;
									else
										Shells[i].angle += 90;
							else
								if (AbramXY[0] - Shells[i].x >= AbramXY[1] - Shells[i].y)
									if (fabs(sin((Shells[i].angle - AbramHullAngle)* PI / 180.0)) - fabs(cos((Shells[i].angle - AbramHullAngle)* PI / 180.0)) >= 0)
										Shells[i].angle -= 90;
									else
										Shells[i].angle += 90;
								else
									if (fabs(sin((Shells[i].angle - AbramHullAngle)* PI / 180.0)) - fabs(cos((Shells[i].angle - AbramHullAngle)* PI / 180.0)) >= 0)
										Shells[i].angle += 90;
									else
										Shells[i].angle -= 90;
						}
						else
						{
							if (AbramXY[1] > Shells[i].y)
								if (AbramXY[0] - Shells[i].x >= AbramXY[1] - Shells[i].y)
									Shells[i].angle -= 90;
								else
									Shells[i].angle += 90;
							else
								if (AbramXY[0] - Shells[i].x >= AbramXY[1] - Shells[i].y)
									Shells[i].angle += 90;
								else
									Shells[i].angle -= 90;
						}
						Shells[i].startTime = Time;
						Shells[i].shooterId = ABRAMID;
						Shells[i].x = AbramXY[0];
						Shells[i].y = AbramXY[1];
					}
					else
					{
						Shells[i].active = false;
					}
				}
				if (
					(Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180.0)) < -MAPEDGEX) ||
					(Shells[i].x - ((Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180.0)) > MAPEDGEX) ||
					(Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180.0)) < -MAPEDGEY) ||
					(Shells[i].y - ((Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180.0)) > MAPEDGEY)
					)
					Shells[i].active = false;
				int tmpi;
				int tmpj;
				//CrateCollisionModel

				std::vector<Crate>::iterator cratecheck = CrateCollisionModel(Shells[i].x, Shells[i].y,
					(Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180.0),
					(Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180.0),
					-1
				);
				if (cratecheck != Crates.end())
				{
					cratecheck->isActive = false;
					myMap.isCrate[cratecheck->i][cratecheck->j] = false;
					Crates.erase(cratecheck);
				}
				if (
					MapCollisionModel(Shells[i].x, Shells[i].y,
					(Time - Shells[i].startTime) * SHELLSPEED * sin(Shells[i].angle * PI / 180.0),
						(Time - Shells[i].startTime) * SHELLSPEED * cos(Shells[i].angle * PI / 180.0),
						-1,
						0,
						&tmpi,
						&tmpj
					)
					)
				{
					if (myMap.MCM[tmpi][tmpj] && myMap.isSolid[tmpi][tmpj] && myMap.color[tmpi][tmpj][0] != 7)
					{
						int wallState = rand() % 23;
						//wallState = 22;
						myMap.isCrate[tmpi][tmpj] = true;
						Crate tmpCrate;
						tmpCrate.X = myMap.coord[tmpi][tmpj][0];
						tmpCrate.Y = myMap.coord[tmpi][tmpj][1];
						tmpCrate.isActive = true;
						tmpCrate.i = tmpi;
						tmpCrate.j = tmpj;
						switch (wallState)
						{
						case 0:
						case 1:
						case 2:
						case 3:
						case 4:
						case 5:
						case 6:
						case 7:
							tmpCrate.type = AMMOCRATE;
							break;
						case 8:
						case 9:
						case 10:
						case 11:
						case 12:
						case 13:
							tmpCrate.type = SMOKECRATE;
							break;
						case 14:
						case 15:
						case 16:
						case 17:
						case 18:
						case 19:
							tmpCrate.type = HPCRATE;
							break;
						case 20:
						case 21:
							tmpCrate.type = MINECRATE;
							break;
						case 22:
							tmpCrate.type = RADIO;
							break;
						}
						Crates.push_back(tmpCrate);
						for (int i = 0; i < 5; i++)
						{
							struct Smoke tmpSmoke;
							tmpSmoke.smokeIDBuffer = Time;
							tmpSmoke.smokeCoordBuffer[0] = myMap.coord[tmpi][tmpj][0];
							tmpSmoke.smokeCoordBuffer[1] = myMap.coord[tmpi][tmpj][1];
							tmpSmoke.smokeDurBuffer = 0.02;
							tmpSmoke.smokeAngleBuffer = rand() % 360;
							tmpSmoke.smokeIDBufferSet = true;
							tmpSmoke.smokeActive = true;
							Smokes.push_back(tmpSmoke);
							
						}
						destructionTimeBuffer[tmpi][tmpj] = Time;
						Shells[i].active = false;
					}
				}
			}
		}
		for (int i = 0; i < BULLETMAX; i++)
		{
			if ((Time - Bullets[i].startTime) < SHELLDURATION && Bullets[i].active)
			{
				drawBullet(Bullets[i].x - ((Time - Bullets[i].startTime) * BULLETSPEED * sin(Bullets[i].angle * PI / 180.0)), Bullets[i].y - ((Time - Bullets[i].startTime) * BULLETSPEED * cos(Bullets[i].angle * PI / 180.0)), Bullets[i].angle);

				// Calculate trajectory:
				if (
					((Bullets[i].x - ((Time - Bullets[i].startTime) * BULLETSPEED * sin(Bullets[i].angle * PI / 180.0)) < IS3XY[0] + BODY) && (Bullets[i].x - ((Time - Bullets[i].startTime) * BULLETSPEED * sin(Bullets[i].angle * PI / 180.0)) > IS3XY[0] - BODY)) &&
					((Bullets[i].y - ((Time - Bullets[i].startTime) * BULLETSPEED * cos(Bullets[i].angle * PI / 180.0)) < IS3XY[1] + BODY) && (Bullets[i].y - ((Time - Bullets[i].startTime) * BULLETSPEED * cos(Bullets[i].angle * PI / 180.0)) > IS3XY[1] - BODY)) &&
					Bullets[i].shooterId == ABRAMID
					)
				{
					//Smoke:
					struct Smoke tmpSmoke;
					tmpSmoke.smokeIDBuffer = Time;
					tmpSmoke.smokeCoordBuffer[0] = IS3XY[0] + 1 * (sin(rand() % 10 *  PI / 6));
					tmpSmoke.smokeCoordBuffer[1] = IS3XY[1] + 1 * (cos(rand() % 10 *  PI / 6));
					tmpSmoke.smokeDurBuffer = 0.01;
					tmpSmoke.smokeAngleBuffer = rand() % 360;
					tmpSmoke.smokeIDBufferSet = true;
					tmpSmoke.smokeActive = true;
					Smokes.push_back(tmpSmoke);
					//spark
					//Dammage:
					IS3HP -= 0.1 * fabs(fabs(sin((Bullets[i].angle - IS3HullAngle)* PI / 180.0)) - fabs(cos((Bullets[i].angle - IS3HullAngle)* PI / 180.0)));
					// Bounce!
					if (fabs(fabs(sin((Bullets[i].angle - IS3HullAngle)* PI / 180.0)) - fabs(cos((Bullets[i].angle - IS3HullAngle)* PI / 180.0))) < BOUNCETHRESH)
					{
						alSourcePlay(Sources[9]);
						if (IS3XY[0] > Bullets[i].x)
						{
							if (IS3XY[1] > Bullets[i].y)
								if (IS3XY[0] - Bullets[i].x >= IS3XY[1] - Bullets[i].y)
									if (fabs(sin((Bullets[i].angle - IS3HullAngle)* PI / 180.0)) - fabs(cos((Bullets[i].angle - IS3HullAngle)* PI / 180.0)) >= 0)
										Bullets[i].angle += 90;
									else
										Bullets[i].angle -= 90;
								else
									if (fabs(sin((Bullets[i].angle - IS3HullAngle)* PI / 180.0)) - fabs(cos((Bullets[i].angle - IS3HullAngle)* PI / 180.0)) >= 0)
										Bullets[i].angle -= 90;
									else
										Bullets[i].angle += 90;
							else
								if (IS3XY[0] - Bullets[i].x >= IS3XY[1] - Bullets[i].y)
									if (fabs(sin((Bullets[i].angle - IS3HullAngle)* PI / 180.0)) - fabs(cos((Bullets[i].angle - IS3HullAngle)* PI / 180.0)) >= 0)
										Bullets[i].angle -= 90;
									else
										Bullets[i].angle += 90;
								else
									if (fabs(sin((Bullets[i].angle - IS3HullAngle)* PI / 180.0)) - fabs(cos((Bullets[i].angle - IS3HullAngle)* PI / 180.0)) >= 0)
										Bullets[i].angle += 90;
									else
										Bullets[i].angle -= 90;
						}
						else
						{
							if (IS3XY[1] > Bullets[i].y)
								if (IS3XY[0] - Bullets[i].x >= IS3XY[1] - Bullets[i].y)
									Bullets[i].angle -= 90;
								else
									Bullets[i].angle += 90;
							else
								if (IS3XY[0] - Bullets[i].x >= IS3XY[1] - Bullets[i].y)
									Bullets[i].angle += 90;
								else
									Bullets[i].angle -= 90;
						}
						Bullets[i].startTime = Time;
						Bullets[i].shooterId = IS3ID;
						Bullets[i].x = IS3XY[0];
						Bullets[i].y = IS3XY[1];
					}
					else
					{
						Bullets[i].active = false;
					}
				}
				if (
					((Bullets[i].x - ((Time - Bullets[i].startTime) * BULLETSPEED * sin(Bullets[i].angle * PI / 180.0)) < AbramXY[0] + BODY) && (Bullets[i].x - ((Time - Bullets[i].startTime) * BULLETSPEED * sin(Bullets[i].angle * PI / 180.0)) > AbramXY[0] - BODY)) &&
					((Bullets[i].y - ((Time - Bullets[i].startTime) * BULLETSPEED * cos(Bullets[i].angle * PI / 180.0)) < AbramXY[1] + BODY) && (Bullets[i].y - ((Time - Bullets[i].startTime) * BULLETSPEED * cos(Bullets[i].angle * PI / 180.0)) > AbramXY[1] - BODY)) &&
					Bullets[i].shooterId == IS3ID
					)
				{
					//Smoke:
					struct Smoke tmpSmoke;
					tmpSmoke.smokeIDBuffer = Time;
					tmpSmoke.smokeCoordBuffer[0] = AbramXY[0] + 1 * (sin(rand() % 10 *  PI / 6));
					tmpSmoke.smokeCoordBuffer[1] = AbramXY[1] + 1 * (cos(rand() % 10 *  PI / 6));
					tmpSmoke.smokeDurBuffer = 0.01;
					tmpSmoke.smokeAngleBuffer = rand() % 360;
					tmpSmoke.smokeIDBufferSet = true;
					tmpSmoke.smokeActive = true;
					Smokes.push_back(tmpSmoke);
					//spark
					//Dammage:
					AbramHP -= 0.1 * fabs(fabs(sin((Bullets[i].angle - AbramHullAngle)* PI / 180.0)) - fabs(cos((Bullets[i].angle - AbramHullAngle)* PI / 180.0)));
					// Bounce!
					if (fabs(fabs(sin((Bullets[i].angle - AbramHullAngle)* PI / 180.0)) - fabs(cos((Bullets[i].angle - AbramHullAngle)* PI / 180.0))) < BOUNCETHRESH)
					{
						alSourcePlay(Sources[9]);
						if (AbramXY[0] > Bullets[i].x)
						{
							if (AbramXY[1] > Bullets[i].y)
								if (AbramXY[0] - Bullets[i].x >= AbramXY[1] - Bullets[i].y)
									if (fabs(sin((Bullets[i].angle - AbramHullAngle)* PI / 180.0)) - fabs(cos((Bullets[i].angle - AbramHullAngle)* PI / 180.0)) >= 0)
										Bullets[i].angle += 90;
									else
										Bullets[i].angle -= 90;
								else
									if (fabs(sin((Bullets[i].angle - AbramHullAngle)* PI / 180.0)) - fabs(cos((Bullets[i].angle - AbramHullAngle)* PI / 180.0)) >= 0)
										Bullets[i].angle -= 90;
									else
										Bullets[i].angle += 90;
							else
								if (AbramXY[0] - Bullets[i].x >= AbramXY[1] - Bullets[i].y)
									if (fabs(sin((Bullets[i].angle - AbramHullAngle)* PI / 180.0)) - fabs(cos((Bullets[i].angle - AbramHullAngle)* PI / 180.0)) >= 0)
										Bullets[i].angle -= 90;
									else
										Bullets[i].angle += 90;
								else
									if (fabs(sin((Bullets[i].angle - AbramHullAngle)* PI / 180.0)) - fabs(cos((Bullets[i].angle - AbramHullAngle)* PI / 180.0)) >= 0)
										Bullets[i].angle += 90;
									else
										Bullets[i].angle -= 90;
						}
						else
						{
							if (AbramXY[1] > Bullets[i].y)
								if (AbramXY[0] - Bullets[i].x >= AbramXY[1] - Bullets[i].y)
									Bullets[i].angle -= 90;
								else
									Bullets[i].angle += 90;
							else
								if (AbramXY[0] - Bullets[i].x >= AbramXY[1] - Bullets[i].y)
									Bullets[i].angle += 90;
								else
									Bullets[i].angle -= 90;
						}
						Bullets[i].startTime = Time;
						Bullets[i].shooterId = ABRAMID;
						Bullets[i].x = AbramXY[0];
						Bullets[i].y = AbramXY[1];
					}
					else
					{
						Bullets[i].active = false;
					}
				}
				if (
					(Bullets[i].x - ((Time - Bullets[i].startTime) * BULLETSPEED * sin(Bullets[i].angle * PI / 180.0)) < -MAPEDGEX) ||
					(Bullets[i].x - ((Time - Bullets[i].startTime) * BULLETSPEED * sin(Bullets[i].angle * PI / 180.0)) > MAPEDGEX) ||
					(Bullets[i].y - ((Time - Bullets[i].startTime) * BULLETSPEED * cos(Bullets[i].angle * PI / 180.0)) < -MAPEDGEY) ||
					(Bullets[i].y - ((Time - Bullets[i].startTime) * BULLETSPEED * cos(Bullets[i].angle * PI / 180.0)) > MAPEDGEY)
					)
					Bullets[i].active = false;
				int tmpi;
				int tmpj;
				//CrateCollisionModel

				std::vector<Crate>::iterator cratecheck = CrateCollisionModel(Bullets[i].x, Bullets[i].y,
					(Time - Bullets[i].startTime) * BULLETSPEED * sin(Bullets[i].angle * PI / 180.0),
					(Time - Bullets[i].startTime) * BULLETSPEED * cos(Bullets[i].angle * PI / 180.0),
					-1
				);
				if (cratecheck != Crates.end())
				{
					//Crates[cratecheck].isActive = false;
					cratecheck->isActive = false;
					//myMap.isCrate[Crates[cratecheck].i][Crates[cratecheck].j] = false;
					myMap.isCrate[cratecheck->i][cratecheck->j] = false;
					Crates.erase(cratecheck);
				}
				if (
					MapCollisionModel(Bullets[i].x, Bullets[i].y,
					(Time - Bullets[i].startTime) *BULLETSPEED * sin(Bullets[i].angle * PI / 180.0),
						(Time - Bullets[i].startTime) * BULLETSPEED * cos(Bullets[i].angle * PI / 180.0),
						-1,
						0,
						&tmpi,
						&tmpj
					)
					)
				{
					if (myMap.MCM[tmpi][tmpj] && myMap.isSolid[tmpi][tmpj] && myMap.color[tmpi][tmpj][0] != 7)
					{
						Bullets[i].active = false;
						struct Smoke tmpSmoke;
						tmpSmoke.smokeIDBuffer = Time;
						tmpSmoke.smokeCoordBuffer[0] = myMap.coord[tmpi][tmpj][0] + 3 * (sin(rand() % 10 * PI / 6));
						tmpSmoke.smokeCoordBuffer[1] = myMap.coord[tmpi][tmpj][1] + 3 * (cos(rand() % 10 * PI / 6));
						tmpSmoke.smokeDurBuffer = 0.01;
						tmpSmoke.smokeAngleBuffer = rand() % 360;
						tmpSmoke.smokeIDBufferSet = true;
						tmpSmoke.smokeActive = true;
						Smokes.push_back(tmpSmoke);
					}
				}
			}
		}
		glDisableVertexAttribArray(0);
	}

	glDisable(GL_DEPTH_TEST);
	if (isInMenu)
		drawMenuText();
	else
		drawinGmaeUI();
	glutSwapBuffers();
	glFlush();
}
float ElapsedSeconds()
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet(GLUT_ELAPSED_TIME);

	// convert it to seconds:

	return (float)ms / 1000.f;
}
void InitGraphics()
{
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// set the initial window configuration:

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(INIT_WINDOW_SIZE, INIT_WINDOW_SIZE);

	// open the window and set its title:

	MainWindow = glutCreateWindow(WINDOWTITLE);
	glutSetWindowTitle(WINDOWTITLE);
	glutFullScreen();           // making the window full screen
	// set the framebuffer clear values:

	glClearColor(BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3]);

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow(MainWindow);
	glutDisplayFunc(Display);
	glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(NULL);
	glutVisibilityFunc(Visibility);
	glutEntryFunc(NULL);
	glutSpecialFunc(keySpecial);
	glutSpaceballMotionFunc(NULL);
	glutSpaceballRotateFunc(NULL);
	glutSpaceballButtonFunc(NULL);
	glutButtonBoxFunc(NULL);
	glutDialsFunc(NULL);
	glutTabletMotionFunc(NULL);
	glutTabletButtonFunc(NULL);
	glutMenuStateFunc(NULL);
	glutTimerFunc(-1, NULL, 0);
	glutIdleFunc(Animate);

	// init glew (a window must be open to do this):


	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "glewInit Error\n");
	}
	else
		fprintf(stderr, "GLEW initialized OK\n");
	fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	Pattern = new GLSLProgram();
	PatternGrass = new GLSLProgram();
	PatternTree = new GLSLProgram();
	PatternCamo = new GLSLProgram();
	PatternSilh = new GLSLProgram();
	bool valid = Pattern->Create((char *)"shaders/lighting.vert", (char *)"shaders/lighting.frag");
	if (!valid)
	{
		fprintf(stderr, "Shader cannot be created!\n");
		Quit();//DoMainMenu(QUIT);
	}
	else
	{
		fprintf(stderr, "Shader created.\n");
	}
	Pattern->SetVerbose(false);

	bool validGrass = PatternGrass->Create((char *)"shaders/grass.vert", (char *)"shaders/grass.frag");
	if (!validGrass)
	{
		fprintf(stderr, "Shader cannot be created!\n");
		Quit();//DoMainMenu(QUIT);
	}
	else
	{
		fprintf(stderr, "Shader created.\n");
	}
	PatternGrass->SetVerbose(false);

	bool validTree = PatternTree->Create((char *)"shaders/tree.vert", (char *)"shaders/tree.frag");
	if (!validTree)
	{
		fprintf(stderr, "Shader cannot be created!\n");
		Quit();//DoMainMenu(QUIT);
	}
	else
	{
		fprintf(stderr, "Shader created.\n");
	}
	PatternTree->SetVerbose(false);
	
	bool validCamo = PatternCamo->Create((char *)"shaders/camo.vert", (char *)"shaders/camo.frag");
	if (!validCamo)
	{
		fprintf(stderr, "Shader cannot be created!\n");
		Quit();//DoMainMenu(QUIT);
	}
	else
	{
		fprintf(stderr, "Shader created.\n");
	}
	PatternCamo->SetVerbose(false);

	bool validsilh = PatternSilh->Create((char *)"shaders/silhouette.vert", (char *)"shaders/silhouette.frag");
	if (!validsilh)
	{
		fprintf(stderr, "Shader cannot be created!\n");
		Quit();//DoMainMenu(QUIT);
	}
	else
	{
		fprintf(stderr, "Shader created.\n");
	}
	PatternSilh->SetVerbose(false);

	//load graphics
	loadAll();
	loadMap();
	DrawPointer1 = &drawAbram;
	DrawPointer2 = &drawIS3;
	DrawPointer1d = &drawAbramDead;
	DrawPointer2d = &drawIS3Dead;
	if (!glfwInit())
	{
		fprintf(stderr, "GLFW failed!\n");
		Quit();//DoMainMenu(QUIT);
	}
}
void InitializeVertexBuffer(GLuint &theBuffer, GLenum target, GLenum usage, const void* data, int size)
{
	glGenBuffers(1, &theBuffer);
	glBindBuffer(target, theBuffer);
	glBufferData(target, size, data, usage);
	glBindBuffer(target, 0);
}
static void list_audio_devices(const ALCchar *devices)
{
	const ALCchar *device = devices, *next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}
void InitLists()
{
	// List here

	// Setup the sound
	float dx = BOXSIZE / 2.f;
	float dy = BOXSIZE / 2.f;
	float dz = BOXSIZE / 2.f;
	glutSetWindow(MainWindow);
	if (!isSoundLoaded)
	{
		device = alcOpenDevice(NULL);
		if (!device)
			exit(0);
		else
			std::cout << "Audio device was created." << std::endl;
		enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
		if (enumeration == AL_FALSE)
			std::cout << "enumeration not supported." << std::endl; // enumeration not supported
		else
			std::cout << "enumeration supported." << std::endl;// enumeration supported
		list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
		context = alcCreateContext(device, NULL);
		if (!alcMakeContextCurrent(context))
			exit(0);
		else
			std::cout << "Context was created." << std::endl;
		alGenSources((ALuint)1, &mainMusic1);
		alGenSources((ALuint)1, &mainMusic2);
		alGenSources((ALuint)1, &mainMusic3);
		alGenSources((ALuint)1, &mainMusic4);
		alGenSources((ALuint)1, &mainMusic5);
		alGenSources((ALuint)1, &mainMusic6);
		alGenSources((ALuint)1, &mainMusic7);
		alGenSources((ALuint)1, &tankShellFire);
		alGenSources((ALuint)1, &tankShellBounce);
		alGenSources((ALuint)1, &tankExplode);
		alGenSources((ALuint)1, &hpRegen);
		alGenSources((ALuint)1, &AmmoSmoke);
		alGenSources((ALuint)1, &tankBulletFire);
		alGenSources((ALuint)1, &A_10);
		// check for errors

		alSourcef(mainMusic1, AL_PITCH, 1);
		// check for errors
		alSourcef(mainMusic1, AL_GAIN, 1);
		// check for errors
		alSource3f(mainMusic1, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(mainMusic1, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(mainMusic1, AL_LOOPING, AL_FALSE);
		// check for errros

		alSourcef(mainMusic2, AL_PITCH, 1);
		// check for errors
		alSourcef(mainMusic2, AL_GAIN, 1);
		// check for errors
		alSource3f(mainMusic2, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(mainMusic2, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(mainMusic2, AL_LOOPING, AL_FALSE);
		// check for errros

		alSourcef(mainMusic3, AL_PITCH, 1);
		// check for errors
		alSourcef(mainMusic3, AL_GAIN, 1);
		// check for errors
		alSource3f(mainMusic3, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(mainMusic3, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(mainMusic3, AL_LOOPING, AL_FALSE);
		// check for errros

		alSourcef(mainMusic4, AL_PITCH, 1);
		// check for errors
		alSourcef(mainMusic4, AL_GAIN, 1);
		// check for errors
		alSource3f(mainMusic4, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(mainMusic4, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(mainMusic4, AL_LOOPING, AL_FALSE);
		// check for errros

		alSourcef(mainMusic5, AL_PITCH, 1);
		// check for errors
		alSourcef(mainMusic5, AL_GAIN, 1);
		// check for errors
		alSource3f(mainMusic5, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(mainMusic5, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(mainMusic5, AL_LOOPING, AL_FALSE);
		// check for errros

		alSourcef(mainMusic6, AL_PITCH, 1);
		// check for errors
		alSourcef(mainMusic6, AL_GAIN, 1);
		// check for errors
		alSource3f(mainMusic6, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(mainMusic6, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(mainMusic6, AL_LOOPING, AL_FALSE);
		// check for errros

		alSourcef(mainMusic7, AL_PITCH, 1);
		// check for errors
		alSourcef(mainMusic7, AL_GAIN, 1);
		// check for errors
		alSource3f(mainMusic7, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(mainMusic7, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(mainMusic7, AL_LOOPING, AL_FALSE);
		// check for errros



		alSourcef(tankShellFire, AL_PITCH, 1);
		// check for errors
		alSourcef(tankShellFire, AL_GAIN, 1);
		// check for errors
		alSource3f(tankShellFire, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(tankShellFire, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(tankShellFire, AL_LOOPING, AL_FALSE);
		// check for errros

		alSourcef(tankShellBounce, AL_PITCH, 1);
		// check for errors
		alSourcef(tankShellBounce, AL_GAIN, 1);
		// check for errors
		alSource3f(tankShellBounce, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(tankShellBounce, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(tankShellBounce, AL_LOOPING, AL_FALSE);
		// check for errros

		alSourcef(tankExplode, AL_PITCH, 1);
		// check for errors
		alSourcef(tankExplode, AL_GAIN, 1);
		// check for errors
		alSource3f(tankExplode, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(tankExplode, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(tankExplode, AL_LOOPING, AL_FALSE);
		// check for errros

		alSourcef(hpRegen, AL_PITCH, 1);
		// check for errors
		alSourcef(hpRegen, AL_GAIN, 1);
		// check for errors
		alSource3f(hpRegen, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(hpRegen, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(hpRegen, AL_LOOPING, AL_FALSE);
		// check for errros

		alSourcef(AmmoSmoke, AL_PITCH, 1);
		// check for errors
		alSourcef(AmmoSmoke, AL_GAIN, 1);
		// check for errors
		alSource3f(AmmoSmoke, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(AmmoSmoke, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(AmmoSmoke, AL_LOOPING, AL_FALSE);
		// check for errros

		alSourcef(tankBulletFire, AL_PITCH, 1);
		// check for errors
		alSourcef(tankBulletFire, AL_GAIN, 1);
		// check for errors
		alSource3f(tankBulletFire, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(tankBulletFire, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(tankBulletFire, AL_LOOPING, AL_FALSE);
		// check for errros

		alSourcef(A_10, AL_PITCH, 1);
		// check for errors
		alSourcef(A_10, AL_GAIN, 1);
		// check for errors
		alSource3f(A_10, AL_POSITION, 0, 0, 0);
		// check for errors
		alSource3f(A_10, AL_VELOCITY, 0, 0, 0);
		// check for errors
		alSourcei(A_10, AL_LOOPING, AL_FALSE);
		// check for errros

		alGenBuffers(NUM_BUFFERS, Buffers);
		
		int channel, bps;
		data = loadWAV("sound/song1.asset", channel, freq, bps, size,format);
		alBufferData(Buffers[0], format, data, size, freq);

		data = loadWAV("sound/song2.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[1], format, data, size, freq);

		data = loadWAV("sound/song3.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[2], format, data, size, freq);

		data = loadWAV("sound/song4.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[3], format, data, size, freq);

		data = loadWAV("sound/song5.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[4], format, data, size, freq);

		data = loadWAV("sound/song6.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[5], format, data, size, freq);

		data = loadWAV("sound/song7.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[6], format, data, size, freq);

		data = loadWAV("sound/song8.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[7], format, data, size, freq);

		data = loadWAV("sound/shot.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[8], format, data, size, freq);

		data = loadWAV("sound/bounce.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[9], format, data, size, freq);

		data = loadWAV("sound/explosion.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[10], format, data, size, freq);

		data = loadWAV("sound/hp.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[11], format, data, size, freq);

		data = loadWAV("sound/ammo.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[12], format, data, size, freq);

		data = loadWAV("sound/bullet.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[13], format, data, size, freq);

		data = loadWAV("sound/a-10.asset", channel, freq, bps, size, format);
		alBufferData(Buffers[14], format, data, size, freq);

		alGenSources(NUM_SOURCES, Sources);

		alSourcei(Sources[0], AL_BUFFER, Buffers[0]);
		alSourcef(Sources[0], AL_PITCH, 1.0);
		alSourcef(Sources[0], AL_GAIN, 1.0);
		alSource3f(Sources[0], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[0], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[0], AL_LOOPING, AL_TRUE);

		alSourcei(Sources[1], AL_BUFFER, Buffers[1]);
		alSourcef(Sources[1], AL_PITCH, 1.0);
		alSourcef(Sources[1], AL_GAIN, 1.0);
		alSource3f(Sources[1], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[1], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[1], AL_LOOPING, AL_TRUE);

		alSourcei(Sources[2], AL_BUFFER, Buffers[2]);
		alSourcef(Sources[2], AL_PITCH, 1.0);
		alSourcef(Sources[2], AL_GAIN, 1.0);
		alSource3f(Sources[2], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[2], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[2], AL_LOOPING, AL_TRUE);

		alSourcei(Sources[3], AL_BUFFER, Buffers[3]);
		alSourcef(Sources[3], AL_PITCH, 1.0);
		alSourcef(Sources[3], AL_GAIN, 1.0);
		alSource3f(Sources[3], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[3], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[3], AL_LOOPING, AL_TRUE);

		alSourcei(Sources[4], AL_BUFFER, Buffers[4]);
		alSourcef(Sources[4], AL_PITCH, 1.0);
		alSourcef(Sources[4], AL_GAIN, 1.0);
		alSource3f(Sources[4], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[4], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[4], AL_LOOPING, AL_TRUE);

		alSourcei(Sources[5], AL_BUFFER, Buffers[5]);
		alSourcef(Sources[5], AL_PITCH, 1.0);
		alSourcef(Sources[5], AL_GAIN, 1.0);
		alSource3f(Sources[5], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[5], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[5], AL_LOOPING, AL_TRUE);

		alSourcei(Sources[6], AL_BUFFER, Buffers[6]);
		alSourcef(Sources[6], AL_PITCH, 1.0);
		alSourcef(Sources[6], AL_GAIN, 1.0);
		alSource3f(Sources[6], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[6], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[6], AL_LOOPING, AL_TRUE);

		alSourcei(Sources[7], AL_BUFFER, Buffers[7]);
		alSourcef(Sources[7], AL_PITCH, 1.0);
		alSourcef(Sources[7], AL_GAIN, 1.0);
		alSource3f(Sources[7], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[7], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[7], AL_LOOPING, AL_TRUE);

		alSourcei(Sources[8], AL_BUFFER, Buffers[8]);
		alSourcef(Sources[8], AL_PITCH, 1.0);
		alSourcef(Sources[8], AL_GAIN, 1.0);
		alSource3f(Sources[8], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[8], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[8], AL_LOOPING, AL_FALSE);

		alSourcei(Sources[9], AL_BUFFER, Buffers[9]);
		alSourcef(Sources[9], AL_PITCH, 1.0);
		alSourcef(Sources[9], AL_GAIN, 1.0);
		alSource3f(Sources[9], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[9], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[9], AL_LOOPING, AL_FALSE);

		alSourcei(Sources[10], AL_BUFFER, Buffers[10]);
		alSourcef(Sources[10], AL_PITCH, 1.0);
		alSourcef(Sources[10], AL_GAIN, 1.0);
		alSource3f(Sources[10], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[10], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[10], AL_LOOPING, AL_FALSE);

		alSourcei(Sources[11], AL_BUFFER, Buffers[11]);
		alSourcef(Sources[11], AL_PITCH, 1.0);
		alSourcef(Sources[11], AL_GAIN, 1.0);
		alSource3f(Sources[11], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[11], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[11], AL_LOOPING, AL_FALSE);

		alSourcei(Sources[12], AL_BUFFER, Buffers[12]);
		alSourcef(Sources[12], AL_PITCH, 1.0);
		alSourcef(Sources[12], AL_GAIN, 1.0);
		alSource3f(Sources[12], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[12], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[12], AL_LOOPING, AL_FALSE);

		alSourcei(Sources[13], AL_BUFFER, Buffers[13]);
		alSourcef(Sources[13], AL_PITCH, 1.0);
		alSourcef(Sources[13], AL_GAIN, 1.0);
		alSource3f(Sources[13], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[13], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[13], AL_LOOPING, AL_FALSE);

		alSourcei(Sources[14], AL_BUFFER, Buffers[14]);
		alSourcef(Sources[14], AL_PITCH, 1.0);
		alSourcef(Sources[14], AL_GAIN, 1.0);
		alSource3f(Sources[14], AL_POSITION, 0, 0, 0);
		alSource3f(Sources[14], AL_VELOCITY, 0, 0, 0);
		alSourcei(Sources[14], AL_LOOPING, AL_FALSE);

		isSoundLoaded = true;
	}
	

	// create the object:
	BoxList = glGenLists(1);
	glNewList(BoxList, GL_COMPILE);
	// Random Objects placed here (polyStack)
	//Grass
	float startx = MAPEDGEX + CUBESIZE;
	float startz = MAPEDGEY + CUBESIZE;
	float endx = (-MAPEDGEX - CUBESIZE);
	float endz = (-MAPEDGEY - CUBESIZE);
	float lengthx = startx - endx;
	float lengthz = startz - endz;
	int grainX = GRASSGRAINX*userGrassMultiplier;
	int grainY = GRASSGRAINY*userGrassMultiplier;
	glBegin(GL_QUADS);
	glPushMatrix();
	glColor3f(0.1, 0.1, 0.0);
	for (int i = 0; i < grainX; i++)
	{
		for (int j = 0; j < grainY; j++)
		{
			glVertex3f(startx - i*(lengthx) / grainX, MENUYOFFSET, startz - j*(lengthz) / grainY);
			glVertex3f(startx - i*(lengthx) / grainX, MENUYOFFSET, startz - (j + 1)*(lengthz) / grainY);
			glVertex3f(startx - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - (j + 1)*(lengthz) / grainY);
			glVertex3f(startx - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - j*(lengthz) / grainY);
		}
	}
	glPopMatrix();
	glEnd();

	glEndList();

	// Generate the textures
	
	AxesList = glGenLists(1);
	glNewList(AxesList, GL_COMPILE);
	glLineWidth(AXES_WIDTH);
	Axes(1.5);
	glLineWidth(1.);
	glEndList();
}
void Keyboard(unsigned char c, int x, int y)
{
	if (isInMenu)
	{
		if (ADVANCEMENU)
		{
			switch (menuState)
			{
			case 0:
			{
				if (DebugOn != 0)
					fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

				switch (c)
				{
				case 'w':
				case 'W':
					if (selectIndex >= 1)
						selectIndex--;
					else
						selectIndex = 4;
					selectIndex = selectIndex % 5;
					break;
				case 's':
				case 'S':
					selectIndex++;
					selectIndex = selectIndex % 5;
					break;

				case ' ':
				case 13:
					switch (selectIndex)
					{
					case 0:
						menuState = 1;
						break;
					case 1:
						menuState = 2;
						break;
					case 2:
						menuState = 4;
						break;
					case 3:
						menuState = 5;
						break;
					case 4:
						menuState = 6;
						break;
					}
					selectIndex = 0;
					break;
				case 'q':
				case 'Q':
				case ESCAPE:
					Quit();
					break;

				}
				glutSetWindow(MainWindow);
				glutPostRedisplay();
			}
			break;
			case 1:
			case 2:
			{
				if (DebugOn != 0)
					fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

				switch (c)
				{
				case 'a':
				case 'A':
					PlayerOne += 1;
					PlayerOne = PlayerOne % TOTALTANKS;
					switch (PlayerOne)
					{
					case 0:
						DrawPointer1 = &drawAbram;
						DrawPointer1d = &drawAbramDead;
						break;
					case 1:
						DrawPointer1 = &drawIS3;
						DrawPointer1d = &drawIS3Dead;
						break;
					case 2:
						DrawPointer1 = &drawT29;
						DrawPointer1d = &drawT29Dead;
						break;
					case 3:
						DrawPointer1 = &drawE100;
						DrawPointer1d = &drawE100Dead;
						break;
					case 4:
						DrawPointer1 = &drawType59;
						DrawPointer1d = &drawType59Dead;
						break;
					}
					break;
				case 'd':
				case 'D':
					PlayerOne += TOTALTANKS; // 4 + 5 = 9, 9 % 5 = 4
					PlayerOne = (PlayerOne - 1) % TOTALTANKS;
					switch (PlayerOne)
					{
					case 0:
						DrawPointer1 = &drawAbram;
						DrawPointer1d = &drawAbramDead;
						break;
					case 1:
						DrawPointer1 = &drawIS3;
						DrawPointer1d = &drawIS3Dead;
						break;
					case 2:
						DrawPointer1 = &drawT29;
						DrawPointer1d = &drawT29Dead;
						break;
					case 3:
						DrawPointer1 = &drawE100;
						DrawPointer1d = &drawE100Dead;
						break;
					case 4:
						DrawPointer1 = &drawType59;
						DrawPointer1d = &drawType59Dead;
						break;
					}
					break;
				case 'j':
				case 'J':
				case '4':
					PlayerTwo += 1;
					PlayerTwo = PlayerTwo % TOTALTANKS;
					switch (PlayerTwo)
					{
					case 0:
						DrawPointer2 = &drawAbram;
						DrawPointer2d = &drawAbramDead;
						break;
					case 1:
						DrawPointer2 = &drawIS3;
						DrawPointer2d = &drawIS3Dead;
						break;
					case 2:
						DrawPointer2 = &drawT29;
						DrawPointer2d = &drawT29Dead;
						break;
					case 3:
						DrawPointer2 = &drawE100;
						DrawPointer2d = &drawE100Dead;
						break;
					case 4:
						DrawPointer2 = &drawType59;
						DrawPointer2d = &drawType59Dead;
						break;
					}
					break;
				case 'l':
				case 'L':
				case '6':
					PlayerTwo += TOTALTANKS;
					PlayerTwo = (PlayerTwo - 1) % TOTALTANKS;
					switch (PlayerTwo)
					{
					case 0:
						DrawPointer2 = &drawAbram;
						DrawPointer2d = &drawAbramDead;
						break;
					case 1:
						DrawPointer2 = &drawIS3;
						DrawPointer2d = &drawIS3Dead;
						break;
					case 2:
						DrawPointer2 = &drawT29;
						DrawPointer2d = &drawT29Dead;
						break;
					case 3:
						DrawPointer2 = &drawE100;
						DrawPointer2d = &drawE100Dead;
						break;
					case 4:
						DrawPointer2 = &drawType59;
						DrawPointer2d = &drawType59Dead;
						break;
					}
					break;

				case ' ':
				case 13:
					selectIndex = 0;
					mapName = "1";
					lastMap = mapName;
					resetState(mapName);
					isInMenu = true;
					for (int i = 0; i < 8; i++)
						alSourceStop(Sources[i]);
					menuState = 3;
					break;
				case 'q':
				case 'Q':
				case ESCAPE:
					menuState = 0;
					break;

				}
				glutSetWindow(MainWindow);
				glutPostRedisplay();
			}
				break;
			case 3:
			{
				if (DebugOn != 0)
					fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);
				switch (c)
				{
				case 'w':
				case 'W':
					if (selectIndex >= 1)
						selectIndex--;
					else
						selectIndex = 9;
					selectIndex = selectIndex % 10;
					switch (selectIndex)
					{
					case 0:
						mapName = '1';
						break;
					case 1:
						mapName = '2';
						break;
					case 2:
						mapName = '7';
						break;
					case 3:
						mapName = '9';
						break;
					case 4:
						mapName = '5';
						break;
					case 5:
						mapName = '8';
						break;
					case 6:
						mapName = '3';
						break;
					case 7:
						mapName = '6';
						break;
					case 8:
						mapName = '4';
						break;
					case 9:
						mapName = 'R';
						break;
					}
					lastMap = mapName;
					resetState(mapName);
					isInMenu = true;
					for (int i = 0; i < 8; i++)
						alSourceStop(Sources[i]);
					break;
				case 's':
				case 'S':
					selectIndex++;
					selectIndex = selectIndex % 10;
					switch (selectIndex)
					{
					case 0:
						mapName = '1';
						break;
					case 1:
						mapName = '2';
						break;
					case 2:
						mapName = '7';
						break;
					case 3:
						mapName = '9';
						break;
					case 4:
						mapName = '5';
						break;
					case 5:
						mapName = '8';
						break;
					case 6:
						mapName = '3';
						break;
					case 7:
						mapName = '6';
						break;
					case 8:
						mapName = '4';
						break;
					case 9:
						mapName = 'R';
						break;
					}
					lastMap = mapName;
					resetState(mapName);
					isInMenu = true;
					for (int i = 0; i < 8; i++)
						alSourceStop(Sources[i]);
					break;
				case ' ':
				case 13:
					{
						keyBuffer['w'] = false;
						keyBuffer['W'] = false;
						keyBuffer['a'] = false;
						keyBuffer['A'] = false;
						keyBuffer['s'] = false;
						keyBuffer['S'] = false;
						keyBuffer['d'] = false;
						keyBuffer['D'] = false;
						keyBuffer['q'] = false;
						keyBuffer['Q'] = false;
						keyBuffer['e'] = false;
						keyBuffer['E'] = false;
						keyBuffer['f'] = false;
						keyBuffer['F'] = false;
						keyBuffer['c'] = false;
						keyBuffer['C'] = false;
						keyBuffer[' '] = false;

						keyBuffer['i'] = false;
						keyBuffer['I'] = false;
						keyBuffer['8'] = false;
						keyBuffer['j'] = false;
						keyBuffer['J'] = false;
						keyBuffer['4'] = false;
						keyBuffer['k'] = false;
						keyBuffer['K'] = false;
						keyBuffer['5'] = false;
						keyBuffer['l'] = false;
						keyBuffer['L'] = false;
						keyBuffer['6'] = false;
						keyBuffer['u'] = false;
						keyBuffer['U'] = false;
						keyBuffer['7'] = false;
						keyBuffer['o'] = false;
						keyBuffer['O'] = false;
						keyBuffer['9'] = false;
						keyBuffer['h'] = false;
						keyBuffer['H'] = false;
						keyBuffer['.'] = false;
						keyBuffer['n'] = false;
						keyBuffer['N'] = false;
						keyBuffer['0'] = false;

						keyBuffer['g'] = false;
						keyBuffer['G'] = false;
						keyBuffer['-'] = false;
						keyBuffer['_'] = false;
						keyBuffer['='] = false;
						keyBuffer['+'] = false;
					}
					resetState();
					run = true;
					break;
				case 'q':
				case 'Q':
				case ESCAPE:
					mapName = "M";
					lastMap = mapName;
					resetState(mapName);
					isInMenu = true;
					for (int i = 0; i < 8; i++)
						alSourceStop(Sources[i]);
					selectIndex = 0;
					if(isSingle)
						menuState = 1;
					else
						menuState = 2;
					break;

				}
				// force a call to Display( ):

				glutSetWindow(MainWindow);
				glutPostRedisplay();
			}
			break;
			case 4:
			{
				if (DebugOn != 0)
					fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

				switch (c)
				{
				case 'q':
				case 'Q':
				case ESCAPE:
					menuState = 0;
					break;

				}
				// force a call to Display( ):

				glutSetWindow(MainWindow);
				glutPostRedisplay();
			}
				break;
			case 5:
			{
				if (DebugOn != 0)
					fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

				switch (c)
				{
				case 'w':
				case 'W':
					if (selectIndex >= 1)
						selectIndex--;
					else
						selectIndex = 1;
					selectIndex = selectIndex % 2;
					break;
				case 's':
				case 'S':
					selectIndex++;
					selectIndex = selectIndex % 2;
					break;
				case 'a':
				case 'A':
					if (selectIndex == 0)
					{
						if (userGrassMultiplier > 0)
							userGrassMultiplier--;
					}
					if (selectIndex == 1)
					{
						if (TANKSPEED > 0.1)
							TANKSPEED /= 1.5;
					}
					if (selectIndex == 2 && false)
					{

					}
					break;
				case 'd':
				case 'D':
					if (selectIndex == 0)
					{
						if (userGrassMultiplier < 20)
							userGrassMultiplier++;
					}
					if (selectIndex == 1)
					{
						if (TANKSPEED < 1.0)
							TANKSPEED *= 1.5;
					}
					if (selectIndex == 2 && false)
					{

					}
					break;
				case 'q':
				case 'Q':
				case ESCAPE:
					InitLists();
					menuState = 0;
					break;

				}
				// force a call to Display( ):

				glutSetWindow(MainWindow);
				glutPostRedisplay();
			}
				break;
			case 6:
			{
				if (DebugOn != 0)
					fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);
				switch (c)
				{
				case 'q':
				case 'Q':
				case ESCAPE:
					menuState = 0;
					break;
				}
				glutSetWindow(MainWindow);
				glutPostRedisplay();
			}
				break;
			}
		}
		else
		{
			if (DebugOn != 0)
				fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

			switch (c)
			{
			case 'o':
			case 'O':
				WhichProjection = ORTHO;
				break;

			case 'p':
			case 'P':
				WhichProjection = PERSP;
				break;
			case 'w':
			case 'W':
				if (selectIndex >= 1)
					selectIndex--;
				else
					selectIndex = 9;
				selectIndex = selectIndex % 10;
				break;
			case 's':
			case 'S':
				selectIndex++;
				selectIndex = selectIndex % 10;
				break;
			case 'a':
			case 'A':
				PlayerOne += 1;
				PlayerOne = PlayerOne % TOTALTANKS;
				switch (PlayerOne)
				{
				case 0:
					DrawPointer1 = &drawAbram;
					DrawPointer1d = &drawAbramDead;
					break;
				case 1:
					DrawPointer1 = &drawIS3;
					DrawPointer1d = &drawIS3Dead;
					break;
				case 2:
					DrawPointer1 = &drawT29;
					DrawPointer1d = &drawT29Dead;
					break;
				case 3:
					DrawPointer1 = &drawE100;
					DrawPointer1d = &drawE100Dead;
					break;
				case 4:
					DrawPointer1 = &drawType59;
					DrawPointer1d = &drawType59Dead;
					break;
				}
				break;
			case 'd':
			case 'D':
				PlayerOne += TOTALTANKS; // 4 + 5 = 9, 9 % 5 = 4
				PlayerOne = (PlayerOne - 1) % TOTALTANKS;
				switch (PlayerOne)
				{
				case 0:
					DrawPointer1 = &drawAbram;
					DrawPointer1d = &drawAbramDead;
					break;
				case 1:
					DrawPointer1 = &drawIS3;
					DrawPointer1d = &drawIS3Dead;
					break;
				case 2:
					DrawPointer1 = &drawT29;
					DrawPointer1d = &drawT29Dead;
					break;
				case 3:
					DrawPointer1 = &drawE100;
					DrawPointer1d = &drawE100Dead;
					break;
				case 4:
					DrawPointer1 = &drawType59;
					DrawPointer1d = &drawType59Dead;
					break;
				}
				break;
			case 'j':
			case 'J':
			case '4':
				PlayerTwo += 1;
				PlayerTwo = PlayerTwo % TOTALTANKS;
				switch (PlayerTwo)
				{
				case 0:
					DrawPointer2 = &drawAbram;
					DrawPointer2d = &drawAbramDead;
					break;
				case 1:
					DrawPointer2 = &drawIS3;
					DrawPointer2d = &drawIS3Dead;
					break;
				case 2:
					DrawPointer2 = &drawT29;
					DrawPointer2d = &drawT29Dead;
					break;
				case 3:
					DrawPointer2 = &drawE100;
					DrawPointer2d = &drawE100Dead;
					break;
				case 4:
					DrawPointer2 = &drawType59;
					DrawPointer2d = &drawType59Dead;
					break;
				}
				break;
			case 'l':
			case 'L':
			case '6':
				PlayerTwo += TOTALTANKS;
				PlayerTwo = (PlayerTwo - 1) % TOTALTANKS;
				switch (PlayerTwo)
				{
				case 0:
					DrawPointer2 = &drawAbram;
					DrawPointer2d = &drawAbramDead;
					break;
				case 1:
					DrawPointer2 = &drawIS3;
					DrawPointer2d = &drawIS3Dead;
					break;
				case 2:
					DrawPointer2 = &drawT29;
					DrawPointer2d = &drawT29Dead;
					break;
				case 3:
					DrawPointer2 = &drawE100;
					DrawPointer2d = &drawE100Dead;
					break;
				case 4:
					DrawPointer2 = &drawType59;
					DrawPointer2d = &drawType59Dead;
					break;
				}
				break;

			case ' ':
			case 13:
				run = true;
				break;
			case 'q':
			case 'Q':
			case ESCAPE:
				Quit();//DoMainMenu(QUIT);	// will not return here
				break;				// happy compiler

			}

			// force a call to Display( ):

			glutSetWindow(MainWindow);
			glutPostRedisplay();
		}
	}
	else
	{
		keyBuffer[c] = true;
		switch (c)
		{
		case 'c':
		case 'C':
			if (AbramSmoke > 0 && AbramHP > 0) {
				AbramSmoke--;
				struct Smoke tmpSmoke;
				tmpSmoke.smokeIDBuffer = Time;
				tmpSmoke.smokeCoordBuffer[0] = AbramXY[0];
				tmpSmoke.smokeCoordBuffer[1] = AbramXY[1];
				tmpSmoke.smokeDurBuffer = 0.09;
				tmpSmoke.smokeAngleBuffer = rand() % 360;
				tmpSmoke.smokeIDBufferSet = true;
				tmpSmoke.smokeActive = true;
				Smokes.push_back(tmpSmoke);
				
				for (int i = 0; i < 12; i++)
				{
					struct Smoke tmpSmoke;
					tmpSmoke.smokeIDBuffer = Time;
					tmpSmoke.smokeCoordBuffer[0] = AbramXY[0] + 10 * (sin(i *  PI / 6));
					tmpSmoke.smokeCoordBuffer[1] = AbramXY[1] + 10 * (cos(i *  PI / 6));
					tmpSmoke.smokeDurBuffer = 0.09;
					tmpSmoke.smokeAngleBuffer = rand() % 360;
					tmpSmoke.smokeIDBufferSet = true;
					tmpSmoke.smokeActive = true;
					Smokes.push_back(tmpSmoke);
					
				}
			}
			break;
		case '.':
		case '1':
		case 'n':
		case 'N':
			if (IS3Smoke > 0 && IS3HP > 0) {
				IS3Smoke--;
				struct Smoke tmpSmoke;
				tmpSmoke.smokeIDBuffer = Time;
				tmpSmoke.smokeCoordBuffer[0] = IS3XY[0];
				tmpSmoke.smokeCoordBuffer[1] = IS3XY[1];
				tmpSmoke.smokeDurBuffer = 0.09;
				tmpSmoke.smokeAngleBuffer = rand() % 360;
				tmpSmoke.smokeIDBufferSet = true;
				tmpSmoke.smokeActive = true;
				Smokes.push_back(tmpSmoke);
				
				for (int i = 0; i < 12; i++)
				{
					struct Smoke tmpSmoke;
					tmpSmoke.smokeIDBuffer = Time;
					tmpSmoke.smokeCoordBuffer[0] = IS3XY[0] + 10 * (sin(i *  PI / 6));
					tmpSmoke.smokeCoordBuffer[1] = IS3XY[1] + 10 * (cos(i *  PI / 6));
					tmpSmoke.smokeDurBuffer = 0.09;
					tmpSmoke.smokeAngleBuffer = rand() % 360;
					tmpSmoke.smokeIDBufferSet = true;
					tmpSmoke.smokeActive = true;
					Smokes.push_back(tmpSmoke);
					
				}
			}
			break;
		case '=':
		case '+':
			if (TANKSPEED < 1.0)
				TANKSPEED *= 1.5;
			std::cout << "Speed set to:" << TANKSPEED << std::endl;
			break;
		case '-':
		case '_':
			if (TANKSPEED > 0.1)
				TANKSPEED /= 1.5;
			std::cout << "Speed set to:" << TANKSPEED << std::endl;
			break;
		case 'g':
		case 'G':
			mapName = lastMap;
			resetState(mapName);
			break;
		}
	}
}
void keySpecial(int key, int x, int y) {
	if(isInMenu)
		if (ADVANCEMENU)
		{
			switch(menuState)
			{
			case 0:
			{
				switch (key)
				{
				case GLUT_KEY_UP:
					if (selectIndex >= 1)
						selectIndex--;
					else
						selectIndex = 4;
					selectIndex = selectIndex % 5;
					break;
				case GLUT_KEY_DOWN:
					selectIndex++;
					selectIndex = selectIndex % 5;
					break;
				}
			}
				break;
			case 3:
			{
				switch (key)
				{
				case GLUT_KEY_UP:
					if (selectIndex >= 1)
						selectIndex--;
					else
						selectIndex = 9;
					selectIndex = selectIndex % 10;
					switch (selectIndex)
					{
					case 0:
						mapName = '1';
						break;
					case 1:
						mapName = '2';
						break;
					case 2:
						mapName = '7';
						break;
					case 3:
						mapName = '9';
						break;
					case 4:
						mapName = '5';
						break;
					case 5:
						mapName = '8';
						break;
					case 6:
						mapName = '3';
						break;
					case 7:
						mapName = '6';
						break;
					case 8:
						mapName = '4';
						break;
					case 9:
						mapName = 'R';
						break;
					}
					lastMap = mapName;
					resetState(mapName);
					isInMenu = true;
					for (int i = 0; i < 8; i++)
						alSourceStop(Sources[i]);
					break;
				case GLUT_KEY_DOWN:
					selectIndex++;
					selectIndex = selectIndex % 10;
					switch (selectIndex)
					{
					case 0:
						mapName = '1';
						break;
					case 1:
						mapName = '2';
						break;
					case 2:
						mapName = '7';
						break;
					case 3:
						mapName = '9';
						break;
					case 4:
						mapName = '5';
						break;
					case 5:
						mapName = '8';
						break;
					case 6:
						mapName = '3';
						break;
					case 7:
						mapName = '6';
						break;
					case 8:
						mapName = '4';
						break;
					case 9:
						mapName = 'R';
						break;
					}
					lastMap = mapName;
					resetState(mapName);
					isInMenu = true;
					for (int i = 0; i < 8; i++)
						alSourceStop(Sources[i]);
					break;
				}
			}
				break;
			case 5:
			{
				switch (key)
				{
				case GLUT_KEY_UP:
					if (selectIndex >= 1)
						selectIndex--;
					else
						selectIndex = 1;
					selectIndex = selectIndex % 2;
					break;
				case GLUT_KEY_DOWN:
					selectIndex++;
					selectIndex = selectIndex % 2;
					break;
				case GLUT_KEY_LEFT:
					if (selectIndex == 0)
					{
						if (userGrassMultiplier > 0)
							userGrassMultiplier--;
					}
					if (selectIndex == 1)
					{
						if (TANKSPEED > 0.1)
							TANKSPEED /= 1.5;
					}
					if (selectIndex == 2 && false)
					{

					}
					break;
				case GLUT_KEY_RIGHT:
					if (selectIndex == 0)
					{
						if (userGrassMultiplier < 20)
							userGrassMultiplier++;
					}
					if (selectIndex == 1)
					{
						if (TANKSPEED < 1.0)
							TANKSPEED *= 1.5;
					}
					if (selectIndex == 2 && false)
					{

					}
					break;
				}
			}
				break;
			}
		}
		else
		{
			switch (key)
			{
			case GLUT_KEY_UP:
				if (selectIndex >= 1)
					selectIndex--;
				else
					selectIndex = 9;
				selectIndex = selectIndex % 10;
				break;
			case GLUT_KEY_DOWN:
				selectIndex++;
				selectIndex = selectIndex % 10;
				break;
			case GLUT_KEY_LEFT:
			case GLUT_KEY_RIGHT:
				isSingle = !isSingle;
				makeAI(isSingle, 'T');
				break;
			}
		}
}
void keyUp(unsigned char c, int x, int y)
{
	keyBuffer[c] = false;
	if(c >= 'a' && c <= 'z')
		keyBuffer[c-32] = false;
	if (c >= 'A' && c <= 'Z')
		keyBuffer[c+32] = false;

}
void gamepad() {
	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
	{
		int axesCount = 0;
		int buttonCount = 0;
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		const unsigned char *button = glfwGetJoystickButtons(GLFW_JOYSTICK_1,&buttonCount);

		if(GLFW_PRESS == button[1] || GLFW_PRESS == button[2])
			keyBuffer['f'] = true;
		else
			keyBuffer['f'] = false;

		if (GLFW_PRESS == button[3])
			keyBuffer['w'] = true;
		else
			keyBuffer['w'] = false;

		if (GLFW_PRESS == button[0])
			keyBuffer['s'] = true;
		else
			keyBuffer['s'] = false;

		if (GLFW_PRESS == button[5])
			keyBuffer['e'] = true;
		else
			keyBuffer['e'] = false;

		if (GLFW_PRESS == button[4])
			keyBuffer['q'] = true;
		else
			keyBuffer['q'] = false;

		if (GLFW_PRESS == button[17] || axes[0] > 0.5)
			keyBuffer['d'] = true;
		else
			keyBuffer['d'] = false;


		if (GLFW_PRESS == button[19] || axes[0] < -0.5)
			keyBuffer['a'] = true;
		else
			keyBuffer['a'] = false;

	}
	if (glfwJoystickPresent(GLFW_JOYSTICK_2))
	{
		int axesCount = 0;
		int buttonCount = 0;
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_2, &axesCount);
		const unsigned char *button = glfwGetJoystickButtons(GLFW_JOYSTICK_2, &buttonCount);

		if (GLFW_PRESS == button[1] || GLFW_PRESS == button[2])
			keyBuffer['h'] = true;
		else
			keyBuffer['h'] = false;

		if (GLFW_PRESS == button[3])
			keyBuffer['i'] = true;
		else
			keyBuffer['i'] = false;

		if (GLFW_PRESS == button[0])
			keyBuffer['k'] = true;
		else
			keyBuffer['k'] = false;

		if (GLFW_PRESS == button[5])
			keyBuffer['o'] = true;
		else
			keyBuffer['o'] = false;

		if (GLFW_PRESS == button[4])
			keyBuffer['u'] = true;
		else
			keyBuffer['u'] = false;

		if (GLFW_PRESS == button[17] || axes[0] > 0.5)
			keyBuffer['l'] = true;
		else
			keyBuffer['l'] = false;

		if (GLFW_PRESS == button[19] || axes[0] < -0.5)
			keyBuffer['j'] = true;
		else
			keyBuffer['j'] = false;
	}
}
void MouseButton(int button, int state, int x, int y)
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if (DebugOn != 0)
		fprintf(stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y);


	// get the proper button bit mask:

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		b = LEFT;		break;

	case GLUT_MIDDLE_BUTTON:
		b = MIDDLE;		break;

	case GLUT_RIGHT_BUTTON:
		b = RIGHT;		break;

	default:
		b = 0;
		fprintf(stderr, "Unknown mouse button: %d\n", button);
	}


	// button down sets the bit, up clears the bit:

	if (state == GLUT_DOWN)
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}
void MouseMotion(int x, int y)
{
	if (isInMenu)
	{
		if (DebugOn != 0)
			fprintf(stderr, "MouseMotion: %d, %d\n", x, y);


		int dx = x - Xmouse;		// change in mouse coords
		int dy = y - Ymouse;

		if ((ActiveButton & LEFT) != 0)
		{
			Xrot += (ANGFACT*dy);
			Yrot += (ANGFACT*dx);
		}


		if ((ActiveButton & MIDDLE) != 0)
		{
			Scale += SCLFACT * (float)(dx - dy);

			// keep object from turning inside-out or disappearing:

			if (Scale < MINSCALE)
				Scale = MINSCALE;
		}

		Xmouse = x;			// new current position
		Ymouse = y;
	}
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void Reset()
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Scale = 1.0;
	WhichColor = WHITE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
}
void Resize(int width, int height)
{
	if (DebugOn != 0)
		fprintf(stderr, "ReSize: %d, %d\n", width, height);

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void Visibility(int state)
{
	if (DebugOn != 0)
		fprintf(stderr, "Visibility: %d\n", state);

	if (state == GLUT_VISIBLE)
	{
		glutSetWindow(MainWindow);
		glutPostRedisplay();
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}
void Axes(float length)
{
	glBegin(GL_LINE_STRIP);
	glVertex3f(length, 0., 0.);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., length, 0.);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., 0., length);
	glEnd();

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 4; i++)
	{
		int j = xorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(base + fact*xx[j], fact*xy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 5; i++)
	{
		int j = yorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(fact*yx[j], base + fact*yy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 6; i++)
	{
		int j = zorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(0.0, fact*zy[j], base + fact*zx[j]);
	}
	glEnd();

}
