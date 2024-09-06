#include "dasher.h"

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    //update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
        {
            data.runningTime = 0.0;
            //update animation frame
            data.rec.x = data.frame * data.rec.width;
            data.frame++;
            if (data.frame > maxFrame) //it's 5 for scarfy and 7 for nebula (1 less than the total)
            {
                data.frame = 0;
            }
        }
    return data;
}