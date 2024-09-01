#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}


int main()
{
    //Window Dimensions
    int windowDimensions [2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    //Initializing the window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher!");

    //Acceleration due to gravity (px/s)/s, apostrofy is ignored by the compiler
    const int gravity{1'000};
  

    //----------------------Nebula variables--------------------------
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    //AnimData for nebula
    const int sizeOfNebulae{10};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.x = windowDimensions[0] + i * 300; //same as the other for loop
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
    }

    //int newDistance{0};
    //for(int i = 0; i < sizeOfNebulae; i++)
    //{
    //    nebulae[i].pos.x = windowDimensions[0] + newDistance;
    //    newDistance = newDistance + 300;
    //}

    //-----------------------Scarfy variables---------------------------
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;


    //nebula X velocity (pixels/second)
    int nebVel{-200};

    //Is the rectangle in the air?, empty braces initializes to false.
    bool isInAir{};
    //jump velocity (pixels/second)
    const int jumpVel{-600};

    int velocity{0};

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        //delta time (time since last frame)
        const float dT{GetFrameTime()};

        //Start Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        //Perform Ground Check
        if(isOnGround(scarfyData, windowDimensions[1]))
        {
            //Rectangle is on the ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            //rectangle is on the air
            velocity += gravity * dT;
            isInAir = true;
        }

        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }

        //update each nebula's position
        for(int i = 0; i < sizeOfNebulae; i++)
        {nebulae[i].pos.x += nebVel * dT;}



        //Update scarfy position
        scarfyData.pos.y += velocity * dT;

        //check to freeze if in the air, update scarfy animation frame
        if (!isInAir)
        {
            //update running time
            scarfyData.runningTime += dT;
            if (scarfyData.runningTime >= scarfyData.updateTime)
            {
                scarfyData.runningTime = 0.0;
                scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
                scarfyData.frame++;
                if (scarfyData.frame > 5)
                {
                    scarfyData.frame = 0;
                }
            }
        }

        for(int i =0; i < sizeOfNebulae; i++)
        {
            //update nebula's animation frame
            nebulae[i].runningTime += dT;
            if (nebulae[i].runningTime >= nebulae[i].updateTime)
            {
                nebulae[i].runningTime = 0.0;
                nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
                nebulae[i].frame++;
                if (nebulae[i].frame > 7)
                {
                    nebulae[i].frame = 0;
                }
            }
        }

        for(int i = 0; i < sizeOfNebulae; i++)
        {
            //Draw nebula
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
        }

        //drawing scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

        //Stop Drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}