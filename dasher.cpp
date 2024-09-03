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
    const int sizeOfNebulae{20};
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

    float finishLine{ nebulae[sizeOfNebulae - 1].pos.x };

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

    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float bgX{};
    float mgX{};
    float fgX{};

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        //delta time (time since last frame)
        const float dT{GetFrameTime()};

        //Start Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 20 * dT; //times dT so movment is frame/rate independent
        mgX -= 40 * dT;
        fgX -= 80 * dT;

        //Reset background
        if (bgX <= -background.width*2)
        {
            bgX = 0.0;
        }
        //Reset midgroundground
        if (mgX <= -background.width*2)
        {
            mgX = 0.0;
        }
        //Reset foreground
        if (fgX <= -background.width*2)
        {
            fgX = 0.0;
        }

        //draw the background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width*2, 0.0}; // initialize the (x;y) right after the first
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        //draw the midground
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width*2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        //draw the foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{fgX + midground.width*2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

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

        //update finish line
        finishLine += nebVel * dT;

        //Update scarfy position
        scarfyData.pos.y += velocity * dT;

        //check to freeze if in the air, update scarfy animation frame
        if (!isInAir)
        {
            //since the functions only make a copy of the data, we have to assign that data to the
            //scarfyData so that the result from the function doesn't get lost and we can use it.
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }

        for(int i =0; i < sizeOfNebulae; i++)
        {
            //update nebula's animation frame
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
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
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}