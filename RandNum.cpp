#include "RandNum.h"

int RandNum::RandInt(int lowerLimit, int upperLimit)
{

    //Random Number generator in range Example

   // 
   // seed rand() with srand(int)
   // can use srand(time(null)) - but only goes to 1 sec minimum each time
   // For a range e.g. 10 - 10 r = min + (rand() % (1 + max - min)

    //std::cout << time(NULL) << std::endl;

    //srand(time(NULL));

    //int upperLimit = 100;
    //int lowerLimit = 25;

    //int rndVal = lowerLimit + (rand() % (1 + upperLimit - lowerLimit));

    //std::cout << "Rand value : " << rndVal << std::endl

    std::random_device rd;
    std::uniform_int_distribution<int> val(lowerLimit, upperLimit);
    int rnd = val(rd);

    return rnd;

}


float RandNum::RandFloat(float lowerLimit, float upperLimit)
{
    std::random_device rd;
    std::uniform_real_distribution<double> dist(lowerLimit, upperLimit);
    float rnd = dist(rd);

    return rnd;

}


Vec2 RandNum::RandFixedVec2()
{
    //Random Number generator in range Example

    // 
    // seed rand() with srand(int)
    // can use srand(time(null)) - but only goes to 1 sec minimum each time
    // For a range e.g. 10 - 10 r = min + (rand() % (1 + max - min)

     //std::cout << time(NULL) << std::endl;

    int WIDTH = 1000;
    int HEIGHT = 1000;

    srand(time(NULL));

    //int upperLimit = 100;
    int lowerLimit = 25;

    int rndX = lowerLimit + (rand() % (1 + WIDTH - lowerLimit));
    int rndY = lowerLimit + (rand() % (1 + HEIGHT - lowerLimit));

    Vec2 retVec(rndX, rndY);

    return retVec;

}

Vec2 RandNum::RandomVec2i(int XlowerLimit, int XupperLimit, int YlowerLimit, int YupperLimit)
{
    std::random_device rd;
    std::uniform_int_distribution<int> Xdist(XlowerLimit, XupperLimit);
    int rndX = Xdist(rd);
    std::uniform_int_distribution<int> Ydist(YlowerLimit, YupperLimit);
    int rndY = Ydist(rd);

    Vec2 retVec(rndX, rndY);

    return retVec;

}

Vec2 RandNum::RandomVec2f(float XlowerLimit, float XupperLimit, float YlowerLimit, float YupperLimit)
{
    std::random_device rd;
    std::uniform_real_distribution<double> Xdist(XlowerLimit, XupperLimit);
    float rndX = Xdist(rd);
    std::uniform_real_distribution<double> Ydist(YlowerLimit, YupperLimit);
    float rndY = Ydist(rd);
    

    Vec2 retVec(rndX, rndY);

    return retVec;

}
