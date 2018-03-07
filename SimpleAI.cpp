#include "SimpleAI.h"
#include <iostream>
#include <cmath>
#include <string>
#include "const.h"
#include "struct.h"
SimpleAI::SimpleAI() {
	myMap = NULL;
	wayPointSet = false;
	pickUp = false;
}
SimpleAI::SimpleAI(struct Map * map) {
	myMap = map;
	wayPointSet = false;
	pickUp = false;
}
float SimpleAI::huristic() {
	return 0;
}
void SimpleAI::getMove(char AIID, bool*keyBuffer, Crate * Crates) {
	if (AIID == 'a' || AIID == 'A')
	{
		keyBuffer['e'] = false;
		keyBuffer['q'] = false;
		keyBuffer['w'] = false;
		keyBuffer['s'] = false;
		keyBuffer['a'] = false;
		keyBuffer['d'] = false;
		keyBuffer['f'] = false;
	}
	else {
		keyBuffer['9'] = false;
		keyBuffer['7'] = false;
		keyBuffer['8'] = false;
		keyBuffer['5'] = false;
		keyBuffer['4'] = false;
		keyBuffer['6'] = false;
		keyBuffer['0'] = false;
	}
	//set waypoint
	targetPos[0] = env->playerPos[0];
	targetPos[1] = env->playerPos[1];
	if (!wayPointSet)
	{
		wayPoint[0] = targetPos[0] + rand() % (AIENGAGE/2);
		wayPoint[1] = targetPos[1] + rand() % (AIENGAGE/2);
		pickUp = false;
		wayPointSet = true;
	}
	if ((*env->AIAmmo < SHELLSTORAGE / 2 || *env->AIHP < TANKHP / 2))
	{
		for (int i = 0; i < 9; i++)
		{
			if (*env->AIAmmo < SHELLSTORAGE / 2 && Crates[i].isActive && Crates[i].type == 0)
			{
				wayPoint[0] = Crates[i].X;
				wayPoint[1] = Crates[i].Y;
			}
			if (*env->AIHP < TANKHP / 2 && Crates[i].isActive && Crates[i].type == 2)
			{
				wayPoint[0] = Crates[i].X;
				wayPoint[1] = Crates[i].Y;
			}
		}
		pickUp = true;
		wayPointSet = true;
	}
	else {
		pickUp = false;
	}
	
	float xdist = wayPoint[0] - (env->AIPos[0]);
	float ydist = wayPoint[1] - (env->AIPos[1]);
	float targetDistance = xdist * xdist + ydist * ydist;

	float AIHullAngle = *env->AIHullAngle;
	
	
	float angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
	float angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
	float current = (float)((int)(AIHullAngle) % 360) * PI / 180;
	float angle;
	if (angleSine >= 0 && angleCosine >= 0)
		angle = asin(angleSine);
	if (angleSine < 0 && angleCosine >= 0)
		angle = asin(angleSine);
	if (angleSine >= 0 && angleCosine < 0)
		angle = PI - asin(angleSine);
	if (angleSine < 0 && angleCosine < 0)
		angle = PI - asin(angleSine);
	if (current > angle + AIMTHRESH * PI / 180)
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['d'] = true;
			keyBuffer['a'] = false;
		}
		else {
			keyBuffer['6'] = true;
			keyBuffer['4'] = false;
		}
	}
	if (current < angle - AIMTHRESH * PI / 180)
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['a'] = true;
			keyBuffer['d'] = false;
		}
		else {
			keyBuffer['4'] = true;
			keyBuffer['6'] = false;
		}
	}
	if ((current <= angle + AIMTHRESH * PI / 180) && (current >= angle - AIMTHRESH * PI / 180) && (targetDistance > AIENGAGE*AIENGAGE || pickUp))
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['w'] = true;
			keyBuffer['s'] = false;
		}
		else {
			keyBuffer['8'] = true;
			keyBuffer['5'] = false;
		}
	}
	else 
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['w'] = false;
		}
		else {
			keyBuffer['8'] = false;
		}
		if (targetDistance <= AIENGAGE*AIENGAGE && !pickUp)
		{
			wayPointSet = false;
		}
	}
	
	float AITurretAngle = *env->AITurretAngle;
	AIHullAngle = *env->AIHullAngle;
	bool isClear = false;
	targetPos[0] = (env->playerPos[0]);
	targetPos[1] = (env->playerPos[1]);
	if (*env->AIAmmo < SHELLSTORAGE / 2 || *env->AIHP < TANKHP / 3)
	{
		float minx = 100;
		float miny = 100;
		for (int i = 0; i < 24; i++)
		{
			for (int j = 0; j < 14; j++)
			{
				if (myMap->isSolid[i][j] && myMap->color[i][j][0] < 5)
				{
					if (sqrt(pow(myMap->coord[i][j][0] - (env->AIPos[0]), 2) + pow(myMap->coord[i][j][1] - (env->AIPos[1]), 2)) < sqrt(pow(minx - env->AIPos[0], 2) + pow(miny - env->AIPos[1], 2)))
					{
						minx = myMap->coord[i][j][0];
						miny = myMap->coord[i][j][1];
					}
				}
			}
		}
		if (sqrt(pow(minx - env->AIPos[0], 2) + pow(miny - env->AIPos[1], 2)) < sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) && minx < 100)
		{
			targetPos[0] = minx;
			targetPos[1] = miny;
			isClear = true;
		}
		else
			isClear = false;
	}
	isClear = (sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) < AIENGAGE && *env->playerHP > 0);

	xdist = targetPos[0] - (env->AIPos[0]);
	ydist = targetPos[1] - (env->AIPos[1]);
	angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
	angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
	current = (float)((int)(AITurretAngle + AIHullAngle) % 360) * PI / 180;
	angle = asin(angleSine);
	if (angleCosine < 0)
		angle = PI - angle;
	if(current <= angle + AIMTHRESH * PI / 180 && current >= angle - AIMTHRESH * PI / 180 && isClear)
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['f'] = true;
		}
		else {
			keyBuffer['0'] = true;

		}
	else
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['f'] = false;
		}
		else {
			keyBuffer['0'] = false;

		}
	if (current > angle + AIMTHRESH * PI / 180)
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['e'] = true;
			keyBuffer['q'] = false;
		}
		else {
			keyBuffer['9'] = true;
			keyBuffer['7'] = false;
		}
	}
	if (current < angle - AIMTHRESH * PI / 180)
	{
		if (AIID == 'a' || AIID == 'A')
		{
			keyBuffer['q'] = true;
			keyBuffer['e'] = false;
		}
		else {
			keyBuffer['7'] = true;
			keyBuffer['9'] = false;
		}
	}
}