// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "utils.h"

// Useful macro to count number of items in an array
#define array_count(a) (sizeof(a)/sizeof(*a))

struct HealthIcon
{
	bool isActive;
	AEVec2 pos;
};

AEVec2 playerPos;
float playerRadius;
AEVec2 redEffectPos;
AEVec2 greenEffectPos;
float effectRadius;

float playerSpeed = 500.f;
float health = 100.0f;
float maxHealthBarLength = 1200.0f;
float healthBarLength = maxHealthBarLength;
float deltaTime = 0.f;

HealthIcon healthIcons[10];

//Circle Mesh: 0 is Red, 1 is Green, 2 is Blue (Player) 
//Square Mesh: 0 is Red, 1 is Dark Red (Background of healthbar)
AEGfxVertexList* circleMesh[3] = { 0, 0, 0 }, * squareMesh[2] = { 0, 0 };

AEMtx33 greenEffectTransform = { 0 }, redEffectTransform = { 0 }, playerTransform = { 0 }, healthBarTransform = { 0 }, healthBarBackgroundTransform = { 0 }, healthIconTransforms[10] = { 0 };

void Create_Mesh(void)
{
	// AEGfxTriAdd() takes in 3 sets of 5 parameters,
	// totalling to a whooping 15 parameters to input.
	//
	// Each set represents a vertex, and with three sets
	// we form a single triangle. 
	// 
	// The 5 parameters in a set are:
	// - A vertex's x value
	// - A vertex's y value
	// - A vertex's color in hexadecimal
	// - The U value of a texture to map to this vertex
	// - The V value of a texture to map to this vertex

	// The code below tells Alpha Engine that the mesh will contain 2 
	// triangles that makes up a white square. 
	//
	// Notice that this 1x1 white square is centered around the origin.

	// Makes 3 meshes with 3 different colors for circle (player and effects)
	u32 circleColors[3] = { 0xFFFF0000 , 0xFF00FF00 , 0xFF0000FF };

	// Makes 2 meshes with 2 different colors for squares (health bars)
	u32 squareColors[2] = { 0xFFFF0000, 0xFF660000 };

	for (int i = 0; i < array_count(circleMesh); i++)
	{
		AEGfxMeshStart();

		for (int j = 0; j < 36; j++)
		{
			AEGfxTriAdd(
				AECos((PI / 180) * j * 10.f), AESin((PI / 180) * j * 10.f), circleColors[i], 0.0f, 0.0f,
				AECos((PI / 180) * (j + 1) * 10.f), AESin((PI / 180) * (j + 1) * 10.f), circleColors[i], 0.0f, 0.0f,
				0.f, 0.f, circleColors[i], 0.0f, 0.0f);
		}

		circleMesh[i] = AEGfxMeshEnd();
	}

	for (int i = 0; i < array_count(squareMesh); i++)
	{
		AEGfxMeshStart();

		AEGfxTriAdd(
		-0.5f, -0.5f, squareColors[i], 0.0f, 1.0f,
		0.5f, -0.5f, squareColors[i], 1.0f, 1.0f,
		-0.5f, 0.5f, squareColors[i], 0.0f, 0.0f);

		AEGfxTriAdd(
			0.5f, -0.5f, squareColors[i], 1.0f, 1.0f,
			0.5f, 0.5f, squareColors[i], 1.0f, 0.0f,
			-0.5f, 0.5f, squareColors[i], 0.0f, 0.0f);

		squareMesh[i] = AEGfxMeshEnd();
	}
}

void Move_Player(void)
{
	AEVec2 movementDirection;
	AEVec2Zero(&movementDirection);

	// Set direction based on triggered button
	if (AEInputCheckCurr(AEVK_W))
	{
		movementDirection = { 0, 1 };
	}
	else if (AEInputCheckCurr(AEVK_A))
	{
		movementDirection = { -1, 0 };
	}
	else if (AEInputCheckCurr(AEVK_S))
	{
		movementDirection = { 0, -1 };
	}
	else if (AEInputCheckCurr(AEVK_D))
	{
		movementDirection = { 1, 0 };
	}

	AEVec2 movementScaled;

	AEVec2Scale(&movementScaled, &movementDirection, playerSpeed * deltaTime);

	//Move player based off direction
	AEVec2Add(&playerPos, &playerPos, &movementScaled);
}

void Update_Health_Rects(void)
{
	//Update the health rects based off current health
	int currentAvailableRects = static_cast<int>(ceil(health / 10));

	for (int i = 0; i < currentAvailableRects; i++)
	{
		healthIcons[i].isActive = true;
	}

	for (int i = currentAvailableRects; i < 10; i++)
	{
		healthIcons[i].isActive = false;
	}
}

void Check_AreaOfEffect(void)
{
	//Reduce health if in red circle
	if (AreCirclesIntersecting(playerPos.x, playerPos.y, playerRadius, redEffectPos.x, redEffectPos.y, effectRadius) == 1)
	{
		health -= 20 * deltaTime;
		health = AEClamp(health, 0.f, 100.f);
		healthBarLength = maxHealthBarLength / 100 * health;
		Update_Health_Rects();
	}
	//Increase health if in green circle
	else if (AreCirclesIntersecting(playerPos.x, playerPos.y, playerRadius, greenEffectPos.x, greenEffectPos.y, effectRadius) == 1)
	{
		health += 20 * deltaTime;
		health = AEClamp(health, 0.f, 100.f);
		healthBarLength = maxHealthBarLength / 100 * health;
		Update_Health_Rects();
	}
}

void Set_Mesh_Initial_Transform(void)
{
	//Effects transformation
	{
		AEMtx33 effectScale = { 0 };
		AEMtx33Scale(&effectScale, effectRadius, effectRadius);

		//Green Effect
		AEMtx33 greenEffectTranslate = { 0 };
		AEMtx33Trans(&greenEffectTranslate, greenEffectPos.x, greenEffectPos.y);

		AEMtx33Concat(&greenEffectTransform, &greenEffectTranslate, &effectScale);

		//Red Effect
		AEMtx33 redEffectTranslate = { 0 };
		AEMtx33Trans(&redEffectTranslate, redEffectPos.x, redEffectPos.y);

		AEMtx33Concat(&redEffectTransform, &redEffectTranslate, &effectScale);
	}

	//Health Bar Background
	{
		AEMtx33 healthBarBackgroundScale = { 0 };
		AEMtx33Scale(&healthBarBackgroundScale, maxHealthBarLength, 50.f);

		AEMtx33 healthBarBackgroundTranslate = { 0 };
		AEMtx33Trans(&healthBarBackgroundTranslate, 0.f, 400.f);

		AEMtx33Concat(&healthBarBackgroundTransform, &healthBarBackgroundTranslate, &healthBarBackgroundScale);
	}
}

void Set_Mesh_Transform(void)
{
	//Player transformation
	{
		AEMtx33 playerScale = { 0 };
		AEMtx33Scale(&playerScale, playerRadius, playerRadius);

		AEMtx33 playerTranslate = { 0 };
		AEMtx33Trans(&playerTranslate, playerPos.x, playerPos.y);

		AEMtx33Concat(&playerTransform, &playerTranslate, &playerScale);
	}

	//Transforms for circle made by many squares being rotated
	/*for (int i = 0; i < MAX_TRANSFORMS; i++)
	{
		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, PI / 180.f * (i + 1));

		AEMtx33Concat(&greenTransforms[i], &rotate, &effectScale);
		AEMtx33Concat(&greenTransforms[i], &greenEffectTranslate, &greenTransforms[i]);

		AEMtx33Concat(&redTransforms[i], &rotate, &effectScale);
		AEMtx33Concat(&redTransforms[i], &redEffectTranslate, &redTransforms[i]);

		AEMtx33Concat(&blueTransforms[i], &rotate, &playerScale);
		AEMtx33Concat(&blueTransforms[i], &blueEffectTranslate, &blueTransforms[i]);
	}*/

	//Health Bar
	{
		AEMtx33 healthBarScale = { 0 };
		AEMtx33Scale(&healthBarScale, healthBarLength, 50.f);

		AEMtx33 healthBarTranslate = { 0 };
		AEMtx33Trans(&healthBarTranslate, -((maxHealthBarLength - healthBarLength) / 2), 400.f);

		AEMtx33Concat(&healthBarTransform, &healthBarTranslate, &healthBarScale);
	}

	//Health Icons
	{
		AEMtx33 healthIconScale = { 0 };
		AEMtx33Scale(&healthIconScale, 75.f, 50.f);

		for (int i = 0; i < array_count(healthIcons); i++)
		{
			AEMtx33 healthIconTranslate = { 0 };
			AEMtx33Trans(&healthIconTranslate, healthIcons[i].pos.x, healthIcons[i].pos.y);
			AEMtx33Concat(&healthIconTransforms[i], &healthIconTranslate, &healthIconScale);
		}
	}
}

void Init(void)
{
	//Init everything
	playerPos = { 0.f, 0.f };
	playerRadius = 50.f;

	greenEffectPos = { -400.f, -100.f };
	redEffectPos = { 400.f, -100.f };
	effectRadius = 200.f;

	for (int i = 0, size = array_count(healthIcons) / 2; i < size; i++)
	{
		healthIcons[i].isActive = true;
		healthIcons[i].pos = { -562.5f + 125.f * i, 325.f };
		healthIcons[i + size].isActive = true;
		healthIcons[i + size].pos = { 62.5f + 125.f * i, 325.f };
	}
}

void Update(void)
{
	//Get delta time
	deltaTime = static_cast<float>(AEFrameRateControllerGetFrameTime());

	//Move player and check for any area of effects effects
	Move_Player();
	Check_AreaOfEffect();

	Set_Mesh_Transform();
}

void Render(void)
{
	// Tell the Alpha Engine to get ready to draw something with texture.
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	//Clear background
	AEGfxSetBackgroundColor(0.6f, 0.6f, 0.6f);
	
	//Rendering of multiple squares to create circles
	{
		//for (int i = 0; i < MAX_TRANSFORMS; i++)
		//{
		//	// Tell Alpha Engine to use the matrix in 'transform' to apply onto all 
		//	// the vertices of the mesh that we are about to choose to draw in the next line.
		//	AEGfxSetTransform(greenTransforms[i].m);

		//	// Tell Alpha Engine to draw the mesh with the above settings.
		//	AEGfxMeshDraw(rgbMesh[0], AE_GFX_MDM_TRIANGLES);

		//	AEGfxSetTransform(redTransforms[i].m);

		//	AEGfxMeshDraw(rgbMesh[1], AE_GFX_MDM_TRIANGLES);
		//}

		//Render player above the other stuff
		//for (int i = 0; i < MAX_TRANSFORMS; i++)
		//{
		//	AEGfxSetTransform(blueTransforms[i].m);

		//	// Tell Alpha Engine to draw the mesh with the above settings.
		//	AEGfxMeshDraw(rgbMesh[2], AE_GFX_MDM_TRIANGLES);
		//}
	}

	AEGfxSetTransform(redEffectTransform.m);
	AEGfxMeshDraw(circleMesh[0], AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(greenEffectTransform.m);
	AEGfxMeshDraw(circleMesh[1], AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(playerTransform.m);
	AEGfxMeshDraw(circleMesh[2], AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(healthBarBackgroundTransform.m);
	AEGfxMeshDraw(squareMesh[1], AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(healthBarTransform.m);
	AEGfxMeshDraw(squareMesh[0], AE_GFX_MDM_TRIANGLES);

	for (int i = 0; i < array_count(healthIconTransforms); i++)
	{
		if (healthIcons[i].isActive)
		{
			AEGfxSetTransform(healthIconTransforms[i].m);
			AEGfxMeshDraw(squareMesh[0], AE_GFX_MDM_TRIANGLES);
		}
	}
}

// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	int gGameRunning = 1;

	// Initialization of your own variables go here
	Init();

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	printf("Hello World\n");

	Create_Mesh();
	Set_Mesh_Initial_Transform();

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Basic way to trigger exiting the application
		// when ESCAPE is hit or when the window is closed
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		// Your own update logic goes here
		Update();

		// Your own rendering logic goes here
		Render();

		// Informing the system about the loop's end
		AESysFrameEnd();

	}

	// This will free the Mesh(es).
	for (int i = 0; i < array_count(circleMesh); i++)
	{
		AEGfxMeshFree(circleMesh[i]);
	}

	for (int i = 0; i < array_count(squareMesh); i++)
	{
		AEGfxMeshFree(squareMesh[i]);
	}

	// free the system
	AESysExit();
}