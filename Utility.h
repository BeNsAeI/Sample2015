#ifndef UTILITY_H
#define UTILITY_H
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
#include <omp.h>
#include "glew.h"
//#include <WS2tcpip.h>
//#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)
#endif

#ifndef WIN32
#include <GL/glew.h>
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
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

void Quit()
{
	glfwTerminate();
	glutSetWindow(MainWindow);
	glFinish();
	glutDestroyWindow(MainWindow);
	exit(0);
}
void Animate()
{
	int ms = glutGet(GLUT_ELAPSED_TIME);
	ms %= TOTAL_MS;
	Time = (float)ms / 180000;
	if (Time == 0)
	{
		std::cout << "Time restarted" << std::endl;
		shakeStartTime = 0;
		//smokeBeginTime = 0;
		AbramLastShot = 0;
		IS3LastShot = 0;
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
bool isBigEndian()
{
	int a = 1;
	return !((char*)& a)[0];
}
int convertToInt(char* buffer, int len)
{
	int a = 0;
	if (!isBigEndian())
		for (int i = 0; i < len; i++)
			((char*)& a)[i] = buffer[i];
	else
		for (int i = 0; i < len; i++)
			((char*)& a)[3 - i] = buffer[i];
	return a;
}
char* loadWAV(const char* fn, int& chan, int& freq, int& bps, int& size, int& form)
{
	char buffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(buffer, 4);
	if (strncmp(buffer, "RIFF", 4) != 0)
	{
		std::cout << "this is not a valid WAVE file" << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	in.read(buffer, 4);      //WAVE
	in.read(buffer, 4);      //fmt
	in.read(buffer, 4);      //16
	in.read(buffer, 2);      //1
	in.read(buffer, 2);
	chan = convertToInt(buffer, 2);
	in.read(buffer, 4);
	freq = convertToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = convertToInt(buffer, 2);
	in.read(buffer, 4);
	in.read(buffer, 4);
	size = convertToInt(buffer, 4);
	char* data = new char[size];
	in.read(data, size);
	if (chan == 1)
	{
		if (bps == 8)
		{
			form = AL_FORMAT_MONO8;
		}
		else {
			form = AL_FORMAT_MONO16;
		}
	}
	else {
		if (bps == 8)
		{
			form = AL_FORMAT_STEREO8;
		}
		else {
			form = AL_FORMAT_STEREO16;
		}
	}
	return data;
}
char getIP()
{
	return NULL;
}
void connect(int* ip)
{

}
void startServer()
{
	// init winsock

}
void killServer()
{
	//shutdown winsock
}
void listen()
{

}
void sendKey()
{

}
char getKey()
{
	return NULL;
}
void loadMap()
{
	Reset();
	for (int i = 0; i < 8; i++)
		alSourceStop(Sources[i]);

	for (std::vector<Crate>::iterator it = Crates.begin(); it != Crates.end(); ++it)
	{
		it->isActive = false;
		myMap.isCrate[it->i][it->j] = false;
	}
	while (!Crates.empty())
		Crates.pop_back();

	for (int j = 0; j < 14; j++)
	{
		for (int i = 0; i < 24; i++)
		{
			myMap.color[i][j][0] = 0;
			myMap.color[i][j][1] = 0;
			myMap.color[i][j][2] = 0;
			myMap.coord[i][j][0] = 0;
			myMap.coord[i][j][1] = 0;
			myMap.coord[i][j][2] = 0;
			myMap.MCM[i][j] = false;
			myMap.isSolid[i][j] = false;
			myMap.isCrate[i][j] = false;
			destructionTimeBuffer[i][j] = -1;
		}
	}
	std::cout << "Enter map directory: " << std::endl;
	if (mapName == " ")
		std::cin >> mapName;
	std::string folder = "Maps/";
	std::string ext = ".txt";
	switch (mapName[0])
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		mapName = folder + mapName[0] + ext;
		break;
	case'r':
	case'R':
		mapName = "RANDOM";
		break;
	case'm':
	case'M':
		mapName = folder + "Main" + ext;
		break;
	}
	std::cout << "Loading Map ..." << std::endl;
	if (mapName == "RANDOM")
	{
		int AX = 0;
		int AY = 0;
		int TX = 0;
		int TY = 0;

		while (AX == TX)
		{
			AX = rand() % 24;
			AY = rand() % 14;
			TX = rand() % 24;
			TY = rand() % 14;
		}

		for (int j = 0; j < 14; j++)
		{
			for (int i = 0; i < 24; i++)
			{
				char e = '_';
				if (i == AX && j == AY)
				{
					e = 'A';
				}
				if (i == TX && j == TY)
				{
					e = 'T';
				}
				if ((!(i == AX && j == AY)) && (!(i == TX && j == TY)))
				{
					int element = rand() % 16;
					switch (element)
					{
					case 0:
						e = '#';
						break;
					case 10:
					case 5:
					case 1:
						e = '$';
						break;
					}
				}
				MapRaw[j * 24 + i] = e;
			}
		}
	}
	else
	{
		std::ifstream mapFile(mapName.c_str());
		char tmp;
		int k = 0;
		while (mapFile.get(tmp))
		{
			MapRaw[k] = tmp;
			k++;
		}
		mapFile.close();
	}
	int rockCount = 0;
	int rowRockCount = 0;
	for (int j = 0; j < 14; j++)
	{
		rowRockCount = 0;
		for (int i = 0; i < 24; i++)
		{
			std::cout << MapRaw[j * 25 + i];
			if (MapRaw[j * 25 + i] == 'A')
			{
				AbramInitCoord[0] = AbramXY[0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
				AbramInitCoord[1] = AbramXY[1] = -MAPEDGEY + i * CUBESIZE;
				if (i <= 12)
					AbramHullAngle = 180;
				else
					AbramHullAngle = 0;
			}
			if (MapRaw[j * 25 + i] == 'T')
			{
				IS3InitCoord[0] = IS3XY[0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
				IS3InitCoord[1] = IS3XY[1] = -MAPEDGEY + i * CUBESIZE;
				if (i <= 12)
					IS3HullAngle = 180;
				else
					IS3HullAngle = 0;
			}
			if (MapRaw[j * 25 + i] == '#')
			{
				myMap.color[i][j][0] = 0.35;
				myMap.color[i][j][1] = 0.25;
				myMap.color[i][j][2] = 0.18;
				myMap.coord[i][j][0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
				myMap.coord[i][j][1] = -MAPEDGEY + i * CUBESIZE;
				myMap.HP[i][j] = BRICKHP;
				myMap.MCM[i][j] = true;
				myMap.isSolid[i][j] = true;
			}
			if (MapRaw[j * 25 + i] == '+')
			{
				myMap.color[i][j][0] = 7;
				myMap.color[i][j][1] = 0.5;
				myMap.color[i][j][2] = 0;
				myMap.coord[i][j][0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
				myMap.coord[i][j][1] = -MAPEDGEY + i * CUBESIZE;
				myMap.MCM[i][j] = true;
				myMap.isSolid[i][j] = true;
			}
			if (MapRaw[j * 25 + i] == '$')
			{
				if (rockCount < ROCKTHRESH)
				{
					int selector = rand() % (8 - rowRockCount);
					if (selector == 7)
					{
						if (rand() % 2 == 0)
							selector = rand() % 7;
					}
					myMap.color[i][j][0] = selector;
					myMap.color[i][j][1] = 0.5;
					myMap.color[i][j][2] = 0;
					myMap.coord[i][j][0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
					myMap.coord[i][j][1] = -MAPEDGEY + i * CUBESIZE;
					myMap.angle[i][j] = rand() % 360;
					myMap.MCM[i][j] = true;
					myMap.isSolid[i][j] = myMap.color[i][j][0] == 7;
					if (myMap.color[i][j][0] == 7)
					{
						rockCount++;
						rowRockCount++;
					}
				}
				else
				{
					myMap.color[i][j][0] = rand() % 7;
					myMap.color[i][j][1] = 0.5;
					myMap.color[i][j][2] = 0;
					myMap.coord[i][j][0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
					myMap.coord[i][j][1] = -MAPEDGEY + i * CUBESIZE;
					myMap.MCM[i][j] = true;
					myMap.isSolid[i][j] = myMap.color[i][j][0] == 7;
				}
			}
			if (MapRaw[j * 25 + i] == 'B')
			{
				myMap.color[i][j][0] = 0.50;
				myMap.color[i][j][1] = 0.50;
				myMap.color[i][j][2] = 0.00;
				myMap.coord[i][j][0] = REFLECT*(-MAPEDGEX - CUBESIZE / 4 + j * CUBESIZE + 2);
				myMap.coord[i][j][1] = -MAPEDGEY + i * CUBESIZE;
				myMap.MCM[i][j] = true;
				myMap.isSolid[i][j] = true;
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (int j = 0; j < 14; j++)
	{
		for (int i = 0; i < 24; i++)
		{
			std::cout << myMap.color[i][j][0] << ", ";
			std::cout << myMap.color[i][j][1] << ", ";
			std::cout << myMap.color[i][j][2] << ", ";
			std::cout << myMap.coord[i][j][0] << ", ";
			std::cout << myMap.coord[i][j][1] << ", ";
			std::cout << myMap.MCM[i][j];
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	Smokes.erase(Smokes.begin(), Smokes.end());
	ScoreSet = false;
	musicID = (musicID + rand()) % 14;
	switch (musicID)
	{
	case 0:
	case 1:
		alSourcePlay(Sources[0]);
		break;
	case 2:
	case 3:
		alSourcePlay(Sources[1]);
		break;
	case 4:
		alSourcePlay(Sources[2]);
		break;
	case 5:
	case 6:
	case 7:
		alSourcePlay(Sources[3]);
		break;
	case 8:
	case 9:
		alSourcePlay(Sources[4]);
		break;
	case 10:
		alSourcePlay(Sources[5]);
		break;
	case 11:
	case 12:
		alSourcePlay(Sources[6]);
		break;
	case 13:
		alSourcePlay(Sources[7]);
		break;
	default:
		break;
	}
}
void loadingText(char * Text, float percent)
{
	glutSetCursor(GLUT_CURSOR_NONE);
	// set which window we want to do the graphics into:

	glutSetWindow(MainWindow);


	// erase the background:

	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (DepthBufferOn != 0)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);


	// specify shading to be flat:

	glShadeModel(GL_FLAT);


	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet(GLUT_WINDOW_WIDTH);
	GLsizei vy = glutGet(GLUT_WINDOW_HEIGHT);
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	v = vx > vy ? vx : vy;
	GLint xl = (vx - v) / 2;
	GLint yb = (vy - v) / 2;
	glViewport(xl, yb, v, v);

	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (WhichProjection == ORTHO)
		glOrtho(-3., 3., -3., 3., 0.1, 1000.);
	else
		gluPerspective(90., 1., 0.1, 1000.);


	// place the objects into the scene:

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// set the eye position, look-at position, and up-vector:
	gluLookAt(eyex, eyey, eyez, targetx, targety, targetz, upx, upy, upz);

	// rotate the scene:

	glRotatef((GLfloat)Yrot, 0., 1., 0.);
	glRotatef((GLfloat)Xrot, 1., 0., 0.);


	// uniformly scale the scene:

	if (Scale < MINSCALE)
		Scale = MINSCALE;
	glScalef((GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale);


	// set the fog parameters:

	if (DepthCueOn != 0)
	{
		glFogi(GL_FOG_MODE, FOGMODE);
		glFogfv(GL_FOG_COLOR, FOGCOLOR);
		glFogf(GL_FOG_DENSITY, FOGDENSITY);
		glFogf(GL_FOG_START, FOGSTART);
		glFogf(GL_FOG_END, FOGEND);
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}


	// possibly draw the axes:

	if (AxesOn == 0)
	{
		glColor3fv(&Colors[WhichColor][0]);
		glCallList(AxesList);
	}
	// -> put a text here
	if (DepthFightingOn != 0)
	{
		glPushMatrix();
		glRotatef(90., 0., 1., 0.);
		glCallList(BoxList);
		glPopMatrix();
	}

	// draw some gratuitous text that just rotates on top of the scene:

	glDisable(GL_DEPTH_TEST);

	glLineWidth(10);
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 1, 1);
	glVertex3f(0, 0, -50.5);
	glVertex3f(0, 0, 50.5);
	glEnd();

	glLineWidth(8);
	glBegin(GL_LINE_STRIP);
	glColor3f(1 - percent / 100, percent / 100, 0);
	glVertex3f(0, 0, -50);
	glVertex3f(0, 0, -50 + percent);
	glEnd();

	glColor3f(1, 1, 1);
	DoRasterString(0, 10, -20, Text);
	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//
	// this is called "percent units", and is just a convenience
	//
	// the modelview matrix is reset to identity as we don't
	// want to transform these coordinates

	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., 100., 0., 100.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1., 1., 1.);
	//DoRasterString(500., 400, 0., (char *)"Final Project: Tank 2017");


	// swap the double-buffered framebuffers:

	glutSwapBuffers();


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush();
}
void loadAll()
{
	// Item goes up to 33 -> percent is ~ 3.1
	float item = 0;
	float percent = 3.0;
	loadingText("Loading Abrams Turret ...", percent * item); 
	item++;
	Abram[0][START] = vertices.size();
	res = loadOBJ("models/abram-turret.vbo", vertices, uvs, normals);
	Abram[0][END] = vertices.size();

	loadingText("Loading Abrams Hull ...", percent * item); 
	item++;
	Abram[1][START] = vertices.size();
	res = loadOBJ("models/abram-hull.vbo", vertices, uvs, normals);
	Abram[1][END] = vertices.size();

	loadingText("Loading T29 Turret ...", percent * item); 
	item++;
	T29[0][START] = vertices.size();
	res = loadOBJ("models/T-29-turret.vbo", vertices, uvs, normals);
	T29[0][END] = vertices.size();

	loadingText("Loading T29 Hull ...", percent * item); 
	item++;
	T29[1][START] = vertices.size();
	res = loadOBJ("models/T-29-hull.vbo", vertices, uvs, normals);
	T29[1][END] = vertices.size();

	loadingText("Loading IS3 Turret ...", percent * item); 
	item++;
	IS3[0][START] = vertices.size();
	res = loadOBJ("models/IS-3-turret.vbo", vertices, uvs, normals);
	IS3[0][END] = vertices.size();

	loadingText("Loading IS3 Hull ...", percent * item); 
	item++;
	IS3[1][START] = vertices.size();
	res = loadOBJ("models/IS-3-upper-hull.vbo", vertices, uvs, normals);
	IS3[1][END] = vertices.size();

	loadingText("Loading IS3 Hull ...", percent * item); 
	item++;
	IS3[2][START] = vertices.size();
	res = loadOBJ("models/IS-3-lower-hull.vbo", vertices, uvs, normals);
	IS3[2][END] = vertices.size();

	loadingText("Loading E100 Turret ...", percent * item); 
	item++;
	E100[0][START] = vertices.size();
	res = loadOBJ("models/E100-turret.vbo", vertices, uvs, normals);
	E100[0][END] = vertices.size();

	loadingText("Loading E100 Hull ...", percent * item); 
	item++;
	E100[1][START] = vertices.size();
	res = loadOBJ("models/E100-hull.vbo", vertices, uvs, normals);
	E100[1][END] = vertices.size();

	loadingText("Loading Type 59 Turret ...", percent * item); 
	item++;
	Type59[0][START] = vertices.size();
	res = loadOBJ("models/Type59-turret.vbo", vertices, uvs, normals);
	Type59[0][END] = vertices.size();

	loadingText("Loading Type 59 Hull ...", percent * item); 
	item++;
	Type59[1][START] = vertices.size();
	res = loadOBJ("models/Type59-hull.vbo", vertices, uvs, normals);
	Type59[1][END] = vertices.size();

	loadingText("Loading Left Track ...", percent * item); 
	item++;
	Track[0][START] = vertices.size();
	res = loadOBJ("models/l-track.vbo", vertices, uvs, normals);
	Track[0][END] = vertices.size();

	loadingText("Loading Right Track ...", percent * item); 
	item++;
	Track[1][START] = vertices.size();
	res = loadOBJ("models/r-track.vbo", vertices, uvs, normals);
	Track[1][END] = vertices.size();

	loadingText("Loading Walls ...", percent * item); 
	item++;
	cube[START] = vertices.size();
	res = loadOBJ("models/cube.vbo", vertices, uvs, normals);
	cube[END] = vertices.size();

	loadingText("Loading Trees ...", percent * item); 
	item++;
	trees[0][START] = vertices.size();
	res = loadOBJ("models/tree1.vbo", vertices, uvs, normals);
	trees[0][END] = vertices.size();
	loadingText("Loading Trees ...", percent * item); 
	item++;
	trees[1][START] = vertices.size();
	res = loadOBJ("models/tree2.vbo", vertices, uvs, normals);
	trees[1][END] = vertices.size();
	loadingText("Loading Trees ...", percent * item); 
	item++;
	trees[2][START] = vertices.size();
	res = loadOBJ("models/tree3.vbo", vertices, uvs, normals);
	trees[2][END] = vertices.size();
	loadingText("Loading Trees ...", percent * item); 
	item++;
	trees[3][START] = vertices.size();
	res = loadOBJ("models/tree4.vbo", vertices, uvs, normals);
	trees[3][END] = vertices.size();
	loadingText("Loading Trees ...", percent * item); 
	item++;
	trees[4][START] = vertices.size();
	res = loadOBJ("models/tree5.vbo", vertices, uvs, normals);
	trees[4][END] = vertices.size();
	loadingText("Loading Trees ...", percent * item); 
	item++;
	trees[5][START] = vertices.size();
	res = loadOBJ("models/tree6.vbo", vertices, uvs, normals);
	trees[5][END] = vertices.size();
	loadingText("Loading Trees ...", percent * item); 
	item++;
	trees[6][START] = vertices.size();
	res = loadOBJ("models/tree7.vbo", vertices, uvs, normals);
	trees[6][END] = vertices.size();
	loadingText("Loading Particles ...", percent * item); 
	item++;
	trees[7][START] = vertices.size();
	res = loadOBJ("models/rock.vbo", vertices, uvs, normals);
	trees[7][END] = vertices.size();

	loadingText("Loading Power-Ups ...", percent * item); 
	item++;
	hpCrate[0][START] = vertices.size();
	res = loadOBJ("models/hp1.vbo", vertices, uvs, normals);
	hpCrate[0][END] = vertices.size();
	loadingText("Loading Power-Ups ...", percent * item); 
	item++;
	hpCrate[1][START] = vertices.size();
	res = loadOBJ("models/hp2.vbo", vertices, uvs, normals);
	hpCrate[1][END] = vertices.size();
	loadingText("Loading Ammunition ...", percent * item); 
	item++;
	shell[START] = vertices.size();
	res = loadOBJ("models/shell.vbo", vertices, uvs, normals);
	shell[END] = vertices.size();

	loadingText("Loading Counter Measures ...", percent * item); 
	item++;
	smokeCrate[0][START] = vertices.size();
	res = loadOBJ("models/smoke1.vbo", vertices, uvs, normals);
	smokeCrate[0][END] = vertices.size();
	loadingText("Loading Counter Measures ...", percent * item); 
	item++;
	smokeCrate[1][START] = vertices.size();
	res = loadOBJ("models/smoke2.vbo", vertices, uvs, normals);
	smokeCrate[1][END] = vertices.size();
	loadingText("Loading Explosives ...", percent * item); 
	item++;
	ammo[START] = vertices.size();
	res = loadOBJ("models/ammo.vbo", vertices, uvs, normals);
	ammo[END] = vertices.size();

	loadingText("Loading Dima's present ...", percent * item); 
	item++;
	mineCrate[START] = vertices.size();
	res = loadOBJ("models/mine.vbo", vertices, uvs, normals);
	mineCrate[END] = vertices.size();

	loadingText("Loading the Radio ...", percent* item); 
	item++;
	radioCrate[0][START] = vertices.size();
	res = loadOBJ("models/radio-main.vbo", vertices, uvs, normals);
	radioCrate[0][END] = vertices.size();
	radioCrate[1][START] = vertices.size();
	res = loadOBJ("models/radio-black.vbo", vertices, uvs, normals);
	radioCrate[1][END] = vertices.size();

	loadingText("Loading the Laptop ...", percent* item);
	item++;
	laptop[0][START] = vertices.size();
	res = loadOBJ("models/laptop-main.vbo", vertices, uvs, normals);
	laptop[0][END] = vertices.size();
	laptop[1][START] = vertices.size();
	res = loadOBJ("models/laptop-black.vbo", vertices, uvs, normals);
	laptop[1][END] = vertices.size();

	loadingText("Loading the A-10 ...", percent* item); 
	item++;
	A_10_M[0][START] = vertices.size();
	res = loadOBJ("models/A-10-main.vbo", vertices, uvs, normals);
	A_10_M[0][END] = vertices.size();
	A_10_M[1][START] = vertices.size();
	res = loadOBJ("models/A-10-window.vbo", vertices, uvs, normals);
	A_10_M[1][END] = vertices.size();
	A_10_M[2][START] = vertices.size();
	res = loadOBJ("models/A-10-black.vbo", vertices, uvs, normals);
	A_10_M[2][END] = vertices.size();

	loadingText("Loading Grass ...", percent * item); 
	item++;
	grass[START] = vertices.size();
	res = loadOBJ("models/grass.vbo", vertices, uvs, normals);
	grass[END] = vertices.size();

	loadingText("Loading Finished ...", 100); 

	if (res)
	{
		glGenBuffers(1, &VertexVBOID);
		glBindBuffer(GL_ARRAY_BUFFER, VertexVBOID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	}
}
void resetState(std::string newMap) {
	for (int i = 0; i < NUM_SOURCES; i++)
	{
		alSourceStop(i);
	}
	AbramHP = TANKHP;
	IS3HP = TANKHP;
	shake = false;
	//smokeIndex = 0;
	shakeOnce = false;
	AbramShells = SHELLSTORAGE;
	IS3Shells = SHELLSTORAGE;
	AbramSmoke = SMOKECOUNT;
	IS3Smoke = SMOKECOUNT;
	AbramTurretAngle = 0;
	IS3TurretAngle = 0;
	AbramLastShot = 0;
	IS3LastShot = 0;
	playerOneSmokeGrace = false;
	playerOneSmokeGraceStart = 0;
	playerTwoSmokeGrace = false;
	playerTwoSmokeGraceStart = 0;
	aTenActive = false;
	aTenTimer = false;
	aTenTarget = 0;
	aTenStart = 0;
	drawATenActive = false;
	aTenTargetCoord = 0;
	Smokes.erase(Smokes.begin(), Smokes.end());
	mapName = newMap;
	loadMap();
}
void resetState() {
	for (int i = 0; i < NUM_SOURCES; i++)
	{
		alSourceStop(i);
	}
	AbramHP = TANKHP;
	IS3HP = TANKHP;
	shake = false;
	//smokeIndex = 0;
	shakeOnce = false;
	AbramShells = SHELLSTORAGE;
	IS3Shells = SHELLSTORAGE;
	AbramSmoke = SMOKECOUNT;
	IS3Smoke = SMOKECOUNT;
	AbramTurretAngle = 0;
	IS3TurretAngle = 0;
	AbramLastShot = 0;
	IS3LastShot = 0;
	playerOneSmokeGrace = false;
	playerOneSmokeGraceStart = 0;
	playerTwoSmokeGrace = false;
	playerTwoSmokeGraceStart = 0;
	aTenActive = false;
	aTenTimer = false;
	aTenTarget = 0;
	aTenStart = 0;
	drawATenActive = false;
	aTenTargetCoord = 0;
	Smokes.erase(Smokes.begin(), Smokes.end());
}
bool MapCollisionModel(float AX, float AY, float Xstride, float Ystride, int sign, int axis, int * ival = NULL, int *jval = NULL)
{
	switch (axis)
	{
	case 0:

		for (int j = 0; j < 14; j++)
		{
			for (int i = 0; i < 24; i++)
			{
				if (myMap.isSolid[i][j] && myMap.color[i][j][0] != 7)
				{
					if ((((AX + (Xstride * sign) < myMap.coord[i][j][0] + BODY) && (AX + (Xstride * sign) > myMap.coord[i][j][0] - BODY)) &&
						((AY + (Ystride * sign) < myMap.coord[i][j][1] + BODY) && (AY + (Ystride * sign) > myMap.coord[i][j][1] - BODY))))
					{
						if (ival != NULL && jval != NULL)
						{
							*ival = i;
							*jval = j;
						}
						return true;
					}
				}
				if (myMap.isSolid[i][j] && myMap.color[i][j][0] == 7)
				{
					if ((((AX + (Xstride * sign) < myMap.coord[i][j][0] + BODY / 2) && (AX + (Xstride * sign) > myMap.coord[i][j][0] - BODY / 2)) &&
						((AY + (Ystride * sign) < myMap.coord[i][j][1] + BODY / 2) && (AY + (Ystride * sign) > myMap.coord[i][j][1] - BODY / 2))))
						return true;
				}
			}
		}
		return false;
	case 1:
		for (int j = 0; j < 14; j++)
		{
			for (int i = 0; i < 24; i++)
			{
				if (myMap.isSolid[i][j] && myMap.color[i][j][0] != 7)
				{
					if (AX + (Xstride * sign) < myMap.coord[i][j][0] + BODY &&
						AX + (Xstride * sign) > myMap.coord[i][j][0] - BODY &&
						AY + (0 * sign) < myMap.coord[i][j][1] + BODY &&
						AY + (0 * sign) > myMap.coord[i][j][1] - BODY)
					{
						return true;
					}
				}
				if (myMap.isSolid[i][j] && myMap.color[i][j][0] == 7)
				{
					if (AX + (Xstride * sign) < myMap.coord[i][j][0] + BODY / 1.5 &&
						AX + (Xstride * sign) > myMap.coord[i][j][0] - BODY / 1.5 &&
						AY + (Ystride * sign) < myMap.coord[i][j][1] + BODY / 1.5 &&
						AY + (Ystride * sign) > myMap.coord[i][j][1] - BODY / 1.5)
						return true;
				}
			}
		}
		return false;
	case 2:
		for (int j = 0; j < 14; j++)
		{
			for (int i = 0; i < 24; i++)
			{
				if (myMap.isSolid[i][j] && myMap.color[i][j][0] != 7)
				{
					if (AX + (0 * sign) < myMap.coord[i][j][0] + BODY &&
						AX + (0 * sign) > myMap.coord[i][j][0] - BODY &&
						AY + (Ystride * sign) < myMap.coord[i][j][1] + BODY &&
						AY + (Ystride * sign) > myMap.coord[i][j][1] - BODY)
					{
						return true;
					}
				}
				if (myMap.isSolid[i][j] && myMap.color[i][j][0] == 7)
				{
					if (AX + (0 * sign) < myMap.coord[i][j][0] + BODY / 1.5 &&
						AX + (0 * sign) > myMap.coord[i][j][0] - BODY / 1.5 &&
						AY + (Ystride * sign) < myMap.coord[i][j][1] + BODY / 1.5 &&
						AY + (Ystride * sign) > myMap.coord[i][j][1] - BODY / 1.5)
						return true;
				}
			}
		}
		return false;
	default:
		return false;
		break;
	}
}
std::vector<Crate>::iterator CrateCollisionModel(float AX, float AY, float Xstride, float Ystride, int sign)
{
	for (std::vector<Crate>::iterator it = Crates.begin(); it != Crates.end(); ++it)
	{
		if (it->isActive)
		{
			if ((((AX + (Xstride * sign) < it->X + BODY) && (AX + (Xstride * sign) > it->X - BODY)) &&
				((AY + (Ystride * sign) <it->Y + BODY) && (AY + (Ystride * sign) > it->Y - BODY))))
			{
				return it;
			}
		}
	}
	return Crates.end();
}
void aTen(char player)
{
	aTenActive = true;
	if (player == '1')
		aTenTarget = 2;
	else
		aTenTarget = 1;
}
void KeyHandler() {
	float AbramTX = TANKSPEED * (float)sin(AbramHullAngle * (float)((float)PI / (float)180));
	float AbramTY = TANKSPEED * (float)cos(AbramHullAngle * (float)((float)PI / (float)180));
	float IS3TX = TANKSPEED * (float)sin(IS3HullAngle * (float)((float)PI / (float)180));
	float IS3TY = TANKSPEED * (float)cos(IS3HullAngle * (float)((float)PI / (float)180));
	if ((keyBuffer['w'] || keyBuffer['W']) && AbramHP > 0) {
		if (((AbramXY[0] - AbramTX) < MAPEDGEX && (AbramXY[0] - AbramTX) > -MAPEDGEX) &&
			(!MapCollisionModel(AbramXY[0], AbramXY[1], AbramTX, AbramTY, (-1), 1)) &&
			!(((AbramXY[0] - AbramTX < IS3XY[0] + BODY) && (AbramXY[0] - AbramTX > IS3XY[0] - BODY)) &&
			((AbramXY[1] - AbramTY < IS3XY[1] + BODY) && (AbramXY[1] - AbramTY > IS3XY[1] - BODY))))
		{
			AbramXY[0] -= AbramTX;
		}
		else
		{
			//adjust angle
			if (AbramTX > 0)
			{
				//hit a wall horizontal wall from top going down
				if (AbramTY > 0)
					AbramHullAngle -= TANKSPEED * 5 * 2;
				else
					AbramHullAngle += TANKSPEED * 5 * 2;

			}
			else
			{
				//hit a wall horizontal wall from bottom going up
				if (AbramTY > 0)
					AbramHullAngle += TANKSPEED * 5 * 2;
				else
					AbramHullAngle -= TANKSPEED * 5 * 2;
			}
		}
		if (((AbramXY[1] - AbramTY) < MAPEDGEY && (AbramXY[1] - AbramTY) > -MAPEDGEY) &&
			(!MapCollisionModel(AbramXY[0], AbramXY[1], AbramTX, AbramTY, (-1), 2)) &&
			!(((AbramXY[0] - AbramTX < IS3XY[0] + BODY) && (AbramXY[0] - AbramTX > IS3XY[0] - BODY)) &&
			((AbramXY[1] - AbramTY < IS3XY[1] + BODY) && (AbramXY[1] - AbramTY > IS3XY[1] - BODY))))
		{
			AbramXY[1] -= AbramTY;
		}
		else
		{
			//adjust angle
			if (AbramTY > 0)
			{
				//hit a wall Vertical wall from right going left
				if (AbramTX > 0)
					AbramHullAngle += TANKSPEED * 5 * 2;
				else
					AbramHullAngle -= TANKSPEED * 5 * 2;

			}
			else
			{
				//hit a wall Vertical wall from left going right
				if (AbramTX > 0)
					AbramHullAngle -= TANKSPEED * 5 * 2;
				else
					AbramHullAngle += TANKSPEED * 5 * 2;
			}

		}
		if (AbramSmokeBudget == VEHICLESMOKEMOD)
		{
			struct Smoke tmpSmoke;
			tmpSmoke.smokeIDBuffer = Time;
			tmpSmoke.smokeCoordBuffer[0] = AbramXY[0];
			tmpSmoke.smokeCoordBuffer[1] = AbramXY[1];
			tmpSmoke.smokeDurBuffer = 0.01;
			tmpSmoke.smokeAngleBuffer = rand() % 360;
			tmpSmoke.smokeIDBufferSet = true;
			tmpSmoke.smokeActive = true;
			Smokes.push_back(tmpSmoke);
			AbramSmokeBudget = 0;
		}
		else
		{
			AbramSmokeBudget++;
		}
		std::vector<Crate>::iterator cratecheck = CrateCollisionModel(AbramXY[0], AbramXY[1],
			AbramTX,
			AbramTY,
			-1
		);
		if (cratecheck != Crates.end())
		{
			switch (cratecheck->type)
			{
			case 0:
				AbramShells = SHELLSTORAGE;
				alSourcePlay(Sources[12]);
				break;
			case 1:
				AbramSmoke = SMOKECOUNT;
				alSourcePlay(Sources[12]);
				break;
			case 2:
				AbramHP = TANKHP;
				alSourcePlay(Sources[11]);
				break;
			case 3:
				AbramHP -= TANKHP;
				alSourcePlay(Sources[8]);
				break;
			case 4:
				alSourcePlay(Sources[14]);
				aTen('1');
				break;
			}
			//Crates[cratecheck].isActive = false;
			cratecheck->isActive = false;
			//myMap.isCrate[Crates[cratecheck].i][Crates[cratecheck].j] = false;
			myMap.isCrate[cratecheck->i][cratecheck->j] = false;
			Crates.erase(cratecheck);
		}
	}
	if ((keyBuffer['s'] || keyBuffer['S']) && AbramHP > 0) {
		if (((AbramXY[0] + AbramTX) < MAPEDGEX && (AbramXY[0] + AbramTX) > -MAPEDGEX) &&
			(!MapCollisionModel(AbramXY[0], AbramXY[1], AbramTX, AbramTY, (1), 1)) &&
			!(((AbramXY[0] + AbramTX < IS3XY[0] + BODY) && (AbramXY[0] + AbramTX > IS3XY[0] - BODY)) &&
			((AbramXY[1] + AbramTY < IS3XY[1] + BODY) && (AbramXY[1] + AbramTY > IS3XY[1] - BODY))))
		{
			AbramXY[0] += AbramTX;
		}
		else
		{
			//adjust angle
			if (AbramTX > 0)
			{
				//hit a wall horizontal wall from top going down
				if (AbramTY > 0)
					AbramHullAngle -= TANKSPEED * 5 * 2;
				else
					AbramHullAngle += TANKSPEED * 5 * 2;

			}
			else
			{
				//hit a wall horizontal wall from bottom going up
				if (AbramTY > 0)
					AbramHullAngle += TANKSPEED * 5 * 2;
				else
					AbramHullAngle -= TANKSPEED * 5 * 2;
			}
		}
		if (((AbramXY[1] + AbramTY) < MAPEDGEY && (AbramXY[1] + AbramTY) > -MAPEDGEY) &&
			(!MapCollisionModel(AbramXY[0], AbramXY[1], AbramTX, AbramTY, (1), 2)) &&
			!(((AbramXY[0] + AbramTX < IS3XY[0] + BODY) && (AbramXY[0] + AbramTX > IS3XY[0] - BODY)) &&
			((AbramXY[1] + AbramTY < IS3XY[1] + BODY) && (AbramXY[1] + AbramTY > IS3XY[1] - BODY))))
		{
			AbramXY[1] += AbramTY;
		}
		else
		{
			//adjust angle
			if (AbramTY > 0)
			{
				//hit a wall Vertical wall from right going left
				if (AbramTX > 0)
					AbramHullAngle += TANKSPEED * 5 * 2;
				else
					AbramHullAngle -= TANKSPEED * 5 * 2;

			}
			else
			{
				//hit a wall Vertical wall from left going right
				if (AbramTX > 0)
					AbramHullAngle -= TANKSPEED * 5 * 2;
				else
					AbramHullAngle += TANKSPEED * 5 * 2;
			}
		}

		if (AbramSmokeBudget == VEHICLESMOKEMOD)
		{
			struct Smoke tmpSmoke;
			tmpSmoke.smokeIDBuffer = Time;
			tmpSmoke.smokeCoordBuffer[0] = AbramXY[0];
			tmpSmoke.smokeCoordBuffer[1] = AbramXY[1];
			tmpSmoke.smokeDurBuffer = 0.01;
			tmpSmoke.smokeAngleBuffer = rand() % 360;
			tmpSmoke.smokeIDBufferSet = true;
			tmpSmoke.smokeActive = true;
			Smokes.push_back(tmpSmoke);
			AbramSmokeBudget = 0;
		}
		else
		{
			AbramSmokeBudget++;
		}

		std::vector<Crate>::iterator cratecheck = CrateCollisionModel(AbramXY[0], AbramXY[1],
			AbramTX,
			AbramTY,
			1
		);
		if (cratecheck != Crates.end())
		{
			switch (cratecheck->type)
			{
			case 0:
				AbramShells = SHELLSTORAGE;
				alSourcePlay(Sources[12]);
				break;
			case 1:
				AbramSmoke = SMOKECOUNT;
				alSourcePlay(Sources[12]);
				break;
			case 2:
				AbramHP = TANKHP;
				alSourcePlay(Sources[11]);
				break;
			case 3:
				AbramHP -= TANKHP;
				alSourcePlay(Sources[8]);
				break;
			case 4:
				alSourcePlay(Sources[14]);
				aTen('1');
				break;
			}
			//Crates[cratecheck].isActive = false;
			cratecheck->isActive = false;
			//myMap.isCrate[Crates[cratecheck].i][Crates[cratecheck].j] = false;
			myMap.isCrate[cratecheck->i][cratecheck->j] = false;
			Crates.erase(cratecheck);
		}
	}
	if ((keyBuffer['a'] || keyBuffer['A']) && AbramHP > 0) {
		if (keyBuffer['s'] || keyBuffer['S'])
			AbramHullAngle -= TANKSPEED * 5;
		else
			AbramHullAngle += TANKSPEED * 5;

		if (AbramSmokeBudget == VEHICLESMOKEMOD)
		{
			struct Smoke tmpSmoke;
			tmpSmoke.smokeIDBuffer = Time;
			tmpSmoke.smokeCoordBuffer[0] = AbramXY[0];
			tmpSmoke.smokeCoordBuffer[1] = AbramXY[1];
			tmpSmoke.smokeDurBuffer = 0.01;
			tmpSmoke.smokeAngleBuffer = rand() % 360;
			tmpSmoke.smokeIDBufferSet = true;
			tmpSmoke.smokeActive = true;
			Smokes.push_back(tmpSmoke);
			AbramSmokeBudget = 0;
		}
		else
		{
			AbramSmokeBudget++;
		}

	}
	if ((keyBuffer['d'] || keyBuffer['D']) && AbramHP > 0) {
		if (keyBuffer['s'] || keyBuffer['S'])
			AbramHullAngle += TANKSPEED * 5;
		else
			AbramHullAngle -= TANKSPEED * 5;

		if (AbramSmokeBudget == VEHICLESMOKEMOD)
		{
			struct Smoke tmpSmoke;
			tmpSmoke.smokeIDBuffer = Time;
			tmpSmoke.smokeCoordBuffer[0] = AbramXY[0];
			tmpSmoke.smokeCoordBuffer[1] = AbramXY[1];
			tmpSmoke.smokeDurBuffer = 0.01;
			tmpSmoke.smokeAngleBuffer = rand() % 360;
			tmpSmoke.smokeIDBufferSet = true;
			tmpSmoke.smokeActive = true;
			Smokes.push_back(tmpSmoke);
			AbramSmokeBudget = 0;
		}
		else
		{
			AbramSmokeBudget++;
		}

	}
	if ((keyBuffer['q'] || keyBuffer['Q']) && AbramHP > 0) {
		AbramTurretAngle += TANKSPEED * 5;
	}
	if ((keyBuffer['e'] || keyBuffer['E']) && AbramHP > 0) {
		AbramTurretAngle -= TANKSPEED * 5;
	}

	if ((keyBuffer['i'] || keyBuffer['I'] || keyBuffer['8']) && IS3HP > 0) {
		if (((IS3XY[0] - IS3TX) < MAPEDGEX && (IS3XY[0] - IS3TX) > -MAPEDGEX) &&
			(!MapCollisionModel(IS3XY[0], IS3XY[1], IS3TX, IS3TY, (-1), 1)) &&
			!(((IS3XY[0] - IS3TX < AbramXY[0] + BODY) && (IS3XY[0] - IS3TX > AbramXY[0] - BODY)) &&
			((IS3XY[1] - IS3TY < AbramXY[1] + BODY) && (IS3XY[1] - IS3TY > AbramXY[1] - BODY))))
		{
			IS3XY[0] -= IS3TX;
		}
		else
		{
			//adjust angle
			if (IS3TX > 0)
			{
				//hit a wall horizontal wall from top going down
				if (IS3TY > 0)
					IS3HullAngle -= TANKSPEED * 5 * 2;
				else
					IS3HullAngle += TANKSPEED * 5 * 2;

			}
			else
			{
				//hit a wall horizontal wall from bottom going up
				if (IS3TY > 0)
					IS3HullAngle += TANKSPEED * 5 * 2;
				else
					IS3HullAngle -= TANKSPEED * 5 * 2;
			}
		}
		if (((IS3XY[1] - IS3TY) < MAPEDGEY && (IS3XY[1] - IS3TY) > -MAPEDGEY) &&
			(!MapCollisionModel(IS3XY[0], IS3XY[1], IS3TX, IS3TY, (-1), 2)) &&
			!(((IS3XY[0] - IS3TX < AbramXY[0] + BODY) && (IS3XY[0] - IS3TX > AbramXY[0] - BODY)) &&
			((IS3XY[1] - IS3TY < AbramXY[1] + BODY) && (IS3XY[1] - IS3TY > AbramXY[1] - BODY))))
		{
			IS3XY[1] -= IS3TY;
		}
		else
		{
			//adjust angle
			if (IS3TY > 0)
			{
				//hit a wall Vertical wall from right going left
				if (IS3TX > 0)
					IS3HullAngle += TANKSPEED * 5 * 2;
				else
					IS3HullAngle -= TANKSPEED * 5 * 2;

			}
			else
			{
				//hit a wall Vertical wall from left going right
				if (IS3TX > 0)
					IS3HullAngle -= TANKSPEED * 5 * 2;
				else
					IS3HullAngle += TANKSPEED * 5 * 2;
			}
		}

		if (IS3SmokeBudget == VEHICLESMOKEMOD)
		{
			struct Smoke tmpSmoke;
			tmpSmoke.smokeIDBuffer = Time;
			tmpSmoke.smokeCoordBuffer[0] = IS3XY[0];
			tmpSmoke.smokeCoordBuffer[1] = IS3XY[1];
			tmpSmoke.smokeDurBuffer = 0.01;
			tmpSmoke.smokeAngleBuffer = rand() % 360;
			tmpSmoke.smokeIDBufferSet = true;
			tmpSmoke.smokeActive = true;
			Smokes.push_back(tmpSmoke);
			IS3SmokeBudget = 0;
		}
		else
		{
			IS3SmokeBudget++;
		}

		std::vector<Crate>::iterator cratecheck = CrateCollisionModel(IS3XY[0], IS3XY[1],
			IS3TX,
			IS3TX,
			-1
		);
		if (cratecheck != Crates.end())
		{
			switch (cratecheck->type)
			{
			case 0:
				IS3Shells = SHELLSTORAGE;
				alSourcePlay(Sources[12]);
				break;
			case 1:
				IS3Smoke = SMOKECOUNT;
				alSourcePlay(Sources[12]);
				break;
			case 2:
				IS3HP = TANKHP;
				alSourcePlay(Sources[11]);
				break;
			case 3:
				IS3HP -= TANKHP;
				alSourcePlay(Sources[8]);
				break;
			case 4:
				alSourcePlay(Sources[14]);
				aTen('2');
				break;
			}
			//Crates[cratecheck].isActive = false;
			cratecheck->isActive = false;
			//myMap.isCrate[Crates[cratecheck].i][Crates[cratecheck].j] = false;
			myMap.isCrate[cratecheck->i][cratecheck->j] = false;
			Crates.erase(cratecheck);
		}
	}
	if ((keyBuffer['k'] || keyBuffer['K'] || keyBuffer['5']) && IS3HP > 0) {
		if (((IS3XY[0] + IS3TX) < MAPEDGEX && (IS3XY[0] + IS3TX) > -MAPEDGEX) &&
			(!MapCollisionModel(IS3XY[0], IS3XY[1], IS3TX, IS3TY, (1), 1)) &&
			!(((IS3XY[0] + IS3TX < AbramXY[0] + BODY) && (IS3XY[0] + IS3TX > AbramXY[0] - BODY)) &&
			((IS3XY[1] + IS3TY < AbramXY[1] + BODY) && (IS3XY[1] + IS3TY > AbramXY[1] - BODY))))
		{
			IS3XY[0] += IS3TX;
		}
		else
		{
			//adjust angle
			if (IS3TX > 0)
			{
				//hit a wall horizontal wall from top going down
				if (IS3TY > 0)
					IS3HullAngle -= TANKSPEED * 5 * 2;
				else
					IS3HullAngle += TANKSPEED * 5 * 2;

			}
			else
			{
				//hit a wall horizontal wall from bottom going up
				if (IS3TY > 0)
					IS3HullAngle += TANKSPEED * 5 * 2;
				else
					IS3HullAngle -= TANKSPEED * 5 * 2;
			}
		}
		if (((IS3XY[1] + IS3TY) < MAPEDGEY && (IS3XY[1] + IS3TY) > -MAPEDGEY) &&
			(!MapCollisionModel(IS3XY[0], IS3XY[1], IS3TX, IS3TY, (1), 2)) &&
			!(((IS3XY[0] + IS3TX < AbramXY[0] + BODY) && (IS3XY[0] + IS3TX > AbramXY[0] - BODY)) &&
			((IS3XY[1] + IS3TY < AbramXY[1] + BODY) && (IS3XY[1] + IS3TY > AbramXY[1] - BODY))))
		{
			IS3XY[1] += IS3TY;
		}
		else
		{
			//adjust angle
			if (IS3TY > 0)
			{
				//hit a wall Vertical wall from right going left
				if (IS3TX > 0)
					IS3HullAngle += TANKSPEED * 5 * 2;
				else
					IS3HullAngle -= TANKSPEED * 5 * 2;

			}
			else
			{
				//hit a wall Vertical wall from left going right
				if (IS3TX > 0)
					IS3HullAngle -= TANKSPEED * 5 * 2;
				else
					IS3HullAngle += TANKSPEED * 5 * 2;
			}
		}

		if (IS3SmokeBudget == VEHICLESMOKEMOD)
		{
			struct Smoke tmpSmoke;
			tmpSmoke.smokeIDBuffer = Time;
			tmpSmoke.smokeCoordBuffer[0] = IS3XY[0];
			tmpSmoke.smokeCoordBuffer[1] = IS3XY[1];
			tmpSmoke.smokeDurBuffer = 0.01;
			tmpSmoke.smokeAngleBuffer = rand() % 360;
			tmpSmoke.smokeIDBufferSet = true;
			tmpSmoke.smokeActive = true;
			Smokes.push_back(tmpSmoke);
			IS3SmokeBudget = 0;
		}
		else
		{
			IS3SmokeBudget++;
		}

	}
	if ((keyBuffer['j'] || keyBuffer['J'] || keyBuffer['4']) && IS3HP > 0) {
		if (keyBuffer['k'] || keyBuffer['K'] || keyBuffer['5'])
			IS3HullAngle -= TANKSPEED * 5;
		else
			IS3HullAngle += TANKSPEED * 5;

		if (IS3SmokeBudget == VEHICLESMOKEMOD)
		{
			struct Smoke tmpSmoke;
			tmpSmoke.smokeIDBuffer = Time;
			tmpSmoke.smokeCoordBuffer[0] = IS3XY[0];
			tmpSmoke.smokeCoordBuffer[1] = IS3XY[1];
			tmpSmoke.smokeDurBuffer = 0.01;
			tmpSmoke.smokeAngleBuffer = rand() % 360;
			tmpSmoke.smokeIDBufferSet = true;
			tmpSmoke.smokeActive = true;
			Smokes.push_back(tmpSmoke);
			IS3SmokeBudget = 0;
		}
		else
		{
			IS3SmokeBudget++;
		}


		std::vector<Crate>::iterator cratecheck = CrateCollisionModel(IS3XY[0], IS3XY[1],
			IS3TX,
			IS3TX,
			1
		);
		if (cratecheck != Crates.end())
		{
			switch (cratecheck->type)
			{
			case 0:
				IS3Shells = SHELLSTORAGE;
				alSourcePlay(Sources[12]);
				break;
			case 1:
				IS3Smoke = SMOKECOUNT;
				alSourcePlay(Sources[12]);
				break;
			case 2:
				IS3HP = TANKHP;
				alSourcePlay(Sources[11]);
				break;
			case 3:
				IS3HP -= TANKHP;
				alSourcePlay(Sources[8]);
				break;
			case 4:
				alSourcePlay(Sources[14]);
				aTen('2');
				break;
			}
			//Crates[cratecheck].isActive = false;
			cratecheck->isActive = false;
			//myMap.isCrate[Crates[cratecheck].i][Crates[cratecheck].j] = false;
			myMap.isCrate[cratecheck->i][cratecheck->j] = false;
			Crates.erase(cratecheck);
		}
	}
	if ((keyBuffer['l'] || keyBuffer['L'] || keyBuffer['6']) && IS3HP > 0) {
		if (keyBuffer['k'] || keyBuffer['K'] || keyBuffer['5'])
			IS3HullAngle += TANKSPEED * 5;
		else
			IS3HullAngle -= TANKSPEED * 5;

		if (IS3SmokeBudget == VEHICLESMOKEMOD)
		{
			struct Smoke tmpSmoke;
			tmpSmoke.smokeIDBuffer = Time;
			tmpSmoke.smokeCoordBuffer[0] = IS3XY[0];
			tmpSmoke.smokeCoordBuffer[1] = IS3XY[1];
			tmpSmoke.smokeDurBuffer = 0.01;
			tmpSmoke.smokeAngleBuffer = rand() % 360;
			tmpSmoke.smokeIDBufferSet = true;
			tmpSmoke.smokeActive = true;
			Smokes.push_back(tmpSmoke);
			IS3SmokeBudget = 0;
		}
		else
		{
			IS3SmokeBudget++;
		}

	}
	if ((keyBuffer['u'] || keyBuffer['U'] || keyBuffer['7']) && IS3HP > 0) {
		IS3TurretAngle += TANKSPEED * 5;
	}
	if ((keyBuffer['o'] || keyBuffer['O'] || keyBuffer['9']) && IS3HP > 0) {
		IS3TurretAngle -= TANKSPEED * 5;
	}
	if (keyBuffer[ESCAPE]) {
		isInMenu = true;
		resetState();
		for (int i = 0; i < 8; i++)
			alSourceStop(Sources[i]);
	}
	if (keyBuffer['f'] || keyBuffer['F'] || keyBuffer[SPACEBAR])
	{
		if ((Time - AbramLastShot) < 0)
			AbramLastShot = 0;

		if (AbramShells > 0 && AbramHP > 0 && ((Time - AbramLastShot) >= RELOADTIME))
		{
			alSourcePlay(Sources[8]);
			AbramLastShot = Time;
			Shells[shellSize].x = AbramXY[0];
			Shells[shellSize].y = AbramXY[1];
			Shells[shellSize].angle = AbramTurretAngle + AbramHullAngle;
			Shells[shellSize].shooterId = ABRAMID;
			Shells[shellSize].startTime = Time;
			Shells[shellSize].active = true;
			if (shellSize < SHELLMAX - 1)
				shellSize++;
			else
				shellSize = 0;
			AbramShells--;
			for (int i = 0; i < 5; i++)
			{
				struct Smoke tmpSmoke;
				tmpSmoke.smokeIDBuffer = Time;
				tmpSmoke.smokeCoordBuffer[0] = AbramXY[0] - 5 * sin((AbramTurretAngle + AbramHullAngle) * PI / 180.0);
				tmpSmoke.smokeCoordBuffer[1] = AbramXY[1] - 5 * cos((AbramTurretAngle + AbramHullAngle) * PI / 180.0);
				tmpSmoke.smokeDurBuffer = 0.015;
				tmpSmoke.smokeAngleBuffer = rand() % 360;
				tmpSmoke.smokeIDBufferSet = true;
				tmpSmoke.smokeActive = true;
				Smokes.push_back(tmpSmoke);

			}
		}
	}
	if (keyBuffer['r'] || keyBuffer['R'])
	{
		if ((Time - AbramLastShotB) < 0)
			AbramLastShotB = 0;

		if (AbramHP > 0 && ((Time - AbramLastShotB) >= RELOADTIME / 20))
		{
			alSourcePlay(Sources[13]);
			AbramLastShotB = Time;
			Bullets[bulletSize].x = AbramXY[0];
			Bullets[bulletSize].y = AbramXY[1];
			Bullets[bulletSize].angle = AbramTurretAngle + AbramHullAngle;
			Bullets[bulletSize].shooterId = ABRAMID;
			Bullets[bulletSize].startTime = Time;
			Bullets[bulletSize].active = true;
			if (bulletSize < BULLETMAX - 1)
				bulletSize++;
			else
				bulletSize = 0;
			for (int i = 0; i < 5; i++)
			{
				struct Smoke tmpSmoke;
				tmpSmoke.smokeIDBuffer = Time;
				tmpSmoke.smokeCoordBuffer[0] = AbramXY[0] - 5 * sin((AbramTurretAngle + AbramHullAngle) * PI / 180.0);
				tmpSmoke.smokeCoordBuffer[1] = AbramXY[1] - 5 * cos((AbramTurretAngle + AbramHullAngle) * PI / 180.0);
				tmpSmoke.smokeDurBuffer = 0.0075;
				tmpSmoke.smokeAngleBuffer = rand() % 360;
				tmpSmoke.smokeIDBufferSet = true;
				tmpSmoke.smokeActive = true;
				Smokes.push_back(tmpSmoke);
			}
		}
	}
	if (keyBuffer['h'] || keyBuffer['H'] || keyBuffer['0'])
	{
		if ((Time - IS3LastShot) < 0)
			IS3LastShot = 0;

		if (IS3Shells > 0 && IS3HP > 0 && ((Time - IS3LastShot) >= RELOADTIME))
		{
			alSourcePlay(Sources[8]);
			IS3LastShot = Time;
			Shells[shellSize].x = IS3XY[0];
			Shells[shellSize].y = IS3XY[1];
			Shells[shellSize].angle = IS3TurretAngle + IS3HullAngle;
			Shells[shellSize].shooterId = IS3ID;
			Shells[shellSize].startTime = Time;
			Shells[shellSize].active = true;
			if (shellSize < SHELLMAX - 1)
				shellSize++;
			else
				shellSize = 0;
			IS3Shells--;
			for (int i = 0; i < 5; i++)
			{

				struct Smoke tmpSmoke;
				tmpSmoke.smokeIDBuffer = Time;
				tmpSmoke.smokeCoordBuffer[0] = IS3XY[0] - 5 * sin((IS3TurretAngle + IS3HullAngle) * PI / 180.0);
				tmpSmoke.smokeCoordBuffer[1] = IS3XY[1] - 5 * cos((IS3TurretAngle + IS3HullAngle) * PI / 180.0);
				tmpSmoke.smokeDurBuffer = 0.015;
				tmpSmoke.smokeAngleBuffer = rand() % 360;
				tmpSmoke.smokeIDBufferSet = true;
				tmpSmoke.smokeActive = true;
				Smokes.push_back(tmpSmoke);

			}
		}
	}
	if (keyBuffer['y'] || keyBuffer['Y'] || keyBuffer[ENTER])
	{
		if ((Time - IS3LastShotB) < 0)
			IS3LastShotB = 0;

		if (IS3HP > 0 && ((Time - IS3LastShotB) >= RELOADTIME / 20))
		{
			alSourcePlay(Sources[13]);
			IS3LastShotB = Time;
			Bullets[bulletSize].x = IS3XY[0];
			Bullets[bulletSize].y = IS3XY[1];
			Bullets[bulletSize].angle = IS3TurretAngle + IS3HullAngle;
			Bullets[bulletSize].shooterId = IS3ID;
			Bullets[bulletSize].startTime = Time;
			Bullets[bulletSize].active = true;
			if (bulletSize < BULLETMAX - 1)
				bulletSize++;
			else
				bulletSize = 0;
			for (int i = 0; i < 5; i++)
			{
				struct Smoke tmpSmoke;
				tmpSmoke.smokeIDBuffer = Time;
				tmpSmoke.smokeCoordBuffer[0] = IS3XY[0] - 5 * sin((IS3TurretAngle + IS3HullAngle) * PI / 180.0);
				tmpSmoke.smokeCoordBuffer[1] = IS3XY[1] - 5 * cos((IS3TurretAngle + IS3HullAngle) * PI / 180.0);
				tmpSmoke.smokeDurBuffer = 0.0075;
				tmpSmoke.smokeAngleBuffer = rand() % 360;
				tmpSmoke.smokeIDBufferSet = true;
				tmpSmoke.smokeActive = true;
				Smokes.push_back(tmpSmoke);
			}
		}
	}
}
void makeAI(bool isActive, char Tank)
{
	myAIKB.isAI = isActive && AIACTIVE;
	myAIKB.AIID = Tank;
	if (myAIKB.isAI)
	{
		switch (myAIKB.AIID)
		{
		case 'A':
		case 'a':
			myAIKB.playerHP = &IS3HP;
			myAIKB.playerPos = IS3XY;
			myAIKB.playerAmmo = &IS3Shells;
			myAIKB.playerHullAngle = &IS3HullAngle;
			myAIKB.playerTurretAngle = &IS3TurretAngle;

			myAIKB.AIHP = &AbramHP;
			myAIKB.AIPos = AbramXY;
			myAIKB.AIAmmo = &AbramShells;
			myAIKB.AIHullAngle = &AbramHullAngle;
			myAIKB.AITurretAngle = &AbramTurretAngle;
			break;
		case 'T':
		case 't':
			myAIKB.playerHP = &AbramHP;
			myAIKB.playerPos = AbramXY;
			myAIKB.playerAmmo = &AbramShells;
			myAIKB.playerHullAngle = &AbramHullAngle;
			myAIKB.playerTurretAngle = &AbramTurretAngle;

			myAIKB.AIHP = &IS3HP;
			myAIKB.AIPos = IS3XY;
			myAIKB.AIAmmo = &IS3Shells;
			myAIKB.AIHullAngle = &IS3HullAngle;
			myAIKB.AITurretAngle = &IS3TurretAngle;
			break;
		}
		myAIKB.agent->env = (SimpleAI::InnerAIKB*)&myAIKB;
	}
}
#endif