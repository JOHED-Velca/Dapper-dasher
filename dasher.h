#ifndef DASHER_H
#define DASHER_H

#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight);
AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame);

#endif  //DASHER_H