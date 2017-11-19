#pragma once
#ifndef SIMPLEAI_H
#define SIMPLEAI_H
#include <iostream>
#include "Map.h"
#define PLANSIZE 3
#define AIMTHRESH 1
class SimpleAI {
public:
	SimpleAI();
	SimpleAI(struct Map * map);
	float huristic();
	void getMove(char AIID,bool * keyBuffer);
	struct InnerAIKB {
		bool isAI = false;
		char AIID = NULL;
		float * playerHP = NULL;
		float * playerPos = NULL;
		int * playerAmmo = NULL;
		float * playerHullAngle = NULL;
		float * playerTurretAngle = NULL;
		float * AIHP = NULL;
		float * AIPos = NULL;
		int * AIAmmo = NULL;
		float * AIHullAngle = NULL;
		float * AITurretAngle = NULL;
		SimpleAI * agent;
	};
	struct InnerAIKB * env;
private:
	float synaptic_weights;
	std::string plan[PLANSIZE];
	struct Map * myMap;
};

#endif