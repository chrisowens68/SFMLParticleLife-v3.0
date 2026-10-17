#pragma once

#include <SFML/Window.hpp>
#include <random>
#include "Vec2.h"

class RandNum
{

public:

	Vec2 RandomVec2i(int XlowerLimit, int XupperLimit, int YlowerLimit, int YupperLimit);
	Vec2 RandFixedVec2();
	int RandInt(int lowerLimit, int upperLimit);
	float RandFloat(float lowerLimit, float upperLimit);
	Vec2 RandomVec2f(float XlowerLimit, float XupperLimit, float YlowerLimit, float YupperLimit);

};

