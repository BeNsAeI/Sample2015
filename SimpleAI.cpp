#include "SimpleAI.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <string>
#include <time.h>
#include "const.h"
#include "struct.h"
SimpleAI::SimpleAI() {
	myMap = NULL;
	wayPointSet = false;
	pickUp = false;
	srand(time(NULL));
}
SimpleAI::SimpleAI(struct Map * map) {
	myMap = map;
	wayPointSet = false;
	pickUp = false;
}
float SimpleAI::huristic() {
	return 0;
}
/*void SimpleAI::getMove(char AIID, bool*keyBuffer, Crate * Crates) {
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
		wayPoint[0] = targetPos[0] +  rand() % (AIENGAGE/3);
		wayPoint[1] = targetPos[1] +  rand() % (AIENGAGE/3);
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
	
//	tempWayPoint[0] = ;
//	tempWayPoint[1] = ;

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
}*/
void SimpleAI::pathFind(char AIID,bool*keyBuffer, Crate * Crates)
{
	float xdist;
	float ydist;
	float targetDistance;
	float angleSine;
	float angleCosine;
	float current;
	float angle;
	float AIHullAngle;
	bool isClear;
	float AITurretAngle;


	if ((*env->AIAmmo < SHELLSTORAGE / 2 || *env->AIHP < TANKHP * 5 / 8))
	{
		for (int i = 0; i < 9; i++)
		{
			if (*env->AIAmmo < SHELLSTORAGE / 2 && Crates[i].isActive && Crates[i].type == 0)
			{
				wayPoint[0] = Crates[i].X;
				wayPoint[1] = Crates[i].Y;
			}
			if (*env->AIHP < TANKHP * 5 / 8 && Crates[i].isActive && Crates[i].type == 2)
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

	float minx = 100;
	float miny = 100;
	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			if (myMap->isSolid[i][j])
			{
				if (sqrt(pow(myMap->coord[i][j][0] - (env->AIPos[0]), 2) + pow(myMap->coord[i][j][1] - (env->AIPos[1]), 2)) < sqrt(pow(minx - env->AIPos[0], 2) + pow(miny - env->AIPos[1], 2)))
				{
					minx = myMap->coord[i][j][0];
					miny = myMap->coord[i][j][1];
				}
			}
		}
	}

	float FX = (wayPoint[0] - (env->AIPos[0]));
	float FY = (wayPoint[1] - (env->AIPos[1]));
	// F = -k * x
	float FSX = 0;
	float FSY = 0;
	if (!pickUp && fabs(minx - (env->AIPos[0])) < (float)CUBESIZE * 3 / 2)
		FSX = -SC * (fabs(minx - (env->AIPos[0])) - ((float)CUBESIZE * 3 / 2));
	if (!pickUp && fabs(miny - (env->AIPos[1])) < (float)CUBESIZE * 3 / 2)
		FSY = -SC * (fabs(miny - (env->AIPos[1])) - ((float)CUBESIZE * 3 / 2));
	xdist = FX - FSX;
	ydist = FY - FSY;

/*	if ((env->AIPos[1] >= miny - CUBESIZE/2 && env->AIPos[1] < miny + CUBESIZE/2) &&
		(env->AIPos[0] < minx && wayPoint[0] > minx ||
			env->AIPos[0] > minx && wayPoint[0] < minx))
	{
		xdist = minx - (env->AIPos[0]);
		ydist = miny + CUBESIZE - (env->AIPos[1]);
	}
	if ((env->AIPos[0] >= minx - CUBESIZE/2 && env->AIPos[0] < minx + CUBESIZE/2) &&
		(env->AIPos[1] < miny && wayPoint[1] > miny ||
			env->AIPos[1] > miny && wayPoint[1] < miny))
	{
		xdist = minx + CUBESIZE - (env->AIPos[0]);
		ydist = miny - (env->AIPos[1]);
	}*/

	targetDistance = xdist * xdist + ydist * ydist;

	AIHullAngle = *env->AIHullAngle;
	angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
	angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
	current = (float)((int)(AIHullAngle) % 360) * PI / 180;
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

	int state = IDLE;
	float xdist;
	float ydist;
	float targetDistance;
	float angleSine;
	float angleCosine;
	float current;
	float angle;
	float AIHullAngle;
	bool isClear;
	float AITurretAngle;

	// Assessment and State analysis
	if (*env->playerHP > 0)
	{
		// Assess
		xdist = env->playerPos[0] - (env->AIPos[0]);
		ydist = env->playerPos[1] - (env->AIPos[1]);
		targetDistance = sqrt(xdist * xdist + ydist * ydist);
		// Set State:

		if (// AI HP above 75% AND
			((float)*env->AIHP >= ((float)TANKHP * 3.0 / 4.0)) &&
			// AI Ammo above 25% AND
			((float)*env->AIAmmo >= ((float)SHELLSTORAGE / 4.0)) &&
			// Enemy Position further than 5 blocks
			(targetDistance > 5 * CUBESIZE)
			)
			state = ATTACK;

		else
			state = DEFEND;

		if (// Enemy is withing 3 block range AND
			targetDistance < 4 * CUBESIZE && (
				// (Out of Ammo) OR 
			(*env->AIAmmo < 1) ||
				//(HP bellow 50%)
				(float)*env->AIHP < ((float)TANKHP * 2.0 / 4.0)
				)
			)
			state = RUN;

		if (0) // (ammo crate present AND Ammo less than 50%) 
			state = AQUIRE;

		if (0) // (ammo crate present AND Ammo more than or equal to 50%)
			state = ELIMINATECRATE;
	}

	// Act:
	switch (state)
	{
	case ATTACK:
		// Turret:
	{
		AITurretAngle = *env->AITurretAngle;
		AIHullAngle = *env->AIHullAngle;
		isClear = false;
		targetPos[0] = (env->playerPos[0]);
		targetPos[1] = (env->playerPos[1]);
		isClear = (sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) < AIENGAGE);
		xdist = targetPos[0] - (env->AIPos[0]);
		ydist = targetPos[1] - (env->AIPos[1]);
		angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		current = (float)((int)(AITurretAngle + AIHullAngle) % 360) * PI / 180;
		angle = asin(angleSine);
		if (angleCosine < 0)
			angle = PI - angle;
		if (current <= angle + AIMTHRESH * PI / 180 && current >= angle - AIMTHRESH * PI / 180 && isClear)
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
	// Hull:
	{
		if (!wayPointSet)
		{
			targetPos[0] = env->playerPos[0];
			targetPos[1] = env->playerPos[1];
			if(rand() % 2)
				wayPoint[0] = targetPos[0] + rand() % (AIENGAGE / 3);
			else
				wayPoint[0] = targetPos[0] - rand() % (AIENGAGE / 3);
			if (rand() % 2)
				wayPoint[1] = targetPos[1] + rand() % (AIENGAGE / 3);
			else
				wayPoint[1] = targetPos[1] - rand() % (AIENGAGE / 3);
			wayPointSet = true;
		}
		pathFind(AIID, keyBuffer, Crates);
	}
	break;

	case DEFEND:
		// Turret:
	{
		AITurretAngle = *env->AITurretAngle;
		AIHullAngle = *env->AIHullAngle;
		isClear = false;
		targetPos[0] = (env->playerPos[0]);
		targetPos[1] = (env->playerPos[1]);
		isClear = (sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) < (float)AIENGAGE*2.0);
		xdist = targetPos[0] - (env->AIPos[0]);
		ydist = targetPos[1] - (env->AIPos[1]);
		angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		current = (float)((int)(AITurretAngle + AIHullAngle) % 360) * PI / 180;
		angle = asin(angleSine);
		if (angleCosine < 0)
			angle = PI - angle;
		if (current <= angle + AIMTHRESH * PI / 180 && current >= angle - AIMTHRESH * PI / 180 && isClear)
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
	// Hull:
	{
		if (!wayPointSet || pow((env->playerPos[0] - env->AIPos[0]), 2) + pow((env->playerPos[1] - env->AIPos[1]), 2) < pow((AIENGAGE * 2), 2))
		{
			targetPos[0] = env->playerPos[0];
			targetPos[1] = env->playerPos[1];
			if (rand() % 2)
				wayPoint[0] = targetPos[0] - rand() % (AIENGAGE / 2);
			else
				wayPoint[0] = targetPos[0] - rand() % (AIENGAGE / 2);
			if (rand() % 2)
				wayPoint[1] = targetPos[1] + rand() % (AIENGAGE / 2);
			else
				wayPoint[1] = targetPos[1] - rand() % (AIENGAGE / 2);
			wayPointSet = true;
		}
		pathFind(AIID, keyBuffer, Crates);
	}
	break;

	case RUN:
		// Turret:
	{
		AITurretAngle = *env->AITurretAngle;
		AIHullAngle = *env->AIHullAngle;
		isClear = false;
		targetPos[0] = (env->playerPos[0]);
		targetPos[1] = (env->playerPos[1]);
		isClear = (sqrt(pow(targetPos[0] - env->AIPos[0], 2) + pow(targetPos[1] - env->AIPos[1], 2)) < (float)AIENGAGE * 2.0);
		xdist = targetPos[0] - (env->AIPos[0]);
		ydist = targetPos[1] - (env->AIPos[1]);
		angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
		current = (float)((int)(AITurretAngle + AIHullAngle) % 360) * PI / 180;
		angle = asin(angleSine);
		if (angleCosine < 0)
			angle = PI - angle;
		if (current <= angle + AIMTHRESH * PI / 180 && current >= angle - AIMTHRESH * PI / 180 && isClear)
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
	// Hull:
	{
		if (!wayPointSet || pow((env->playerPos[0] - env->AIPos[0]), 2) + pow((env->playerPos[1] - env->AIPos[1]), 2) < pow((AIENGAGE), 2))
		{
			targetPos[0] = env->playerPos[0];
			targetPos[1] = env->playerPos[1];
			if (rand() % 2)
				wayPoint[0] = targetPos[0] - rand() % (AIENGAGE);
			else
				wayPoint[0] = targetPos[0] - rand() % (AIENGAGE);
			if (rand() % 2)
				wayPoint[1] = targetPos[1] + rand() % (AIENGAGE);
			else
				wayPoint[1] = targetPos[1] - rand() % (AIENGAGE);
			wayPointSet = true;
		}
		pathFind(AIID, keyBuffer, Crates);
	}
	break;

	case AQUIRE:

		break;
	case ELIMINATECRATE:

		break;
	}
}