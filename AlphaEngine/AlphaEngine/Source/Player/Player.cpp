#include "Player.h"
#include "../Projectiles/BaseProjectile.h"
#include "../utils.h"
#include <iostream>
#include "../System/CSVManager.h"


Player::Player()
	: input{ 0.f, 0.f }
	, speed{ 500.f }
	, size{ 100.f }
	, gravity{ -1400.f }
	, isGrounded{ false }
{
	GameObject.GetTransform().SetScale(size, size);
	//transform.SetScale(size, size);
	AEGfxGetCamPosition(&cameraPosition.x, &cameraPosition.y);
	AEGfxGetCamPosition(&cameraPosition.x, &cameraPosition.y);
	AEVec2Set(&cameraOffset, 400.f, -50.f);

	GameObject.sprite = new Sprite(CSVManager::ReadInfo(SPRITESHEET_DATA, "ame"));
	GameObject.sprite->PlayAnimation("bounce", -1, 1.f);

	proj = nullptr;
}

Player::~Player()
{
	for (BaseProjectile* pProj : projectileList)
	{
		if (pProj)
			delete pProj;
	}
	projectileList.clear();

	delete GameObject.sprite;
	GameObject.sprite = nullptr;
}

void Player::HandleInput()
{
	// Zero Out Input Vector
	AEVec2Zero(&input);

	if (AEInputCheckCurr(AEVK_D))
	{
		playerDirection = RIGHT;
		input.x += 1.f;
	}

	if (AEInputCheckCurr(AEVK_A))
	{
		playerDirection = LEFT;
		input.x -= 1.f;
	}


	// Jump (Triggered)
	if (AEInputCheckTriggered(AEVK_W) && isGrounded)
	{
		GameObject.SetVelocity(GameObject.GetVelocity().x, 900.f);
		//velocity.y = 900.f;
		isGrounded = false;
	}
	else if (!AEInputCheckCurr(AEVK_W) && GameObject.GetVelocity().y > 0)
	{
		GameObject.SetVelocity(GameObject.GetVelocity().x, GameObject.GetVelocity().y * 0.5f);
		//velocity.y *= 0.5f;
	}

}

void Player::Update(float deltaTime)
{
	const float playerAcceleration = 4000.f;
	const float maxSpeed = 800.f;
	const float friction = 0.75f;

	//* Gather Input Movement
	if (input.x != 0.f)
	{
		GameObject.SetVelocity(GameObject.GetVelocity().x + input.x * playerAcceleration * deltaTime, GameObject.GetVelocity().y);
	}
	else
	{
		GameObject.SetVelocity(GameObject.GetVelocity().x * friction * deltaTime, GameObject.GetVelocity().y);
	}

	//GameObject.GetVelocity().x = AEClamp(velocity.x, -maxSpeed, maxSpeed);
	GameObject.SetVelocity(AEClamp(GameObject.GetVelocity().x, -maxSpeed, maxSpeed), GameObject.GetVelocity().y + gravity * deltaTime);
	//velocity.y += gravity * deltaTime;

	//* Compute Physics Movement
	AEVec2 position = GameObject.GetTransform().GetPosition();
	position.x += GameObject.GetVelocity().x * deltaTime;
	position.y += GameObject.GetVelocity().y * deltaTime;

	// Placeholder grounded until I can figure collision
	if (position.y <= -400.f)
	{
		position.y = -400.f;
		GameObject.SetVelocity(GameObject.GetVelocity().x, 0.f);
		//GameObject.GetVelocity().y = 0.f;
		isGrounded = true;
	}

	//* Apply Physics Movement
	GameObject.GetTransform().SetPosition(position.x, position.y);

	//* Apply Camera Movement
	AEVec2 cameraLerpVector{ GameObject.GetTransform().GetPosition().x + cameraOffset.x * input.x,
							GameObject.GetTransform().GetPosition().y + cameraOffset.y };

	// Constant is for tracking speed
	AEVec2Lerp(&cameraPosition, &cameraPosition, &cameraLerpVector, deltaTime * 5.0f);
	AEGfxSetCamPosition(cameraPosition.x, cameraPosition.y);

	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		s32 xDir, yDir;
		AEInputGetCursorPosition(&xDir, &yDir);
		AEVec2 mousePos = {static_cast<f32>(xDir), static_cast<f32>(yDir)};
		AEVec2 playerDir = ScreenToWorldPos(mousePos);
		std::cout << playerDir.x << " " << playerDir.y << std::endl;

		projectileList.push_back(new BaseProjectile(position, playerDir));
	}
	for (int i = 0; i < projectileList.size(); i++)
	{
		BaseProjectile* pProj = projectileList[i];

		pProj->Update(deltaTime);
		if (pProj->GetLifespan() <= 0.f)
		{
			delete pProj; // free memory
			projectileList.erase(projectileList.begin() + i);
			i--;
		}
	}

	GameObject.sprite->Update();
	//sprite->Update();
}

void Player::Draw(AEGfxVertexList* mesh)
{
	for (BaseProjectile* pProj : projectileList)
	{
		pProj->Draw(mesh);
	}

	/*AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToAdd(-1.f, -1.f, 0.7f, 0.0f);
	AEGfxSetTransform(transform.GetMatrix().m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);*/

	GameObject.sprite->Render(GameObject.GetTransform());
	//sprite->Render(transform);
}
