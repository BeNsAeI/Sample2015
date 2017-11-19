#include "SimpleAI.h"
#include <iostream>
#include <cmath>
#include <string>
SimpleAI::SimpleAI() {
	myMap = NULL;
}
SimpleAI::SimpleAI(struct Map * map) {
	myMap = map;
}
float SimpleAI::huristic() {
	return 0;
}
void SimpleAI::getMove(char AIID, bool*keyBuffer) {
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
	float AITurretAngle = *env->AITurretAngle;
	float AIHullAngle = *env->AIHullAngle;
	float xdist = (env->playerPos[0]) - (env->AIPos[0]);
	float ydist = (env->playerPos[1]) - (env->AIPos[1]);
	float angleSine = -xdist / sqrt(pow(xdist, 2) + pow(ydist, 2));
	float angleCosine = -ydist / sqrt(pow(xdist, 2) + pow(ydist, 2));
	float current = (float)((int)(AITurretAngle + AIHullAngle) % 360) * PI / 180;
	float angle = asin(angleSine);
	if (angleCosine < 0)
		angle = PI - angle;
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