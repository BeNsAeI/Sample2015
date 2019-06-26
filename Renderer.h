#ifndef RENDERER_H
#define RENDERER_H
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
#include <time.h>
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

void drawSparks(float X, float Y)
{
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3f(X, 0, Y);
	glVertex3f(X + (rand() % 10), 10, Y + (rand() % 10));
	glEnd();
	glBegin(GL_LINES);
	glColor3f(1, 1, 0);
	glVertex3f(X, 0, Y);
	glVertex3f(X + (rand() % 10), 10, Y + (rand() % 10));
	glEnd();
	glColor3f(1, 0, 0);
	glVertex3f(X, 0, Y);
	glVertex3f(X + (rand() % 10), 10, Y + (rand() % 10));
	glEnd();
}
void drawSmoke(float X, float Y, float Z, float angle, float scale, float r, float g, float b, float beginTime, float duration)
{
	int beginPoint;
	int endPoint;
	if ((Time - beginTime) >= 0 && (Time - beginTime) <= duration)
	{
		glPushMatrix();
		glTranslatef(X, Z + 3 + (Time - beginTime) * 100, Y);
		glScalef(1 + (Time - beginTime) * 10000, 1 + (Time - beginTime) * 10000, 1 + (Time - beginTime) * 10000);
		glRotatef(angle, 0, 1, 0);
		glPushMatrix();
		glScalef(scale, scale, scale);
		glTranslatef(0.5, 0, 0.25);
		beginPoint = trees[7][START];
		endPoint = trees[7][END] - trees[7][START];
		glPushMatrix();
		glTranslatef(0.5, 0, 0.5);
		SetMaterial(r - 0.10, g - 0.10, b - 0.10, 1);
		glColor3f(r, g, b);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

	}
}
void drawExplosion(float X, float Y, float Z, float angle, float scale, float r, float g, float b, float beginTime, float duration)
{
	int beginPoint;
	int endPoint;
	if ((Time - beginTime) >= 0 && (Time - beginTime) <= duration)
	{
		glPushMatrix();
		glTranslatef(X, Z + 3 + (Time - beginTime) * 100, Y);
		glScalef(1 + (Time - beginTime) * 10000, 1 + (Time - beginTime) * 10000, 1 + (Time - beginTime) * 10000);
		glRotatef(angle, 0, 1, 0);
		glPushMatrix();
		glScalef(scale, scale, scale);
		glTranslatef(0.5, 0, 0.25);
		beginPoint = trees[7][START];
		endPoint = trees[7][END] - trees[7][START];
		glPushMatrix();
		glTranslatef(0.5, 0, 0.5);
		glColor3f(r, g, b);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

	}
}
void drawIS3(float X, float Y, float Z, float hullAngle, float turretAngle)
{
	glPushMatrix();
	glTranslatef(X, Y, Z);	//movement
	glTranslatef(0, 2.1, 0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = IS3[0][START];
	int endPoint = IS3[0][END] - IS3[0][START];
	glPushMatrix();
	glTranslatef(0, 3, 0);
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0, -3, 0);
	glColor3f(0, 0.25, 1);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = IS3[1][START];
	endPoint = IS3[1][END] - IS3[1][START];
	glPushMatrix();
	glTranslatef(0.75, 0, 0);
	glColor3f(0, 0.25, 1);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = IS3[2][START];
	endPoint = IS3[2][END] - IS3[2][START];
	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-13, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(0, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawIS3Dead(float X, float Y, float Z, float hullAngle, float turretAngle)
{
	glPushMatrix();
	glTranslatef(X, Y, Z);	//movement
	glTranslatef(0, 2.1, 0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = IS3[0][START];
	int endPoint = IS3[0][END] - IS3[0][START];
	glPushMatrix();
	glTranslatef(0, 3, 0);
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0, -3, 0);
	glColor3f(0, 0, 0.1);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = IS3[1][START];
	endPoint = IS3[1][END] - IS3[1][START];
	glPushMatrix();
	glTranslatef(0.75, 0, 0);
	glColor3f(0, 0, 0.1);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = IS3[2][START];
	endPoint = IS3[2][END] - IS3[2][START];
	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-13, 0, -3);
	glColor3f(0, 0, 0.1);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(0, 0, -3);
	glColor3f(0, 0, 0.1);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawAbram(float X, float Y, float Z, float hullAngle, float turretAngle)
{
	glPushMatrix();
	glTranslatef(X, Y, Z);	//movement
	glTranslatef(0, 2.1, 0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = Abram[0][START];
	int endPoint = Abram[0][END] - Abram[0][START];
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0, -1, 0);
	glTranslatef(1.75, 0, 0.25);
	glColor3f(1, 0.5, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Abram[1][START];
	endPoint = Abram[1][END] - Abram[1][START];
	glPushMatrix();
	glTranslatef(-1, 0, 0.25);
	glColor3f(1, 0.5, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-12.75, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(-0.25, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawAbramDead(float X, float Y, float Z, float hullAngle, float turretAngle)
{
	glPushMatrix();
	glTranslatef(X, Y, Z);	//movement
	glTranslatef(0, 2.1, 0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = Abram[0][START];
	int endPoint = Abram[0][END] - Abram[0][START];
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0, -1, 0);
	glTranslatef(1.75, 0, 0.25);
	glColor3f(0.1, 0.1, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Abram[1][START];
	endPoint = Abram[1][END] - Abram[1][START];
	glPushMatrix();
	glTranslatef(-1, 0, 0.25);
	glColor3f(0.1, 0.1, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-12.75, 0, -3);
	glColor3f(0.1, 0.1, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(-0.25, 0, -3);
	glColor3f(0.1, 0.1, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawT29(float X, float Y, float Z, float hullAngle, float turretAngle)
{
	glPushMatrix();
	glTranslatef(X, Y, Z);	//movement
	glTranslatef(0, 2.1, 0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = T29[0][START];
	int endPoint = T29[0][END] - T29[0][START];
	glPushMatrix();
	glTranslatef(0, 3, 0);
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0, -3, 0);
	glColor3f(0.38, 0.2, 0.01);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = T29[1][START];
	endPoint = T29[1][END] - T29[1][START];
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glColor3f(0.38, 0.2, 0.01);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-14.75, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(1.75, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawT29Dead(float X, float Y, float Z, float hullAngle, float turretAngle)
{
	glPushMatrix();
	glTranslatef(X, Y, Z);	//movement
	glTranslatef(0, 2.1, 0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = T29[0][START];
	int endPoint = T29[0][END] - T29[0][START];
	glPushMatrix();
	glTranslatef(0, 3, 0);
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0, -3, 0);
	glColor3f(0.1, 0.1, 0.01);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = T29[1][START];
	endPoint = T29[1][END] - T29[1][START];
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glColor3f(0.1, 0.1, 0.01);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-14.75, 0, -3);
	glColor3f(0.1, 0.1, 0.01);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(1.75, 0, -3);
	glColor3f(0.1, 0.1, 0.01);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawE100(float X, float Y, float Z, float hullAngle, float turretAngle)
{
	glPushMatrix();
	glTranslatef(X, Y, Z);	//movement
	glTranslatef(0, 2.1, 0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = E100[0][START];
	int endPoint = E100[0][END] - E100[0][START];
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0, -1, 0);
	glColor3f(1, 0.1, 0.5);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = E100[1][START];
	endPoint = E100[1][END] - E100[1][START];
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glColor3f(1, 0.1, 0.5);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-13, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(0, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawE100Dead(float X, float Y, float Z, float hullAngle, float turretAngle)
{
	glPushMatrix();
	glTranslatef(X, Y, Z);	//movement
	glTranslatef(0, 2.1, 0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = E100[0][START];
	int endPoint = E100[0][END] - E100[0][START];
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0, -1, 0);
	glColor3f(0.1, 0.1, 0.01);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = E100[1][START];
	endPoint = E100[1][END] - E100[1][START];
	glPushMatrix();
	glTranslatef(0, 1, 0);
	glColor3f(0.1, 0.1, 0.01);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-13, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(0, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawType59(float X, float Y, float Z, float hullAngle, float turretAngle)
{
	glPushMatrix();
	glTranslatef(X, Y, Z);	//movement
	glTranslatef(0, 2.1, 0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = Type59[0][START];
	int endPoint = Type59[0][END] - Type59[0][START];
	glPushMatrix();
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0.75, 0, -8);
	glColor3f(0, 0.5, 0.125);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Type59[1][START];
	endPoint = Type59[1][END] - Type59[1][START];
	glPushMatrix();
	glTranslatef(0.75, 0, -8);
	glColor3f(0, 0.5, 0.125);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-13, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(0, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawType59Dead(float X, float Y, float Z, float hullAngle, float turretAngle)
{
	glPushMatrix();
	glTranslatef(X, Y, Z);	//movement
	glTranslatef(0, 2.1, 0);
	glRotatef(hullAngle, 0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glScalef(TANKSCALE, TANKSCALE, TANKSCALE);

	int beginPoint = Type59[0][START];
	int endPoint = Type59[0][END] - Type59[0][START];
	glPushMatrix();
	glRotatef(turretAngle, 0, 0, 1);
	glTranslatef(0.75, 0, -8);
	glColor3f(0.1, 0.1, 0.01);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Type59[1][START];
	endPoint = Type59[1][END] - Type59[1][START];
	glPushMatrix();
	glTranslatef(0.75, 0, -8);
	glColor3f(0.1, 0.1, 0.01);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[0][START];
	endPoint = Track[0][END] - Track[0][START];
	glPushMatrix();
	glTranslatef(-13, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = Track[1][START];
	endPoint = Track[1][END] - Track[1][START];
	glPushMatrix();
	glTranslatef(0, 0, -3);
	glColor3f(0.3, 0.25, 0.18);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawA_Ten()
{
	int beginPoint;
	int endPoint;

	glPushMatrix();

	int currentTime = clock();

	glTranslatef(aTenTargetCoord, 20 - 10 *sin((currentTime - drawATenActiveTime) % 360) / 150, (float)((400 * 33) - (int)((currentTime - drawATenActiveTime)) % (600 * 33)) / 33);
	glRotatef((float)(-(90*150) + (int)((currentTime - drawATenActiveTime)) % (120 * 150))/150, 1, 0, 0);
	glRotatef(270, 0, 1, 0);
	glScalef(0.03625, 0.03625, 0.03625);

	beginPoint = A_10_M[0][START];
	endPoint = A_10_M[0][END] - A_10_M[0][START];

	glColor3f(0.5, 0.5, 0.5);
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = A_10_M[1][START];
	endPoint = A_10_M[1][END] - A_10_M[1][START];

	glColor3f(0, 0.75, 1);
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = A_10_M[2][START];
	endPoint = A_10_M[2][END] - A_10_M[2][START];

	glColor3f(0.0625, 0.0625, 0.0625);
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
	if ((currentTime - drawATenActiveTime)/CLOCKS_PER_SEC > flightDuration)
	{
		drawATenActive = false;
		drawATenActiveTime = 0;

	}
}
void drawCube(float X, float Y, float Z, float r, float g, float b)
{
	glPushMatrix();				// 0
	glTranslatef(X, Z, Y);	//movement
	glTranslatef(-5.25, CUBESIZE / 2, 0);
	glScalef(CUBESIZE / 2, CUBESIZE / 2, CUBESIZE / 2);

	int beginPoint = cube[START];
	int endPoint = cube[END] - cube[START];

	glPushMatrix();				// 1
	glTranslatef(1.75, 0, 0);

	glPushMatrix();				// 2
	glScalef(.9, .9, .9);
	//SetMaterial(0.25, 0.25, 0.25, 1.0);
	glColor3f(0.25, 0.25, 0.25);
	SetMaterial(0.25, 0.25, 0.25, 1.0);
	glRotated(-90, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();				// 2

								//SetMaterial(r, g, b, 1.0);
	glColor3f(r, g, b);
	SetMaterial(r, g, b, 1.0);
	glPushMatrix();				// 3
	glTranslatef(-0.55, -0.55, 0);
	glScalef(0.48, 0.48, .95);
	glRotated(-90, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();				// 3

	glPushMatrix();				// 4
	glTranslatef(0.55, -0.55, 0);
	glScalef(0.48, 0.48, .95);
	glRotated(-90, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();				// 4

	glPushMatrix();				// 5
	glTranslatef(0, 0.55, -0.55);
	glScalef(.95, 0.48, 0.48);
	glRotated(-90, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();				// 5

	glPushMatrix();				// 6
	glTranslatef(0, 0.55, 0.55);
	glScalef(.95, 0.48, 0.48);
	glRotated(-90, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();				// 6

	glPopMatrix();				// 1

	glPopMatrix();				// 0
}
void drawTreeCube(float X, float Y, float angle, int index)
{
	int beginPoint;
	int endPoint;
	switch (index)
	{
	case 0:
		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		glRotatef(angle, 0, 1, 0);
		glTranslatef(-0.325, 0, 0.275);

		beginPoint = trees[0][START];
		endPoint = 3960 - 2250;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		//SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();

		beginPoint = trees[0][START] + 3960 - 2250;
		endPoint = 2250;
		glPushMatrix();
		//SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 1:
		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		glRotatef(angle, 0, 1, 0);
		glTranslatef(0, 0, 0.15);
		beginPoint = trees[1][START];
		endPoint = 5688 - 2022;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		//SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		beginPoint = trees[1][START] + 5688 - 2022;
		endPoint = 2022;
		glPushMatrix();
		//SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 2:

		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		glScalef(1.25, 1.25, 1.25);
		glRotatef(angle, 0, 1, 0);
		glTranslatef(0.22, 0, 0.2);
		beginPoint = trees[2][START];
		endPoint = 4398 - 3525;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		//SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();

		beginPoint = trees[2][START] + 4398 - 3525;
		endPoint = 3525;
		glPushMatrix();
		//SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 3:

		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		glScalef(1.5, 1.5, 1.5);
		glRotatef(angle, 0, 1, 0);
		glTranslatef(0.4, 0, 0.2);
		beginPoint = trees[3][START];
		endPoint = 3522 - 3000;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		//SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();

		beginPoint = trees[3][START] + 3522 - 3000;
		endPoint = 3000;
		glPushMatrix();
		//SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 4:

		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		glRotatef(angle, 0, 1, 0);
		glTranslatef(-0.24, 0.42, 0.21);
		beginPoint = trees[4][START];
		endPoint = 4752 - 3816;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		//SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();

		beginPoint = trees[4][START] + 4752 - 3816;
		endPoint = 3816;
		glPushMatrix();
		//SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 5:

		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		glRotatef(angle, 0, 1, 0);
		glTranslatef(0, 0.42, 0.21);
		beginPoint = trees[5][START];
		endPoint = 6105 - 4668;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		//SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();

		beginPoint = trees[5][START] + 6105 - 4668;
		endPoint = 4668;
		glPushMatrix();
		//SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 6:
		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glScalef(TREESCALE, TREESCALE, TREESCALE);
		glRotatef(angle, 0, 1, 0);
		glTranslatef(0.24, 0.42, 0.21);
		beginPoint = trees[6][START];
		endPoint = 6030 - 4944;
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		//SetMaterial(0.4, 0.2, 0, 1.0);
		glColor3f(0.4, 0.2, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();

		beginPoint = trees[6][START] + 6030 - 4944;
		endPoint = 4944;
		glPushMatrix();
		//SetMaterial(0, 0.75, 0, 1.0);
		glColor3f(0, 0.75, 0);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	case 7:
		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glScalef(ROCKSCALE, ROCKSCALE, ROCKSCALE);
		glRotatef(angle, 0, 1, 0);
		glTranslatef(0.75, 0, 0.25);
		beginPoint = trees[7][START];
		endPoint = trees[7][END] - trees[7][START];
		glPushMatrix();
		//SetMaterial(0.125, 0.125, 0.125, 1);
		glColor3f(0.25, 0.25, 0.25);
		glRotatef(270, 1, 0, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
		break;
	}
	if (index > 1 && index < 7)
	{
		glPushMatrix();
		glTranslatef(X, 0, Y);	//movement
		glRotatef(angle, 0, 1, 0);
		glTranslatef(2, 0.5, 5);
		glScalef(BUSHSCALE, BUSHSCALE / 2, BUSHSCALE);
		beginPoint = grass[START];
		endPoint = grass[END] - grass[START];
		glPushMatrix();
		glRotatef(270, 1, 0, 0);
		glColor3f(0, 0.5, 0);
		glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
		glPopMatrix();
		glPopMatrix();
	}
}
void drawShell(float X, float Y, float angle, float scale = 1)
{
	int beginPoint;
	int endPoint;

	glPushMatrix();
	glTranslatef(X, 2.75, Y);	//movement
	glRotatef(angle, 0, 1, 0);
	glTranslatef(0, 0, -3);
	glScalef(SHELLSCALE*scale, SHELLSCALE*scale, SHELLSCALE*scale);
	beginPoint = shell[START];
	endPoint = shell[END] - shell[START];
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	//SetMaterial(1, 1, 0, 1.0);
	glColor3f(1, 0.5, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();
	glPopMatrix();
}
void drawBullet(float X, float Y, float angle, float scale = 1)
{
	int beginPoint;
	int endPoint;

	glPushMatrix();
	glTranslatef(X + 0.1, 2.9, Y + 0.1);	//movement
	glRotatef(angle, 0, 1, 0);
	glTranslatef(0, 0, -3);
	glScalef(SHELLSCALE*scale / 2, SHELLSCALE*scale / 2, SHELLSCALE*scale / 2);
	beginPoint = shell[START];
	endPoint = shell[END] - shell[START];
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	//SetMaterial(1, 1, 0, 1.0);
	glColor3f(1, 0.5, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();
	glPopMatrix();
}
void drawAmmo(float X, float Y)
{
	int beginPoint;
	int endPoint;

	glPushMatrix();
	glTranslatef(X, 1, Y);	//movement
	glRotatef((int)(Time * 5000) % 360, 0, 1, 0);
	glTranslatef(0, 0, 0);
	glScalef(200, 200, 200);
	beginPoint = ammo[START];
	endPoint = ammo[END] - ammo[START];
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	//SetMaterial(1, 1, 0, 1.0);
	glColor3f(1 - sin(Time * 1000) / 4, 0.5 + sin(Time * 1000) / 4, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();
	glPopMatrix();
}
void drawSmokeCrate(float X, float Y, int angle = 0)
{
	int beginPoint;
	int endPoint;

	glPushMatrix();

	glTranslatef(X, 1, Y);	//movement
	glRotatef(angle, 1, 0, 0);
	glRotatef((int)(Time * 5000) % 360, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);

	beginPoint = smokeCrate[1][START];
	endPoint = smokeCrate[1][END] - smokeCrate[1][START];

	//SetMaterial(0.25, 0.25, 0.25, 1.0);
	glColor3f(0.25, 0.25, 0.25);
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = smokeCrate[0][START];
	endPoint = smokeCrate[0][END] - smokeCrate[0][START];

	//SetMaterial(0, 1, 0, 1.0);
	glColor3f(0, 1 - sin(Time * 1000) * 3 / 4, 0);
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawHPCrate(float X, float Y)
{
	int beginPoint;
	int endPoint;

	glPushMatrix();

	glTranslatef(X, 1, Y);	//movement
	glRotatef((int)(Time * 5000) % 360, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);

	beginPoint = hpCrate[0][START];
	endPoint = hpCrate[0][END] - hpCrate[0][START];

	//SetMaterial(0.25, 0.25, 0.25, 1.0);
	glColor3f(0.25, 0.25, 0.25);
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = hpCrate[1][START];
	endPoint = hpCrate[1][END] - hpCrate[1][START];

	//SetMaterial(1, 0.75, 0, 1.0);
	glColor3f(1, 0.5 - sin(Time * 1000) / 4, 0);
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawRadioCrate(float X, float Y)
{
	int beginPoint;
	int endPoint;

	glPushMatrix();

	glTranslatef(X, 1, Y);	//movement
	glRotatef((int)(Time * 5000) % 360, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);

	beginPoint = radioCrate[0][START];
	endPoint = radioCrate[0][END] - radioCrate[0][START];

	glColor3f(0.05, 0.31, 0.1);
	glPushMatrix();
	glTranslatef(0, 5, 0);
	glRotatef(315, 1, 0, 0);
	glScalef(0.25, 0.25, 0.25);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	beginPoint = radioCrate[1][START];
	endPoint = radioCrate[1][END] - radioCrate[1][START];

	glColor3f(0.25 - sin(Time * 1000) / 4, 0.25 - sin(Time * 1000) / 4, 0.25 - sin(Time * 1000) / 4);
	glPushMatrix();
	glTranslatef(0, 5, 0);
	glRotatef(315, 1, 0, 0);
	glScalef(0.25, 0.25, 0.25);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawMine(float X, float Y)
{
	int beginPoint;
	int endPoint;

	glPushMatrix();

	glTranslatef(X, 1, Y);
	glRotatef(270, 1, 0, 0);
	glScalef(1.5, 1.5, 1.5);

	beginPoint = mineCrate[START];
	endPoint = mineCrate[END] - mineCrate[START];

	glColor3f(1, 0.5 + sin(Time * 5000) / 4, 0);
	glPushMatrix();

	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();

	glPopMatrix();
}
void drawGrass(float X, float Y)
{
	glPushMatrix();
	glTranslatef(X, 0, Y);	//movement
	glTranslatef(2, 0.5, 5);
	glScalef(0.5, 0.65, 0.5);
	int	beginPoint = grass[START];
	int endPoint = grass[END] - grass[START];
	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	glColor3f(0, 1, 0);
	glDrawArrays(GL_TRIANGLES, beginPoint, endPoint);
	glPopMatrix();
	glPopMatrix();
}
void drawSpark(float X, float Y, float angle, float timeIndex)
{
	glLineWidth(1);
	glBegin(GL_LINE_STRIP);
	glColor3f(1, 1, 1 - timeIndex * 20);
	glVertex3f(0, 10, 5);
	glVertex3f(0, 10, timeIndex * 100);
	glEnd();
}
void drawMenu()
{
	if (ADVANCEMENU)
	{
		switch (menuState)
		{
		case -1: // Login menue for users

			break;
		case 0:
		{
			PatternGrass->Use();
			float startx = 1 + MAPEDGEX / 2 + CUBESIZE;
			float startz = MAPEDGEY / 2 + CUBESIZE;
			float endx = (-MAPEDGEX / 2 - CUBESIZE);
			float endz = (-MAPEDGEY / 2 - CUBESIZE);
			float lengthx = startx - endx;
			float lengthz = startz - endz;
			int grainX = GRASSGRAINX*MENUMULTIPLIER*userGrassMultiplier;
			int grainY = GRASSGRAINY*MENUMULTIPLIER*userGrassMultiplier;
			glEnable(GL_NORMALIZE);
			glBegin(GL_QUADS);
			glPushMatrix();
			glColor3f(0.1, 0.1, 0.0);
			for (int i = 0; i < grainX; i++)
			{
				for (int j = 0; j < grainY; j++)
				{
					glVertex3f(startx - MENUXOFFSET - i*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - j*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - i*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - (j + 1)*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - (j + 1)*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - j*(lengthz) / grainY);
				}
			}
			glPopMatrix();
			glEnd();
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
			// draw map
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
			//end of drawing map

			// Push the GL attribute bits so that we don't wreck any settings
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			// Enable polygon offsets, and offset filled polygons forward by 2.5
			glDisable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(-2.5f, -2.5f);
			// Set the render mode to be line rendering with a thick line width
			glLineWidth(OUTLINE);
			// Set the colour to be white
			glColor3f(.75, .75, .75);
			// Render the object
			PatternSilh->Use();
			//backgroundRand = 9;
			switch (backgroundRand)
			{
			case 0:
				drawHPCrate(0, 0);
				break;
			case 1:
				drawSmokeCrate(0, 0);
				break;
			case 2:
				drawAmmo(0, 0);
				break;
			case 3:
				drawIS3(0, 0, 0, -45, -45);
				break;
			case 4:
				drawAbram(0, 0, 0, -45, -45);
				break;
			case 5:
				drawMine(0, 0);
				break;
			case 6:
				drawT29(0, 0, 0, -45, -45);
				break;
			case 7:
				drawE100(0, 0, 0, -45, -45);
				break;
			case 8:
				drawType59(0, 0, 0, -45, -45);
				break;
			case 9:
				drawRadioCrate(0, 0);
				break;
			}
			PatternSilh->Use(0);
			// Set the polygon mode to be filled triangles 
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(-2.5f, -2.5f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//glShadeModel(GL_FLAT);
			//glEnable(GL_LIGHTING);
			////SetPointLight(GL_LIGHT2, 0, 15, 0, 0.75, 0.75, 0.75);
			// Set the colour to the background
			glColor3f(0.0f, 0.0f, 0.0f);
			// Render the object
			//backgroundRand = 9;
			switch (backgroundRand)
			{
			case 0:
				Pattern->Use();
				drawHPCrate(0, 0);
				Pattern->Use(0);
				break;
			case 1:
				Pattern->Use();
				drawSmokeCrate(0, 0);
				Pattern->Use(0);
				break;
			case 2:
				Pattern->Use();
				drawAmmo(0, 0);
				Pattern->Use(0);
				break;
			case 3:
				PatternCamo->Use();
				drawIS3(0, 0, 0, -45, -45);
				PatternCamo->Use(0);
				break;
			case 4:
				PatternCamo->Use();
				drawAbram(0, 0, 0, -45, -45);
				PatternCamo->Use(0);
				break;
			case 5:
				Pattern->Use();
				drawMine(0, 0);
				Pattern->Use(0);
				break;
			case 6:
				PatternCamo->Use();
				drawT29(0, 0, 0, -45, -45);
				PatternCamo->Use(0);
				break;
			case 7:
				PatternCamo->Use();
				drawE100(0, 0, 0, -45, -45);
				PatternCamo->Use(0);
				break;
			case 8:
				PatternCamo->Use();
				drawType59(0, 0, 0, -45, -45);
				PatternCamo->Use(0);
				break;
			case 9:
				Pattern->Use();
				drawRadioCrate(0, 0);
				Pattern->Use(0);
				break;
			}
			// Pop the state changes off the attribute stack
			// to set things back how they were
			glPopAttrib();

			//glDisable(GL_LIGHT2);
			//glDisable(GL_LIGHTING);

			glDisableVertexAttribArray(0);
		}
		break;
		case 1:
		case 2:
		{
			PatternGrass->Use();
			float startx = 1 + MAPEDGEX / 2 + CUBESIZE;
			float startz = MAPEDGEY / 2 + CUBESIZE;
			float endx = (-MAPEDGEX / 2 - CUBESIZE);
			float endz = (-MAPEDGEY / 2 - CUBESIZE);
			float lengthx = startx - endx;
			float lengthz = startz - endz;
			int grainX = GRASSGRAINX*MENUMULTIPLIER*userGrassMultiplier;
			int grainY = GRASSGRAINY*MENUMULTIPLIER*userGrassMultiplier;
			glEnable(GL_NORMALIZE);
			glBegin(GL_QUADS);
			glPushMatrix();
			glColor3f(0.1, 0.1, 0.0);
			for (int i = 0; i < grainX; i++)
			{
				for (int j = 0; j < grainY; j++)
				{
					glVertex3f(startx - MENUXOFFSET - i*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - j*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - i*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - (j + 1)*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - (j + 1)*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - j*(lengthz) / grainY);
				}
			}
			glPopMatrix();
			glEnd();
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
			// draw map
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
			//end of drawing map

			// Push the GL attribute bits so that we don't wreck any settings
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			// Enable polygon offsets, and offset filled polygons forward by 2.5
			glDisable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(-2.5f, -2.5f);
			// Set the render mode to be line rendering with a thick line width
			glLineWidth(OUTLINE);
			// Set the colour to be white
			glColor3f(.75, .75, .75);
			// Render the object
			PatternSilh->Use();
			//BehnamSaeedi
			switch (PlayerOne)
			{
			case 0:
				drawAbram(0, 0, 3, -45, -45);
				break;
			case 1:
				drawIS3(0, 0, 3, -45, -45);
				break;
			case 2:
				drawT29(0, 0, 3, -45, -45);
				break;
			case 3:
				drawE100(0, 0, 3, -45, -45);
				break;
			case 4:
				drawType59(0, 0, 3, -45, -45);
				break;
			}
			switch (PlayerTwo)
			{
			case 0:
				drawAbram(0, 0, -6, -45, -45);
				break;
			case 1:
				drawIS3(0, 0, -6, -45, -45);
				break;
			case 2:
				drawT29(0, 0, -6, -45, -45);
				break;
			case 3:
				drawE100(0, 0, -6, -45, -45);
				break;
			case 4:
				drawType59(0, 0, -6, -45, -45);
				break;
			}
			PatternSilh->Use(0);
			// Set the polygon mode to be filled triangles 
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(-2.5f, -2.5f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//glShadeModel(GL_FLAT);
			//glEnable(GL_LIGHTING);
			////SetPointLight(GL_LIGHT2, 0, 15, 0, 0.75, 0.75, 0.75);
			// Set the colour to the background
			glColor3f(0.0f, 0.0f, 0.0f);
			// Render the object
			switch (PlayerOne)
			{
			case 0:
				PatternCamo->Use();
				drawAbram(0, 0, 3, -45, -45);
				PatternCamo->Use(0);
				break;
			case 1:
				PatternCamo->Use();
				drawIS3(0, 0, 3, -45, -45);
				PatternCamo->Use(0);
				break;
			case 2:
				PatternCamo->Use();
				drawT29(0, 0, 3, -45, -45);
				PatternCamo->Use(0);
				break;
			case 3:
				PatternCamo->Use();
				drawE100(0, 0, 3, -45, -45);
				PatternCamo->Use(0);
				break;
			case 4:
				PatternCamo->Use();
				drawType59(0, 0, 3, -45, -45);
				PatternCamo->Use(0);
				break;
			}
			switch (PlayerTwo)
			{
			case 0:
				PatternCamo->Use();
				drawAbram(0, 0, -6, -45, -45);
				PatternCamo->Use(0);
				break;
			case 1:
				PatternCamo->Use();
				drawIS3(0, 0, -6, -45, -45);
				PatternCamo->Use(0);
				break;
			case 2:
				PatternCamo->Use();
				drawT29(0, 0, -6, -45, -45);
				PatternCamo->Use(0);
				break;
			case 3:
				PatternCamo->Use();
				drawE100(0, 0, -6, -45, -45);
				PatternCamo->Use(0);
				break;
			case 4:
				PatternCamo->Use();
				drawType59(0, 0, -6, -45, -45);
				PatternCamo->Use(0);
				break;
			}

			glPopAttrib();


			glDisableVertexAttribArray(0);
		}
		break;
		case 3:
		{
			PatternGrass->Use();
			float startx = 1 + MAPEDGEX + CUBESIZE;
			float startz = MAPEDGEY + CUBESIZE;
			float endx = (-MAPEDGEX - CUBESIZE);
			float endz = (-MAPEDGEY - CUBESIZE);
			float lengthx = startx - endx;
			float lengthz = startz - endz;
			int grainX = GRASSGRAINX*userGrassMultiplier;
			int grainY = GRASSGRAINY*userGrassMultiplier;
			glEnable(GL_NORMALIZE);
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
			// draw map
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
			//end of drawing map

			glDisableVertexAttribArray(0);
		}
		break;
		case 4:
		{
			PatternGrass->Use();
			float startx = 1 + MAPEDGEX / 2 + CUBESIZE;
			float startz = MAPEDGEY / 2 + CUBESIZE;
			float endx = (-MAPEDGEX / 2 - CUBESIZE);
			float endz = (-MAPEDGEY / 2 - CUBESIZE);
			float lengthx = startx - endx;
			float lengthz = startz - endz;
			int grainX = GRASSGRAINX*MENUMULTIPLIER*userGrassMultiplier;
			int grainY = GRASSGRAINY*MENUMULTIPLIER*userGrassMultiplier;
			glEnable(GL_NORMALIZE);
			glBegin(GL_QUADS);
			glPushMatrix();
			glColor3f(0.1, 0.1, 0.0);
			for (int i = 0; i < grainX; i++)
			{
				for (int j = 0; j < grainY; j++)
				{
					glVertex3f(startx - MENUXOFFSET - i*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - j*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - i*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - (j + 1)*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - (j + 1)*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - j*(lengthz) / grainY);
				}
			}
			glPopMatrix();
			glEnd();
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
			// draw map
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
			//end of drawing map



			glDisableVertexAttribArray(0);
		}
		break;
		case 5:
		{
			PatternGrass->Use();
			float startx = 1 + MAPEDGEX / 2 + CUBESIZE;
			float startz = MAPEDGEY / 2 + CUBESIZE;
			float endx = (-MAPEDGEX / 2 - CUBESIZE);
			float endz = (-MAPEDGEY / 2 - CUBESIZE);
			float lengthx = startx - endx;
			float lengthz = startz - endz;
			int grainX = GRASSGRAINX*MENUMULTIPLIER*userGrassMultiplier;
			int grainY = GRASSGRAINY*MENUMULTIPLIER*userGrassMultiplier;
			glEnable(GL_NORMALIZE);
			glBegin(GL_QUADS);
			glPushMatrix();
			glColor3f(0.1, 0.1, 0.0);
			for (int i = 0; i < grainX; i++)
			{
				for (int j = 0; j < grainY; j++)
				{
					glVertex3f(startx - MENUXOFFSET - i*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - j*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - i*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - (j + 1)*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - (j + 1)*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - j*(lengthz) / grainY);
				}
			}
			glPopMatrix();
			glEnd();
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
			// draw map
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
			//end of drawing map

			// Push the GL attribute bits so that we don't wreck any settings
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			// Enable polygon offsets, and offset filled polygons forward by 2.5
			glDisable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(-2.5f, -2.5f);
			// Set the render mode to be line rendering with a thick line width
			glLineWidth(OUTLINE);
			// Set the colour to be white
			glColor3f(.75, .75, .75);
			// Render the object
			PatternSilh->Use();
			//backgroundRand = 9;
			switch (backgroundRand)
			{
			case 0:
				drawHPCrate(0, 0);
				break;
			case 1:
				drawSmokeCrate(0, 0);
				break;
			case 2:
				drawAmmo(0, 0);
				break;
			case 3:
				drawIS3(0, 0, 0, -45, -45);
				break;
			case 4:
				drawAbram(0, 0, 0, -45, -45);
				break;
			case 5:
				drawMine(0, 0);
				break;
			case 6:
				drawT29(0, 0, 0, -45, -45);
				break;
			case 7:
				drawE100(0, 0, 0, -45, -45);
				break;
			case 8:
				drawType59(0, 0, 0, -45, -45);
				break;
			case 9:
				drawRadioCrate(0, 0);
				break;
			}
			PatternSilh->Use(0);
			// Set the polygon mode to be filled triangles 
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(-2.5f, -2.5f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//glShadeModel(GL_FLAT);
			//glEnable(GL_LIGHTING);
			////SetPointLight(GL_LIGHT2, 0, 15, 0, 0.75, 0.75, 0.75);
			// Set the colour to the background
			glColor3f(0.0f, 0.0f, 0.0f);
			// Render the object
			//backgroundRand = 9;
			switch (backgroundRand)
			{
			case 0:
				Pattern->Use();
				drawHPCrate(0, 0);
				Pattern->Use(0);
				break;
			case 1:
				Pattern->Use();
				drawSmokeCrate(0, 0);
				Pattern->Use(0);
				break;
			case 2:
				Pattern->Use();
				drawAmmo(0, 0);
				Pattern->Use(0);
				break;
			case 3:
				PatternCamo->Use();
				drawIS3(0, 0, 0, -45, -45);
				PatternCamo->Use(0);
				break;
			case 4:
				PatternCamo->Use();
				drawAbram(0, 0, 0, -45, -45);
				PatternCamo->Use(0);
				break;
			case 5:
				Pattern->Use();
				drawMine(0, 0);
				Pattern->Use(0);
				break;
			case 6:
				PatternCamo->Use();
				drawT29(0, 0, 0, -45, -45);
				PatternCamo->Use(0);
				break;
			case 7:
				PatternCamo->Use();
				drawE100(0, 0, 0, -45, -45);
				PatternCamo->Use(0);
				break;
			case 8:
				PatternCamo->Use();
				drawType59(0, 0, 0, -45, -45);
				PatternCamo->Use(0);
				break;
			case 9:
				Pattern->Use();
				drawRadioCrate(0, 0);
				Pattern->Use(0);
				break;
			}
			// Pop the state changes off the attribute stack
			// to set things back how they were
			glPopAttrib();

			//glDisable(GL_LIGHT2);
			//glDisable(GL_LIGHTING);

			glDisableVertexAttribArray(0);
		}
		break;
		case 6:
		{
			PatternGrass->Use();
			float startx = 1 + MAPEDGEX / 2 + CUBESIZE;
			float startz = MAPEDGEY / 2 + CUBESIZE;
			float endx = (-MAPEDGEX / 2 - CUBESIZE);
			float endz = (-MAPEDGEY / 2 - CUBESIZE);
			float lengthx = startx - endx;
			float lengthz = startz - endz;
			int grainX = GRASSGRAINX*MENUMULTIPLIER*userGrassMultiplier;
			int grainY = GRASSGRAINY*MENUMULTIPLIER*userGrassMultiplier;
			glEnable(GL_NORMALIZE);
			glBegin(GL_QUADS);
			glPushMatrix();
			glColor3f(0.1, 0.1, 0.0);
			for (int i = 0; i < grainX; i++)
			{
				for (int j = 0; j < grainY; j++)
				{
					glVertex3f(startx - MENUXOFFSET - i*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - j*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - i*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - (j + 1)*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - (j + 1)*(lengthz) / grainY);
					glVertex3f(startx - MENUXOFFSET - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - j*(lengthz) / grainY);
				}
			}
			glPopMatrix();
			glEnd();
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
			// draw map
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
			//end of drawing map

			// Push the GL attribute bits so that we don't wreck any settings
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			// Enable polygon offsets, and offset filled polygons forward by 2.5
			glDisable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(-2.5f, -2.5f);
			// Set the render mode to be line rendering with a thick line width
			glLineWidth(OUTLINE);
			// Set the colour to be white
			glColor3f(.75, .75, .75);
			// Render the object
			PatternSilh->Use();
			drawE100(-3, 0, 12, -135, -45);
			drawIS3(-2, 0, 6, -112.5, -45);
			drawT29(3, 0, 0, -90, 0);
			drawAbram(-2, 0, -6, -67.5, 45);
			drawType59(-3, 0, -12, -45, 45);
			PatternSilh->Use(0);
			// Set the polygon mode to be filled triangles 
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(-2.5f, -2.5f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//glShadeModel(GL_FLAT);
			//glEnable(GL_LIGHTING);
			////SetPointLight(GL_LIGHT2, 0, 15, 0, 0.75, 0.75, 0.75);
			// Set the colour to the background
			glColor3f(0.0f, 0.0f, 0.0f);
			// Render the object
			PatternCamo->Use();
			drawE100(-3, 0, 12, -135, -45);
			drawIS3(-2, 0, 6, -112.5, -45);
			drawT29(3, 0, 0, -90, 0);
			drawAbram(-2, 0, -6, -67.5, 45);
			drawType59(-3, 0, -12, -45, 45);
			PatternCamo->Use(0);
			// Pop the state changes off the attribute stack
			// to set things back how they were
			glPopAttrib();

			//glDisable(GL_LIGHT2);
			//glDisable(GL_LIGHTING);

			glDisableVertexAttribArray(0);
		}
		break;
		}
	}
	else
	{
		PatternGrass->Use();
		float startx = 1 + MAPEDGEX / 2 + CUBESIZE;
		float startz = MAPEDGEY / 2 + CUBESIZE;
		float endx = (-MAPEDGEX / 2 - CUBESIZE);
		float endz = (-MAPEDGEY / 2 - CUBESIZE);
		float lengthx = startx - endx;
		float lengthz = startz - endz;
		int grainX = GRASSGRAINX*MENUMULTIPLIER*userGrassMultiplier;
		int grainY = GRASSGRAINY*MENUMULTIPLIER*userGrassMultiplier;
		glEnable(GL_NORMALIZE);
		glBegin(GL_QUADS);
		glPushMatrix();
		glColor3f(0.1, 0.1, 0.0);
		for (int i = 0; i < grainX; i++)
		{
			for (int j = 0; j < grainY; j++)
			{
				glVertex3f(startx - MENUXOFFSET - i*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - j*(lengthz) / grainY);
				glVertex3f(startx - MENUXOFFSET - i*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - (j + 1)*(lengthz) / grainY);
				glVertex3f(startx - MENUXOFFSET - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - (j + 1)*(lengthz) / grainY);
				glVertex3f(startx - MENUXOFFSET - (i + 1)*(lengthx) / grainX, MENUYOFFSET, startz - MENUZOFFSET - j*(lengthz) / grainY);
			}
		}
		glPopMatrix();
		glEnd();
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
		// draw map
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
		//end of drawing map

		// Push the GL attribute bits so that we don't wreck any settings
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Enable polygon offsets, and offset filled polygons forward by 2.5
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		// Set the render mode to be line rendering with a thick line width
		glLineWidth(OUTLINE);
		// Set the colour to be white
		glColor3f(.75, .75, .75);
		// Render the object
		PatternSilh->Use();
		//backgroundRand = 9;
		switch (backgroundRand)
		{
		case 0:
			drawHPCrate(0, 0);
			break;
		case 1:
			drawSmokeCrate(0, 0);
			break;
		case 2:
			drawAmmo(0, 0);
			break;
		case 3:
			drawIS3(0, 0, 0, -45, -45);
			break;
		case 4:
			drawAbram(0, 0, 0, -45, -45);
			break;
		case 5:
			drawMine(0, 0);
			break;
		case 6:
			drawT29(0, 0, 0, -45, -45);
			break;
		case 7:
			drawE100(0, 0, 0, -45, -45);
			break;
		case 8:
			drawType59(0, 0, 0, -45, -45);
			break;
		case 9:
			drawRadioCrate(0, 0);
			break;
		}
		PatternSilh->Use(0);
		// Set the polygon mode to be filled triangles 
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(-2.5f, -2.5f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glShadeModel(GL_FLAT);
		//glEnable(GL_LIGHTING);
		////SetPointLight(GL_LIGHT2, 0, 15, 0, 0.75, 0.75, 0.75);
		// Set the colour to the background
		glColor3f(0.0f, 0.0f, 0.0f);
		// Render the object
		//backgroundRand = 9;
		switch (backgroundRand)
		{
		case 0:
			Pattern->Use();
			drawHPCrate(0, 0);
			Pattern->Use(0);
			break;
		case 1:
			Pattern->Use();
			drawSmokeCrate(0, 0);
			Pattern->Use(0);
			break;
		case 2:
			Pattern->Use();
			drawAmmo(0, 0);
			Pattern->Use(0);
			break;
		case 3:
			PatternCamo->Use();
			drawIS3(0, 0, 0, -45, -45);
			PatternCamo->Use(0);
			break;
		case 4:
			PatternCamo->Use();
			drawAbram(0, 0, 0, -45, -45);
			PatternCamo->Use(0);
			break;
		case 5:
			Pattern->Use();
			drawMine(0, 0);
			Pattern->Use(0);
			break;
		case 6:
			PatternCamo->Use();
			drawT29(0, 0, 0, -45, -45);
			PatternCamo->Use(0);
			break;
		case 7:
			PatternCamo->Use();
			drawE100(0, 0, 0, -45, -45);
			PatternCamo->Use(0);
			break;
		case 8:
			PatternCamo->Use();
			drawType59(0, 0, 0, -45, -45);
			PatternCamo->Use(0);
			break;
		case 9:
			Pattern->Use();
			drawRadioCrate(0, 0);
			Pattern->Use(0);
			break;
		}
		// Pop the state changes off the attribute stack
		// to set things back how they were
		glPopAttrib();

		//glDisable(GL_LIGHT2);
		//glDisable(GL_LIGHTING);

		glDisableVertexAttribArray(0);
	}
}
void drawMenuText()
{
	if (!ADVANCEMENU)
		menuState = 7;
	switch (menuState)
	{
	case 0:	// main: single player, multiplayer, controlls, graphics, credits
	{
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0., 100., 0., 100.);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1., 1., 1.);
		int y = 70;
		int inc = 0;
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		float amount = 40;
		selectIndex = selectIndex % 10;
		glColor3f(0.125, 0.125, 0.125);
		DoStringBox(60, y, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 2, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 4, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 6, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 8, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 10, 0, (char *)"Please select the map: Single Player");
		glColor3f(1., 1., 1.);
		DoRasterString(60, y, 0, (char *)  "Use Arrow Keys and Enter (Quit: Esc)");
		glColor3f(1., 1., 0);
		//backgroundRand = 9;
		switch (backgroundRand)
		{
		case 0:
			DoRasterString(10, 30, 0, (char *)"Health Boost: Took a few hits? Try snatching one of these bad boys, and you'll be back in action in no time!");
			break;
		case 1:
			DoRasterString(10, 30, 0, (char *)"Smoke Boost: Do you like ninjas? These Ninjutsu certified smoke canisters can take your breath away!");
			break;
		case 2:
			DoRasterString(10, 30, 0, (char *)"Ammo Boost: Don't you hated when your finger gets stuck on the fire button? This will feed that addiction.");
			break;
		case 3:
			DoRasterString(10, 30, 0, (char *)"IS3: This tanks is called Joseph Stalin 3 or IS3 ... but trust me, nothing about this tank is stalin' ...");
			break;
		case 4:
			DoRasterString(10, 30, 0, (char *)"Abram: How Abram tanks moves so fast? Well ask the turbine jet engine behind it!");
			break;
		case 5:
			DoRasterString(10, 30, 0, (char *)"Dima's present: Dima (our powerup supplier) is a psychopath ... watch out for these powerups!");
			break;
		case 6:
			DoRasterString(10, 30, 0, (char *)"T29: Hate groupmates? just remember that this tank has 6 crew members ... Good Lord!");
			break;
		case 7:
			DoRasterString(10, 30, 0, (char *)"E100: Don't let the color intimidate you ... The tank is only as good as its commander.");
			break;
		case 8:
			DoRasterString(10, 30, 0, (char *)"Type 59: If it looks russian, sound russian, or smells russian ... it's from communist China!");
			break;
		case 9:
			DoRasterString(10, 30, 0, (char*)"This bad boy can call in the real fire power!");
			break;
		}
		glColor3f(0.125, 0.125, 0.125);
		DoStringBox(80, 25, 0, (char *)"Developed By Behnam ");
		DoStringBox(80, 27, 0, (char *)"Developed By Behnam ");
		glColor3f(1., 1., 1.);
		DoRasterString(80, 27, 0, (char *)" Developed By:");
		DoRasterString(80, 25, 0, (char *)"    - Behnam Saeedi");
		if (selectIndex == 0)
		{
			glColor3f(1., 1., 0);
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 2, 0, (char *)"Single Player");
		if (selectIndex == 1)
		{
			glColor3f(1., 1., 0);
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 4, 0, (char *)"MultiPlayer");
		if (selectIndex == 2)
		{
			glColor3f(1., 1., 0);
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 6, 0, (char *)"Controlls");
		if (selectIndex == 3)
		{
			glColor3f(1., 1., 0);
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 8, 0, (char *)"Graphics");
		if (selectIndex == 4)
		{
			glColor3f(1., 1., 0);
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 10, 0, (char *)"Credits");
	}
	break;
	case 1:	// single player: Select Tank
	{
		isSingle = true;
		makeAI(isSingle, 'T');
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0., 100., 0., 100.);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1., 1., 1.);
		int y = 70;
		int inc = 0;
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		float amount = 40;
		selectIndex = selectIndex % 10;
		glColor3f(0.125, 0.125, 0.125);
		DoStringBox(55, y, 0, (char *)   "Please select the map: Single Player----------------");
		glColor3f(1., 1., 1.);
		DoRasterString(55, y, 0, (char *)"Use A,D,J,L,4, 6 or Enter to select tank (BACK: ESC)");
		DoStringBox(25, 60, 0, (char *)"Player N");
		DoStringBox(60, 60, 0, (char *)"Player N");
		glColor3f(1., 1., 1.);
		DoRasterString(25, 60, 0, (char *)"Player 1");
		DoRasterString(60, 60, 0, (char *)"Player 2");
		glColor3f(0.125, 0.125, 0.125);
		DoStringBox(80, 25, 0, (char *)"Developed By Behnam ");
		DoStringBox(80, 27, 0, (char *)"Developed By Behnam ");
		glColor3f(1., 1., 1.);
		DoRasterString(80, 27, 0, (char *)" Developed By:");
		DoRasterString(80, 25, 0, (char *)"    - Behnam Saeedi");
	}
	break;
	case 2:	// multiplayer: Select Tank
	{
		isSingle = false;
		makeAI(isSingle, 'T');
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0., 100., 0., 100.);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1., 1., 1.);
		int y = 70;
		int inc = 0;
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		float amount = 40;
		selectIndex = selectIndex % 10;
		glColor3f(0.125, 0.125, 0.125);
		DoStringBox(55, y, 0, (char *)   "Please select the map: Single Player----------------");
		glColor3f(1., 1., 1.);
		DoRasterString(55, y, 0, (char *)"Use A,D,J,L,4, 6 or Enter to select tank (BACK: ESC)");
		DoStringBox(25, 60, 0, (char *)"Player N");
		DoStringBox(60, 60, 0, (char *)"Player N");
		glColor3f(1., 1., 1.);
		DoRasterString(25, 60, 0, (char *)"Player 1");
		DoRasterString(60, 60, 0, (char *)"Player 2");
		glColor3f(0.125, 0.125, 0.125);
		DoStringBox(80, 25, 0, (char *)"Developed By Behnam ");
		DoStringBox(80, 27, 0, (char *)"Developed By Behnam ");
		glColor3f(1., 1., 1.);
		DoRasterString(80, 27, 0, (char *)" Developed By:");
		DoRasterString(80, 25, 0, (char *)"    - Behnam Saeedi");
	}
	break;
	case 3:	// Select Map
	{
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0., 100., 0., 100.);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1., 1., 1.);
		int y = 70;
		int inc = 0;
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		float amount = 40;
		selectIndex = selectIndex % 10;
		glColor3f(0.125, 0.125, 0.125);
		DoStringBox(55, y, 0, (char *)     "Please select the map: Single Player---------");
		DoStringBox(55, y - 2, 0, (char *) "Please select the map: Single Player---------");
		DoStringBox(55, y - 4, 0, (char *) "Please select the map: Single Player---------");
		DoStringBox(55, y - 6, 0, (char *) "Please select the map: Single Player---------");
		DoStringBox(55, y - 8, 0, (char *) "Please select the map: Single Player---------");
		DoStringBox(55, y - 10, 0, (char *)"Please select the map: Single Player---------");
		DoStringBox(55, y - 12, 0, (char *)"Please select the map: Single Player---------");
		DoStringBox(55, y - 14, 0, (char *)"Please select the map: Single Player---------");
		DoStringBox(55, y - 16, 0, (char *)"Please select the map: Single Player---------");
		DoStringBox(55, y - 18, 0, (char *)"Please select the map: Single Player---------");
		DoStringBox(55, y - 20, 0, (char *)"Please select the map: Single Player---------");
		glColor3f(1., 1., 1.);
		DoRasterString(55, y, 0, (char *)  "Please select the map: (Up, Dowm, ESC)");
		glColor3f(1., 1., 0);
		DoRasterString(65, y - 2 * (selectIndex + 1), 0, (char *)"(Select: Enter)");
		glColor3f(1., 1., 0);
		glColor3f(0.125, 0.125, 0.125);
		DoStringBox(80, 25, 0, (char *)"Developed By Behnam ");
		DoStringBox(80, 27, 0, (char *)"Developed By Behnam ");
		glColor3f(1., 1., 1.);
		DoRasterString(80, 27, 0, (char *)" Developed By:");
		DoRasterString(80, 25, 0, (char *)"    - Behnam Saeedi");
		if (selectIndex == 0)
		{
			glColor3f(1., 1., 0);
			//if ((ActiveButton & LEFT) != 0) 
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 1");
				mapName = "1";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(55, y - 2, 0, (char *)"Classic (Medium)");
		if (selectIndex == 1)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 2");
				mapName = "2";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(55, y - 4, 0, (char *)"Duel (Medium)");
		if (selectIndex == 2)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 7");
				mapName = "7";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(55, y - 6, 0, (char *)"Field (Easy)");
		if (selectIndex == 3)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 9");
				mapName = "9";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(55, y - 8, 0, (char *)"aim_map (Easy)");
		if (selectIndex == 4)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 5");
				mapName = "5";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(55, y - 10, 0, (char *)"Joe Graphics (Hard)");
		if (selectIndex == 5)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 8");
				mapName = "8";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(55, y - 12, 0, (char *)"Limbo (Medium)");
		if (selectIndex == 6)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 3");
				mapName = "3";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(55, y - 14, 0, (char *)"Jungle (Hard)");
		if (selectIndex == 7)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 6");
				mapName = "6";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(55, y - 16, 0, (char *)"Maze (Hard)");
		if (selectIndex == 8)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 4");
				mapName = "4";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(55, y - 18, 0, (char *)"Temple (Medium):");
		if (selectIndex == 9)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" r");
				mapName = "r";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(55, y - 20, 0, (char *)"Random (Easy)");
	}
	break;
	case 4:	// controlls: Display control options
	{
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0., 100., 0., 100.);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1., 1., 1.);
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		float y = 65;

		// Legend
		DoStringBox(45, y, 0, (char *)     "FUNCTION");
		DoStringBox(45, y - 5, 0, (char *) "FUNCTION");
		DoStringBox(45, y - 15, 0, (char *)"FUNCTION");
		DoStringBox(45, y - 20, 0, (char *)"FUNCTION");
		DoStringBox(35.75, y - 25, 0, (char *)"[  ]");
		DoStringBox(45.75, y - 25, 0, (char *)"[  ]");
		DoStringBox(55.75, y - 25, 0, (char *)"[  ]");
		DoStringBox(35, y - 28, 0, (char *)"GAMEKEY");
		DoStringBox(45, y - 28, 0, (char *)"GAMEKEY");
		DoStringBox(55, y - 28, 0, (char *)"GAMEKEY");

		glColor3f(1, 1, 0);

		DoRasterString(45, y, 0, (char *)  "Turret:");
		DoRasterString(45, y - 5, 0, (char*)"Movement:");
		DoRasterString(45, y - 15, 0, (char*)"Ordnance:");
		DoRasterString(45, y - 20, 0, (char *)"Gmae key:");
		glColor3f(1, 1, 1);
		DoRasterString(35.75, y - 25, 0, (char *)"[ - ]");
		DoRasterString(45.75, y - 25, 0, (char *)"[ G ]");
		DoRasterString(55.75, y - 25, 0, (char *)"[ + ]");
		glColor3f(1, 1, 0);
		DoRasterString(35, y - 28, 0, (char *)" Slower");
		DoRasterString(45, y - 28, 0, (char *)" Reset");
		DoRasterString(55, y - 28, 0, (char *)" Faster");

		// Player 1:
		DoStringBox(20, y + 5, 0, (char *) "PLAYER N");

		DoStringBox(15, y, 0, (char *)     "[  ]");
		DoStringBox(25, y, 0, (char *)     "[  ]");

		DoStringBox(20, y - 5, 0, (char *) "[  ]");
		DoStringBox(15, y - 10, 0, (char *)"[  ]");
		DoStringBox(20, y - 10, 0, (char *)"[  ]");
		DoStringBox(25, y - 10, 0, (char *)"[  ]");

		DoStringBox(15, y - 15, 0, (char *)"[  ]");
		DoStringBox(20, y - 15, 0, (char *)"[  ]");
		DoStringBox(25, y - 15, 0, (char *)"[  ]");

		glColor3f(1, 1, 1);

		DoRasterString(20, y + 5, 0, (char *) "Player 1");

		DoRasterString(15, y, 0, (char *)     "[ Q ]");
		DoRasterString(25, y, 0, (char *)     "[ E ]");

		DoRasterString(20, y - 5, 0, (char *) "[ W ]");
		DoRasterString(15, y - 10, 0, (char *)"[ A ]");
		DoRasterString(20, y - 10, 0, (char *)"[ S ]");
		DoRasterString(25, y - 10, 0, (char *)"[ D ]");

		DoRasterString(15, y - 15, 0, (char *)"[ R ]");
		DoRasterString(20, y - 15, 0, (char *)"[ F ]");
		DoRasterString(25, y - 15, 0, (char *)"[ C ]");



		// Player 2:

		DoStringBox(75, y + 5, 0, (char *) "PLAYER N");

		DoStringBox(70, y, 0, (char *)     "[  ]");
		DoStringBox(80, y, 0, (char *)     "[  ]");

		DoStringBox(75, y - 5, 0, (char *) "[  ]");
		DoStringBox(70, y - 10, 0, (char *)"[  ]");
		DoStringBox(75, y - 10, 0, (char *)"[  ]");
		DoStringBox(80, y - 10, 0, (char *)"[  ]");

		DoStringBox(70, y - 15, 0, (char *)"[      ]");
		DoStringBox(75, y - 15, 0, (char *)"[  ]");
		DoStringBox(80, y - 15, 0, (char *)"[  ]");

		glColor3f(1, 1, 1);

		DoRasterString(75, y + 5, 0, (char *) "Player 2");

		DoRasterString(70, y, 0, (char *)     "[U/7]");
		DoRasterString(80, y, 0, (char *)     "[O/9]");

		DoRasterString(75, y - 5, 0, (char *) "[I/8]");
		DoRasterString(70, y - 10, 0, (char *)"[J/4]");
		DoRasterString(75, y - 10, 0, (char *)"[K/5]");
		DoRasterString(80, y - 10, 0, (char *)"[L/6]");

		DoRasterString(70, y - 15, 0, (char *)"[Y/Entr]");
		DoRasterString(75, y - 15, 0, (char *)"[H/0]");
		DoRasterString(80, y - 15, 0, (char *)"[N/.]");

	}
	break;
	case 5:	// graphics: Display Graphics option
	{
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0., 100., 0., 100.);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1., 1., 1.);
		int y = 70;
		int inc = 0;
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		float amount = 40;
		selectIndex = selectIndex % 10;
		glColor3f(0.125, 0.125, 0.125);
		DoStringBox(40, y, 0, (char *)"Please select the map: Single Player");
		DoStringBox(40, y - 2, 0, (char *)"Please select the map: Single Player");
		DoStringBox(40, y - 4, 0, (char *)"Please select the map: Single Player");
		DoStringBox(40, y - 6, 0, (char *)"Please select the map: Single Player");
		DoStringBox(40, y - 8, 0, (char *)"Please select the map: Single Player");
		DoStringBox(40, y - 10, 0, (char *)"Please select the map: Single Player");
		glColor3f(1., 1., 1.);
		DoRasterString(40, y, 0, (char *)  "Use Arrow Keys (Back: Esc)");
		glColor3f(1., 1., 0);
		glColor3f(0.125, 0.125, 0.125);
		DoStringBox(80, 25, 0, (char *)"Developed By Behnam ");
		DoStringBox(80, 27, 0, (char *)"Developed By Behnam ");
		glColor3f(1., 1., 1.);
		DoRasterString(80, 27, 0, (char *)" Developed By:");
		DoRasterString(80, 25, 0, (char *)"    - Behnam Saeedi");
		if (selectIndex == 0)
		{
			glColor3f(1., 1., 0);
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		char buffer1[5];
		itoa(userGrassMultiplier, buffer1, 10);
		char text1[50] = "Grass Count: ";
		DoRasterString(40, y - 2, 0, (char *)strcat(text1, buffer1));

		if (selectIndex == 1)
		{
			glColor3f(1., 1., 0);
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		char buffer2[5];
		itoa((int)(TANKSPEED * 100), buffer2, 10);
		char text2[50] = "Movement Speed: ";
		DoRasterString(40, y - 4, 0, (char *)strcat(text2, buffer2));

		if (selectIndex == 2)
		{
			glColor3f(1., 1., 0);
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		char buffer3[5];
		int shellspeed = SHELLSPEED / 100;
		itoa(shellspeed, buffer3, 10);
		char text3[50] = "Shell Speed: ";
		DoRasterString(40, y - 6, 0, (char *)strcat(text3, buffer3));

	}
	break;
	case 6:	// credits: Display contact information
	{
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0., 100., 0., 100.);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1., 1., 1.);
		int y = 70;
		int inc = 0;
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);
		glColor3f(0.125, 0.125, 0.125);
		DoStringBox(45, 67, 0, (char *)"Developed By Behnam ");
		DoStringBox(45, 65, 0, (char *)"Developed By Behnam ");
		glColor3f(1., 1., 1.);
		DoRasterString(45, 67, 0, (char *)" Developed By:");
		DoRasterString(45, 65, 0, (char *)"    - Behnam Saeedi");
	}
	break;
	case 7:	// traditional menu
	{
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0., 100., 0., 100.);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(1., 1., 1.);
		int y = 70;
		int inc = 0;
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		float amount = 40;
		selectIndex = selectIndex % 10;
		glColor3f(0.125, 0.125, 0.125);
		DoStringBox(60, y, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 2, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 4, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 6, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 8, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 10, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 12, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 14, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 16, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 18, 0, (char *)"Please select the map: Single Player");
		DoStringBox(60, y - 20, 0, (char *)"Please select the map: Single Player");
		switch (PlayerOne)
		{
		case 0:
			DoStringBoxColor(72, y, 0, (char *)   "", 0.5, 0.5, 0);
			break;
		case 1:
			DoStringBoxColor(72, y, 0, (char *)   "", 0, 0, 1);
			break;
		case 2:
			DoStringBoxColor(72, y, 0, (char *)   "", 0.38, 0.2, 0.01);
			break;
		case 3:
			DoStringBoxColor(72, y, 0, (char *)   "", 1, 0.1, 0.5);
			break;
		case 4:
			DoStringBoxColor(72, y, 0, (char *)   "", 0, 0.5, 0);
			break;
		}
		DoStringBox(74, y, 0, (char *)   "");
		switch (PlayerTwo)
		{
		case 0:
			DoStringBoxColor(76, y, 0, (char *)   "", 0.5, 0.5, 0);
			break;
		case 1:
			DoStringBoxColor(76, y, 0, (char *)   "", 0, 0, 1);
			break;
		case 2:
			DoStringBoxColor(76, y, 0, (char *)   "", 0.38, 0.2, 0.01);
			break;
		case 3:
			DoStringBoxColor(76, y, 0, (char *)   "", 1, 0.1, 0.5);
			break;
		case 4:
			DoStringBoxColor(76, y, 0, (char *)   "", 0, 0.5, 0);
			break;
		}
		glColor3f(1., 1., 1.);
		DoRasterString(60, y, 0, (char *)"Please select the map:");
		DoRasterString(72, y, 0, (char *)"1     vs     2");
		glColor3f(1., 1., 0);
		if (AIACTIVE)
		{
			if (isSingle)
				DoRasterString(70, y - 2 * (selectIndex + 1), 0, (char *)"<-Single Player->");
			else
				DoRasterString(70, y - 2 * (selectIndex + 1), 0, (char *)"<-CO-OP->");
		}
		//backgroundRand = 9;
		switch (backgroundRand)
		{
		case 0:
			DoRasterString(10, 30, 0, (char *)"Health Boost: Took a few hits? Try snatching one of these bad boys, and you'll be back in action in no time!");
			break;
		case 1:
			DoRasterString(10, 30, 0, (char *)"Smoke Boost: Do you like ninjas? These Ninjutsu certified smoke canisters can take your breath away!");
			break;
		case 2:
			DoRasterString(10, 30, 0, (char *)"Ammo Boost: Don't you hated when your finger gets stuck on the fire button? This will feed that addiction.");
			break;
		case 3:
			DoRasterString(10, 30, 0, (char *)"IS3: This tanks is called Joseph Stalin 3 or IS3 ... but trust me, nothing about this tank is stalin' ...");
			break;
		case 4:
			DoRasterString(10, 30, 0, (char *)"Abram: How Abram tanks moves so fast? Well ask the turbine jet engine behind it!");
			break;
		case 5:
			DoRasterString(10, 30, 0, (char *)"Dima's present: Dima (our powerup supplier) is a psychopath ... watch out for these powerups!");
			break;
		case 6:
			DoRasterString(10, 30, 0, (char *)"T29: Hate groupmates? just remember that this tank has 6 crew members ... Good Lord!");
			break;
		case 7:
			DoRasterString(10, 30, 0, (char *)"E100: Don't let the color intimidate you ... The tank is only as good as its commander.");
			break;
		case 8:
			DoRasterString(10, 30, 0, (char *)"Type 59: If it looks russian, sound russian, or smells russian ... it's from communist China!");
			break;
		case 9:
			DoRasterString(10, 30, 0, (char*)"This bad boy can call in the real fire power!");
			break;
		}
		glColor3f(0.125, 0.125, 0.125);
		DoStringBox(80, 25, 0, (char *)"Developed By Behnam ");
		DoStringBox(80, 27, 0, (char *)"Developed By Behnam ");
		glColor3f(1., 1., 1.);
		DoRasterString(80, 27, 0, (char *)" Developed By:");
		DoRasterString(80, 25, 0, (char *)"    - Behnam Saeedi");
		if (selectIndex == 0)
		{
			glColor3f(1., 1., 0);
			//if ((ActiveButton & LEFT) != 0) 
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 1");
				mapName = "1";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 2, 0, (char *)"Classic (Medium)");
		if (selectIndex == 1)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 2");
				mapName = "2";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 4, 0, (char *)"Duel (Medium)");
		if (selectIndex == 2)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 7");
				mapName = "7";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 6, 0, (char *)"Field (Easy)");
		if (selectIndex == 3)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 9");
				mapName = "9";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 8, 0, (char *)"aim_map (Easy)");
		if (selectIndex == 4)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 5");
				mapName = "5";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 10, 0, (char *)"Joe Graphics (Hard)");
		if (selectIndex == 5)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 8");
				mapName = "8";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 12, 0, (char *)"Limbo (Medium)");
		if (selectIndex == 6)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 3");
				mapName = "3";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 14, 0, (char *)"Jungle (Hard)");
		if (selectIndex == 7)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 6");
				mapName = "6";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 16, 0, (char *)"Maze (Hard)");
		if (selectIndex == 8)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" 4");
				mapName = "4";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 18, 0, (char *)"Temple (Medium):");
		if (selectIndex == 9)
		{
			glColor3f(1., 1., 0);
			if (run)
			{
				run = false;
				//system_hidden("\"Tank-2017.exe\" r");
				mapName = "r";
				lastMap = mapName;
				loadMap();
				isInMenu = false;
				Sleep(500);
				backgroundRand = (backgroundRand + rand()) % 10;
			}
		}
		else
			glColor3f(1., 1., 1.);
		inc++;
		DoRasterString(60, y - 20, 0, (char *)"Random (Easy)");
	}
	break;
	}
}
void drawinGmaeUI()
{
	// Winner Text:
	if (AbramHP <= 0 && IS3HP <= 0)
	{
		glColor3f(1., 1., 1.);
		DoRasterString(MAPEDGEX + 22, 0, -10, (char *)"Draw!");
	}
	else
	{
		if (AbramHP <= 0)
		{
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
			DoRasterString(MAPEDGEX + 22, 0, -10, (char *)"Player 2 Wins!");
			if (!ScoreSet)
			{
				ScoreSet = true;
				IS3Score++;
			}
		}
		if (IS3HP <= 0)
		{
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
			DoRasterString(MAPEDGEX + 22, 0, -10, (char *)"Player 1 Wins!");
			if (!ScoreSet)
			{
				ScoreSet = true;
				AbramScore++;
			}
		}
	}
	// Controlls here:
	// Player 1:
	glColor3f(1, 1, 1);
	DoRasterString(MAPEDGEX + 25, 3, -MAPEDGEY - 45, (char *)"Turret");
	DoRasterString(MAPEDGEX + 25, 3, -MAPEDGEY - 30, (char *)"Q        E");
	DoRasterString(MAPEDGEX + 17.5, 3, -MAPEDGEY - 45, (char *)"Movement");
	DoRasterString(MAPEDGEX + 20, 3, -MAPEDGEY - 29, (char *)"    W");
	DoRasterString(MAPEDGEX + 15, 3, -MAPEDGEY - 28, (char *)"A   S   D");
	DoRasterString(MAPEDGEX + 10, 3, -MAPEDGEY - 40, (char *)"Ordnance");
	DoRasterString(MAPEDGEX + 10, 3, -MAPEDGEY - 26, (char *)"  F   C");
	DoRasterString(MAPEDGEX + 05, 3, -MAPEDGEY - 25, (char *)"G   +   -");
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
	DoRasterString(MAPEDGEX + 25, 3, -MAPEDGEY - 31, (char *)"[    ]     [   ]");
	DoRasterString(MAPEDGEX + 20, 3, -MAPEDGEY - 29, (char *)"   [    ]");
	DoRasterString(MAPEDGEX + 15, 3, -MAPEDGEY - 29, (char *)"[   ][    ][   ]");
	DoRasterString(MAPEDGEX + 10, 3, -MAPEDGEY - 27, (char *)"  [   ] [   ]");
	DoRasterString(MAPEDGEX + 05, 3, -MAPEDGEY - 26, (char *)"[    ][   ][   ]");

	// Player 2:
	glColor3f(1, 1, 1);
	DoRasterString(MAPEDGEX + 25, 3, MAPEDGEY + 15, (char *)"Turret");
	DoRasterString(MAPEDGEX + 25, 3, MAPEDGEY + 34, (char *)"7        9");
	DoRasterString(MAPEDGEX + 17.5, 3, MAPEDGEY + 15, (char *)"Movement");
	DoRasterString(MAPEDGEX + 20, 3, MAPEDGEY + 34, (char *)"    8");
	DoRasterString(MAPEDGEX + 15, 3, MAPEDGEY + 32, (char *)"4   5   6");
	DoRasterString(MAPEDGEX + 10, 3, MAPEDGEY + 15, (char *)"Ordnance");
	DoRasterString(MAPEDGEX + 10, 3, MAPEDGEY + 30, (char *)"  0   .");
	DoRasterString(MAPEDGEX + 05, 3, MAPEDGEY + 29, (char *)"G   +   -");
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
	DoRasterString(MAPEDGEX + 25, 3, MAPEDGEY + 33, (char *)"[   ]     [   ]");
	DoRasterString(MAPEDGEX + 20, 3, MAPEDGEY + 33.5, (char *)"   [   ]");
	DoRasterString(MAPEDGEX + 15, 3, MAPEDGEY + 31, (char *)"[   ][   ][   ]");
	DoRasterString(MAPEDGEX + 10, 3, MAPEDGEY + 29, (char *)" [   ] [   ]");
	DoRasterString(MAPEDGEX + 05, 3, MAPEDGEY + 28, (char *)"[    ][   ][   ]");


	if (AbramShells == 0)
	{

		glColor3f(1 - sin(Time * 1000), 1 - sin(Time * 1000), 1 - sin(Time * 1000));
		DoRasterString(MAPEDGEX + 15, 3, -MAPEDGEY, (char *)"OUT OF AMMO!");
	}
	if (IS3Shells == 0)
	{
		glColor3f(1 - sin(Time * 1000), 1 - sin(Time * 1000), 1 - sin(Time * 1000));
		DoRasterString(MAPEDGEX + 15, 3, MAPEDGEY - 15, (char *)"OUT OF AMMO!");
	}
	if (AbramSmoke == 0)
	{
		glColor3f(1 - sin(Time * 1000), 1 - sin(Time * 1000), 1 - sin(Time * 1000));
		DoRasterString(MAPEDGEX + 22, 3, -MAPEDGEY, (char *)"OUT OF SMOKE!");
	}
	if (IS3Smoke == 0)
	{
		glColor3f(1 - sin(Time * 1000), 1 - sin(Time * 1000), 1 - sin(Time * 1000));
		DoRasterString(MAPEDGEX + 22, 3, MAPEDGEY - 15, (char *)"OUT OF SMOKE!");
	}

	itoa(AbramScore, scoreText, 10);
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
	DoRasterString(MAPEDGEX + 22, 3, -MAPEDGEY - 10, (char *)scoreText);
	itoa(IS3Score, scoreText, 10);
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
	DoRasterString(MAPEDGEX + 22, 3, MAPEDGEY + 10, (char *)scoreText);

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
}
void DoRasterString(float x, float y, float z, char *s)
{
	char c;			// one character to print
	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z + 1);
	for (; (c = *s) != '\0'; s++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}
void DoStringBox(float x, float y, float z, char *s)
{
	char c;			// one character to print
	float len = 0.5;
	float x2 = x + 1;
	for (; (c = *s) != '\0'; s++)
	{
		x2 += len;
	}
	glPushMatrix();
	glColor3f(0.125, 0.125, 0.125);
	glRectd(x - 1, y - 1, x2, y + 1);
	glPopMatrix();
}
void DoStringBoxColor(float x, float y, float z, char *s, float r, float g, float b)
{
	char c;			// one character to print
	float len = 0.5;
	float x2 = x + 1;
	for (; (c = *s) != '\0'; s++)
	{
		x2 += len;
	}
	glPushMatrix();
	glColor3f(r, g, b);
	glRectd(x - 1, y - 1, x2, y + 1);
	glPopMatrix();
}
void DoStringBoxColor(float x, float y, float z, char *s)
{
	char c;			// one character to print
	float len = 0.5;
	float x2 = x + 1;
	for (; (c = *s) != '\0'; s++)
	{
		x2 += len;
	}
	glRectd(x - 1, y - 1, x2, y + 1);
}
#endif