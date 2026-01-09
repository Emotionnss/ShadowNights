#include "game.h"
#include "cprocessing.h"
#include "mainmenu.h"
#include "utils.h"
#include <math.h>

typedef struct
{
    int isActive;
    CP_Vector pos;

} HealthIcon;

CP_Vector playerPos;
float playerRadius;
CP_Vector redEffectPos;
float redEffectRadius;
CP_Vector greenEffectPos;
float greenEffectRadius;

float playerSpeed = 300;
float health = 100.0f;
float healthBarLength = 1200.0f;
float deltaTime = 0.f;

HealthIcon healthRects[10];

void Game_Init(void)
{
    //Init everything
    playerPos = CP_Vector_Set(800, 550);
    playerRadius = 50;

    redEffectPos = CP_Vector_Set(400, 550);
    greenEffectPos = CP_Vector_Set(1200, 550);
    redEffectRadius = greenEffectRadius = 150;

    for (int i = 0; i < 10; i++)
    {
        healthRects[i].isActive = 1;
        healthRects[i].pos = CP_Vector_Set(200 + 125 * i, 200);
    }

    CP_Settings_RectMode(CP_POSITION_CORNER);
}


void Game_Update(void)
{
    //Get delta time
    deltaTime = CP_System_GetDt();

	//Clear background
	CP_Graphics_ClearBackground(CP_Color_Create(150, 150, 150, 255));

    //Move player and check for any area of effects effects
    Move_Player();
    Check_AreaOfEffect();


    //Render game
    Game_Render();
}

void Game_Exit(void)
{

}

void Game_Render(void)
{
    //Draws red circle
    CP_Settings_Fill(CP_Color_Create(175, 0, 0, 255));
    CP_Graphics_DrawCircle(redEffectPos.x, redEffectPos.y, redEffectRadius * 2);

    //Draws green circle
    CP_Settings_Fill(CP_Color_Create(0, 175, 0, 255));
    CP_Graphics_DrawCircle(greenEffectPos.x, greenEffectPos.y, greenEffectRadius * 2);

    //Draws player
    CP_Settings_Fill(CP_Color_Create(0, 0, 255, 255));
    CP_Graphics_DrawCircle(playerPos.x, playerPos.y, playerRadius * 2);

    //Draw big health bar
    CP_Settings_Fill(CP_Color_Create(100, 0, 0, 255));
    CP_Graphics_DrawRect(200, 100, healthBarLength, 75);
    CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
    CP_Graphics_DrawRect(200, 100, healthBarLength * (health / 100), 75);

    for (int i = 0; i < 10; i++)
    {
        //Draw health rects
        if (healthRects[i].isActive == 1)
        {
            CP_Graphics_DrawRect(healthRects[i].pos.x, healthRects[i].pos.y, 75, 50);
        }
    }
}

void Check_AreaOfEffect(void)
{
    //Reduce health if in red circle
    if (AreCirclesIntersecting(playerPos.x, playerPos.y, playerRadius, redEffectPos.x, redEffectPos.y, redEffectRadius) == 1)
    {
        health -= 20 * deltaTime;
        health = CP_Math_ClampFloat(health, 0.f, 100.f);
        Update_Health_Rects();
    }
    //Increase health if in green circle
    else if (AreCirclesIntersecting(playerPos.x, playerPos.y, playerRadius, greenEffectPos.x, greenEffectPos.y, greenEffectRadius) == 1)
    {
        health += 20 * deltaTime;
        health = CP_Math_ClampFloat(health, 0.f, 100.f);
        Update_Health_Rects();
    }
}

void Move_Player(void)
{
    CP_Vector movementDirection = CP_Vector_Zero();

    // Set direction based on triggered button
    if (CP_Input_KeyDown(KEY_W))
    {
        movementDirection = CP_Vector_Set(0, -1);
    }
    else if (CP_Input_KeyDown(KEY_A))
    {
        movementDirection = CP_Vector_Set(-1, 0);
    }
    else if (CP_Input_KeyDown(KEY_S))
    {
        movementDirection = CP_Vector_Set(0, 1);
    }
    else if (CP_Input_KeyDown(KEY_D))
    {
        movementDirection = CP_Vector_Set(1, 0);
    }

    //Move player based off direction
    playerPos = CP_Vector_Add(playerPos, CP_Vector_Scale(movementDirection, playerSpeed * deltaTime));
}

void Update_Health_Rects(void)
{
    //Update the health rects based off current health
    int currentAvailableRects = ceil(health / 10);

    for (int i = 0; i < currentAvailableRects; i++)
    {
        healthRects[i].isActive = 1;
    }

    for (int i = currentAvailableRects; i < 10; i++)
    {
        healthRects[i].isActive = 0;
    }
}
