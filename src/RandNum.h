#pragma once

#include <random>
#include "Vec2.h"

class RandNum
{

private:

    // One generator seeded once, rather than a new std::random_device on every call
    static std::mt19937& engine();

public:

	Vec2 RandomVec2i(int XlowerLimit, int XupperLimit, int YlowerLimit, int YupperLimit);
	Vec2 RandFixedVec2();
	int RandInt(int lowerLimit, int upperLimit);
	float RandFloat(float lowerLimit, float upperLimit);
	Vec2 RandomVec2f(float XlowerLimit, float XupperLimit, float YlowerLimit, float YupperLimit);

};

